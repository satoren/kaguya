// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/type.hpp"


#define KAGUYA_ERROR_HANDLER_METATABLE "error_handler_kaguya_metatype"

namespace kaguya
{

	inline const char* get_error_message(lua_State *state)
	{
		const char* message = lua_tostring(state, -1);
		return message ? message : "unknown error";
	}



#if KAGUYA_NO_SET_AT_PANIC
	inline void set_at_panic_handler(lua_State* state)
	{
	}
	inline int lua_pcall_wrap(lua_State* state, int argnum, int retnum)
	{
		int result = lua_pcall(state, argnum, retnum, 0);
		return result;
	}
#else
	inline int panic_handler(lua_State *state)
	{
		const char* errormessage = get_error_message(state);
		throw LuaException(lua_status(state), errormessage);
		return 0;
	}
	inline void set_at_panic_handler(lua_State* state)
	{
		lua_atpanic(state, panic_handler);
	}
	inline int lua_pcall_wrap(lua_State* state, int argnum, int retnum)
	{
		try
		{
			lua_call(state, argnum, LUA_MULTRET);
		}
		catch (const LuaException& e)
		{
			return LUA_ERRRUN;
//			return e.status();//status not changed at luajit 2.0.4
		}
		return lua_status(state);
	}
#endif

	struct ErrorHandler
	{
		typedef standard::function<void(int, const char*)> function_type;


		static bool handle(const char* message, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(0, message);
				return true;
			}
			return false;
		}
		static bool handle(int status_code, const char* message, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(status_code, message);
				return true;
			}
			return false;
		}
		static bool handle(int status_code, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(status_code, get_error_message(state));
				return true;
			}
			return false;
		}

		static function_type getHandler(lua_State* state)
		{
			function_type* funptr = getFunctionPointer(state);
			if (funptr)
			{
				return *funptr;
			}
			return function_type();
		}


		static void unregisterHandler(lua_State* state)
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
		static void registerHandler(lua_State* state, function_type f)
		{
			
			set_at_panic_handler(state);
			if (state)
			{
				function_type* funptr = getFunctionPointer(state);
				if (!funptr)
				{
					util::ScopedSavedStack save(state);
					lua_pushlightuserdata(state, handlerRegistryKey());
					void* ptr = lua_newuserdata(state, sizeof(function_type));//dummy data for gc call
					funptr = new(ptr) function_type();

					//create function_type metatable
					lua_newtable(state);
					lua_pushcclosure(state, &error_handler_cleanner, 0);
					lua_setfield(state, -2, "__gc");
					lua_pushvalue(state, -1);
					lua_setfield(state, -1, "__index");
					lua_setmetatable(state, -2);

					lua_settable(state, LUA_REGISTRYINDEX);
				}
				*funptr = f;
			}
		}
		
		static void throwDefaultError(int status, const char* message=0)
		{
			switch (status)
			{
			case LUA_ERRSYNTAX:
				throw LuaSyntaxError(status, message ? std::string(message) : "unknown syntax error");
			case LUA_ERRRUN:
				throw LuaRuntimeError(status, message ? std::string(message) : "unknown runtime error");
			case LUA_ERRMEM:
				throw LuaMemoryError(status, message ? std::string(message) : "lua memory allocation error");
			case LUA_ERRERR:
				throw LuaRunningError(status, message ? std::string(message) : "unknown error");
#if LUA_VERSION_NUM >= 502
			case LUA_ERRGCMM:
				throw LuaGCError(status, message ? std::string(message) : "unknown gc error");
#endif
			default:
				throw LuaUnknownError(status, message ? std::string(message) : "lua unknown error");
			}
		}
	private:

		static void* handlerRegistryKey()
		{
			static void* key;
			return key;
		}
		static function_type* getFunctionPointer(lua_State* state)
		{
			if (state)
			{
				util::ScopedSavedStack save(state);
				lua_pushlightuserdata(state, handlerRegistryKey());
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
			if (ErrorHandler::handle(message.c_str(), state))
			{
				return;
			}
#if !KAGUYA_ERROR_NO_THROW
			throw LuaKaguyaError(message);
#endif
		}
		inline void typeMismatchError(lua_State *state, const std::string& message)
		{
			if(ErrorHandler::handle(message.c_str(), state))
			{
				return;
			}
#if !KAGUYA_ERROR_NO_THROW
			throw LuaTypeMismatch(message);
#endif
		}
		inline void memoryError(lua_State *state, const char* message)
		{
			if (ErrorHandler::handle(message, state))
			{
				return;
			}
#if !KAGUYA_ERROR_NO_THROW
			throw LuaMemoryError(lua_status(state), message ? std::string(message) : "lua memory allocation error");
#endif
		}
		inline bool checkErrorAndThrow(int status, lua_State *state)
		{
			if (status != 0 && status != LUA_YIELD)
			{
				if (ErrorHandler::handle(status, state))
				{
					return false;
				}
#if !KAGUYA_ERROR_NO_THROW
				throwDefaultError(status, lua_tostring(state, -1));
#endif
				return false;
			}
			return true;
		}
	}
}
