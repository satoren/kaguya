#pragma once

#include <string>

#include "kaguya/config.hpp"


#define KAGUYA_ERROR_HANDLER_METATABLE "error_handler_kaguya_metatype"

namespace kaguya
{

	const char* get_error_message(lua_State *state)
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
				if (luaL_newmetatable(state, KAGUYA_ERROR_HANDLER_METATABLE))//register error handler destructor to Lua state
				{
					lua_pushcclosure(state, &error_handler_cleanner, 0);
					lua_setfield(state, -2, "__gc");
					lua_setfield(state, -1, "__index");
					void* ptr = lua_newuserdata(state, sizeof(function_type));//dummy data for gc call
					function_type* funptr = new(ptr) function_type();
					luaL_setmetatable(state, KAGUYA_ERROR_HANDLER_METATABLE);
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
};
