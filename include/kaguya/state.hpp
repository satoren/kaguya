#pragma once

#include <string>

#include "kaguya/config.hpp"

#include "kaguya/utility.hpp"
#include "kaguya/metatable.hpp"
#include "kaguya/error_handler.hpp"

#include "kaguya/lua_ref_table.hpp"
#include "kaguya/lua_ref_function.hpp"

namespace kaguya
{
	typedef std::pair<std::string, lua_CFunction> LoadLib;
	typedef std::vector<LoadLib> LoadLibs;
	inline LoadLibs NoLoadLib() { return LoadLibs(); };
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
			if (!ErrorHandler::instance().getHandler(state_))
			{
				setErrorHandler(&stderror_out);
			}
			nativefunction::reg_functor_destructor(state_);
		}

	public:

		//! create Lua state with lua standard library
		State() :state_(luaL_newstate()), created_(true)
		{
			init();
			openlibs();
		}

		//! create Lua state with(or without) library
		State(const LoadLibs& libs) :state_(luaL_newstate()), created_(true)
		{
			init();
			openlibs(libs);
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

		void setErrorHandler(standard::function<void(int statuscode, const char*message)> errorfunction)
		{
			util::ScopedSavedStack save(state_);
			ErrorHandler::instance().registerHandler(state_, errorfunction);
		}

		//! load all lua standard library
		void openlibs()
		{
			util::ScopedSavedStack save(state_);
			luaL_openlibs(state_);
		}

		//! load lua library
		void openlib(const LoadLib& lib)
		{
			util::ScopedSavedStack save(state_);

			luaL_requiref(state_, lib.first.c_str(), lib.second, 1);
		}

		//! load lua libraries
		void openlibs(const LoadLibs& libs)
		{
			for (LoadLibs::const_iterator it = libs.begin(); it != libs.end(); ++it)
			{
				openlib(*it);
			}
		}

		/**
		* @name loadfile
		* @brief If there are no errors,compiled file as a Lua function and return.
		*  Otherwise send error message to error handler and return nil reference 
		* @param file  file path of lua script
		* @return reference of lua function 
		*/
		//@{
		LuaFunction loadfile(const std::string& file)
		{
			return loadfile(file.c_str());
		}
		LuaFunction loadfile(const char* file)
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadfile(state_, file);

			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return LuaRef(state_);
			}
			return LuaFunction(state_, StackTop());
		}
		//@}


		/**
		* @name loadstring
		* @brief If there are no errors,compiled string as a Lua function and return.
		*  Otherwise send error message to error handler and return nil reference
		* @param str lua code
		* @return reference of lua function 
		*/
		//@{
		LuaFunction loadstring(const std::string& str)
		{
			return loadstring(str.c_str());
		}
		LuaFunction loadstring(const char* str)
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadstring(state_, str);

			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return LuaRef(state_);
			}
			return LuaFunction(state_, StackTop());
		}
		//@}

		/**
		* @name dofile
		* @brief Loads and runs the given file.
		* @param file file path of lua script
		* @param env execute env table
		* @return If there are no errors, returns true.Otherwise return false
		*/
		//@{
		bool dofile(const std::string& file, const LuaTable& env = LuaTable())
		{
			return dofile(file.c_str(), env);
		}
		bool dofile(const char* file, const LuaTable& env = LuaTable())
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadfile(state_, file);

			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}

			if (!env.isNilref())
			{
				env.push();
#if LUA_VERSION_NUM >= 502
				lua_setupvalue(state_, -2, 1);
#else
				lua_setfenv(state_, -2);
#endif
			}

			status = lua_pcall(state_, 0, LUA_MULTRET, 0);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			return true;
		}
		//@}

		/**
		* @name dostring
		* @brief Loads and runs the given string.
		* @param str lua script cpde
		* @param env execute env table
		* @return If there are no errors, returns true.Otherwise return false
		*/
		bool dostring(const char* str, const LuaTable& env = LuaTable())
		{
			util::ScopedSavedStack save(state_);

			int status = luaL_loadstring(state_, str);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			if (!env.isNilref())
			{
				env.push();
#if LUA_VERSION_NUM >= 502
				lua_setupvalue(state_, -2, 1);
#else
				lua_setfenv(state_, -2);
#endif
			}
			status = lua_pcall(state_, 0, LUA_MULTRET, 0);
			if (status)
			{
				ErrorHandler::instance().handle(status, state_);
				return false;
			}
			return true;
		}
		bool dostring(const std::string& str, const LuaTable& env = LuaTable())
		{
			return dostring(str.c_str(), env);
		}
		bool operator()(const std::string& str)
		{
			return dostring(str);
		}
		bool operator()(const char* str)
		{
			return dostring(str);
		}
		//@}

		//! return element reference from global table
		TableKeyReference operator[](const std::string& str)
		{
			return TableKeyReference(globalTable(), LuaRef(state_, str));
		}

		//! return element reference from global table
		TableKeyReference operator[](const char* str)
		{
			return TableKeyReference(globalTable(), LuaRef(state_, str));
		}

		//! return global table
		LuaTable globalTable()
		{
			return newRef(GlobalTable());
		}

		//! return new Lua reference from argument value
		template<typename T>
		LuaRef newRef(T value)
		{
			return LuaRef(state_, value);
		}

		//! return new Lua table
		LuaTable newTable()
		{
			return LuaTable(state_);
		}
		//! return new Lua table
		LuaTable newTable(int reserve_array, int reserve_record)
		{
			return LuaTable(state_, NewTable(reserve_array, reserve_record));
		}

		//! return new Lua thread
		LuaThread newThread()
		{
			return LuaThread(state_);
		}

		//push to Lua stack
		template<typename T>
		void pushToStack(T value)
		{
			types::push(state_, value);
		}

		//! Garbage Collection of Lua 
		struct GCType
		{
			GCType(lua_State* state) :state_(state) {}

			/**
			* Performs a full garbage-collection cycle.
			*/
			void collect()
			{
				lua_gc(state_, LUA_GCCOLLECT, 0);
			}
			/** Performs an incremental step of garbage collection.
			* @return If returns true,the step finished a collection cycle.
			*/
			bool step()
			{
				return lua_gc(state_, LUA_GCSTEP, 0) == 1;
			}
			/**
			* Performs an incremental step of garbage collection.
			* @args the collector will perform as if that amount of memory (in KBytes) had been allocated by Lua. 
			*/
			bool step(int size)
			{
				return lua_gc(state_, LUA_GCSTEP, size) == 1;
			}
			/**
			* enable gc
			*/
			void restart() { enable(); }
			/**
			* disable gc
			*/
			void stop() { disable(); }

			/**
			* returns the total memory in use by Lua in Kbytes.
			*/
			int count()const { return lua_gc(state_, LUA_GCCOUNT, 0); }

			/**
			* sets arg as the new value for the pause of the collector. Returns the previous value for pause.
			*/
			int steppause(int value) { return lua_gc(state_, LUA_GCSETPAUSE, value); }
			/**
			*  sets arg as the new value for the step multiplier of the collector. Returns the previous value for step.
			*/
			int setstepmul(int value) { return lua_gc(state_, LUA_GCSETSTEPMUL, value); }

			/**
			* enable gc
			*/
			void enable()
			{
				lua_gc(state_, LUA_GCRESTART, 0);
			}

			/**
			* disable gc
			*/
			void disable()
			{
				lua_gc(state_, LUA_GCSTOP, 0);
			}
#if LUA_VERSION_NUM >= 502
			/**
			* returns a boolean that tells whether the collector is running
			*/
			bool isrunning()const { return isenabled(); }
			/**
			* returns a boolean that tells whether the collector is running
			*/
			bool isenabled()const
			{
				return lua_gc(state_, LUA_GCISRUNNING, 0) != 0;
			}
#endif

		private:
			lua_State* state_;
		};

		//! return Garbage collection interface.
		GCType gc()const
		{
			return GCType(state_);
		}
		//! performs a full garbage-collection cycle.
		void garbageCollect()
		{
			gc().collect();
		}

		//! returns the current amount of memory (in Kbytes) in use by Lua.
		size_t useKBytes()const
		{
			return size_t(gc().count());
		}



		/**
		* @brief create Table and push to stack.
		* using for Lua module
		* @return return Lua Table Reference
		*/
		LuaRef newLib()
		{
			LuaTable newtable = newTable();
			newtable.push(state_);
			return newtable;
		}

		/**
		* @brief return lua_State*.
		* @return lua_State*
		*/
		lua_State *state() { return state_; };
	};
};
