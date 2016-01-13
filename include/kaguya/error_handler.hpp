#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/type.hpp"


#define KAGUYA_ERROR_HANDLER_METATABLE "error_handler_kaguya_metatype"

namespace kaguya
{

	inline const char* get_error_message(lua_State *state)
	{
		return lua_tostring(state, -1);
	}
	struct ErrorHandler
	{
		typedef standard::function<void(int, const char*)> function_type;


		void handle(const char* message, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(0, message);
			}
		}
		void handle(int status_code, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(status_code, get_error_message(state));
			}
		}

		function_type getHandler(lua_State* state)
		{

			function_type* funptr = getFunctionPointer(state);
			if (funptr)
			{
				return *funptr;
			}
			return function_type();
		}


		void unregisterHandler(lua_State* state)
		{
			if (state)
			{
				function_type* funptr = getFunctionPointer(state);
				if (funptr)
				{
					*funptr = function_type();
				}
			}
		}
		void registerHandler(lua_State* state, function_type f)
		{
			if (state)
			{
				util::ScopedSavedStack save(state);
				lua_pushlightuserdata(state, this);
				if (class_userdata::newmetatable<function_type>(state))//register error handler destructor to Lua state
				{
					lua_pushcclosure(state, &error_handler_cleanner, 0);
					lua_setfield(state, -2, "__gc");
					lua_setfield(state, -1, "__index");
					void* ptr = lua_newuserdata(state, sizeof(function_type));//dummy data for gc call
					if (!ptr) { throw std::runtime_error("critical error. maybe failed memory allocation"); }//critical error
					function_type* funptr = new(ptr) function_type();
					if (!funptr) { throw std::runtime_error("critical error. maybe failed memory allocation"); }//critical error
					class_userdata::setmetatable<function_type>(state);
					lua_settable(state, LUA_REGISTRYINDEX);
					*funptr = f;
				}
				else
				{
					function_type* funptr = getFunctionPointer(state);
					if (funptr)
					{
						*funptr = f;
					}
				}
			}
		}

		static ErrorHandler& instance() {
			static ErrorHandler instance_;
			return instance_;
		}
	private:
		function_type* getFunctionPointer(lua_State* state)
		{
			if (state)
			{
				util::ScopedSavedStack save(state);
				lua_pushlightuserdata(state, this);
				lua_gettable(state, LUA_REGISTRYINDEX);
				function_type* ptr = (function_type*)lua_touserdata(state, -1);
				return ptr;
			}
			return 0;
		}

		ErrorHandler() {}

		ErrorHandler(const ErrorHandler&);
		ErrorHandler& operator=(const ErrorHandler&);

		static int error_handler_cleanner(lua_State *state)
		{
			function_type* ptr = (function_type*)lua_touserdata(state, 1);
			ptr->~function_type();
			return 0;
		}
	};

	namespace except
	{
		inline void OtherError(lua_State *state, const std::string& message)
		{
			ErrorHandler::instance().handle(message.c_str(), state);
#if !KAGUYA_ERROR_NO_THROW
			throw LuaKaguyaError(message);
#endif
		}
		inline void typeMismatchError(lua_State *state, const std::string& message)
		{
			ErrorHandler::instance().handle(message.c_str(), state);
#if !KAGUYA_ERROR_NO_THROW
			throw LuaTypeMismatch(message);
#endif
		}
		inline bool checkErrorAndThrow(int status, lua_State *state)
		{
			if (status != 0 && status != LUA_YIELD)
			{
				ErrorHandler::instance().handle(status, state);
#if !KAGUYA_ERROR_NO_THROW
				const char* message = 0;
				switch (status)
				{
				case LUA_ERRSYNTAX:
					message = lua_tostring(state, -1);
					throw LuaSyntaxError(status, message ? std::string(message) : "unknown syntax error");
				case LUA_ERRRUN:
					message = lua_tostring(state, -1);
					throw LuaRuntimeError(status, message ? std::string(message) : "unknown runtime error");
				case LUA_ERRMEM:
					throw LuaMemoryError(status, "lua memory allocation error");
				case LUA_ERRERR:
					message = lua_tostring(state, -1);
					throw LuaRunningError(status, message ? std::string(message) : "unknown error");
				case LUA_ERRGCMM:
					message = lua_tostring(state, -1);
					throw LuaGCError(status, message ? std::string(message) : "unknown gc error");
				default:
					throw LuaUnknownError(status, "lua unknown error");
				}
#endif
				return false;
			}
			return true;
		}
	}
};
