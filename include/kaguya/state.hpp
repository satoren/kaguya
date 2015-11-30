#pragma once

#include <string>

#include "kaguya/config.hpp"

#include "kaguya/utility.hpp"
#include "kaguya/metatable.hpp"
#include "kaguya/error_handler.hpp"

#include "kaguya/lua_ref_table.hpp"

namespace kaguya
{
	class State
	{
		lua_State *state_;
		bool created_;
		
		//non copyable
		State(const State&);
		State& operator =(const State&);

		static void stderror_out(int status, const char* message)
		{
			std::cerr << message << std::endl;
		}
		void init()
		{
			setErrorHandler(&stderror_out);
			nativefunction::reg_functor_destructor(state_);
		}

	public:
		State() :state_(luaL_newstate()), created_(true)
		{
			init();
		}
		State(lua_State* lua) :state_(lua), created_(false)
		{
			init();
		}
		~State()
		{
			if (created_)
			{
				lua_close(state_);
			}
		}

		void setErrorHandler(standard::function<void(int statuscode,const char*message)> errorfunction)
		{
			util::ScopedSavedStack save(state_);
			ErrorHandler::instance().registerHandler(state_, errorfunction);
		}

		void openlibs()
		{
			util::ScopedSavedStack save(state_);
			luaL_openlibs(state_);
		}

		bool dofile(const std::string& str)
		{
			return dofile(str.c_str());
		}
		bool dofile(const char* file)
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadfile(state_, file);

			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			status = lua_pcall(state_, 0, LUA_MULTRET, 0);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			return true;
		}

		bool dostring(const char* str)
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadstring(state_, str);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}

			status = lua_pcall(state_, 0, LUA_MULTRET, 0);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			return true;
		}
		bool dostring(const std::string& str)
		{
			return dostring(str.c_str());
		}

		bool operator()(const std::string& str)
		{
			return dostring(str);
		}
		bool operator()(const char* str)
		{
			return dostring(str);
		}
		TableKeyReference operator[](const std::string& str)
		{
			return TableKeyReference(globalTable(), LuaRef(state_, str));
		}

		TableKeyReference operator[](const char* str)
		{
			return TableKeyReference(globalTable(),LuaRef(state_, str));
		}

		LuaRef globalTable()
		{
			return LuaRef(state_, GlobalTable());
		}

		void garbageCollect()
		{
			lua_gc(state_, LUA_GCCOLLECT, 0);
		}

		//for Lua module
		LuaRef newLib()
		{
			LuaRef newtable(state_,NewTable());
			newtable.push(state_);
			return newtable;
		}

		lua_State *state() { return state_; };
	};
};
