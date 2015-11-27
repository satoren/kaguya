#pragma once

#include <exception>

#include "kaguya/utils.hpp"



namespace kaguya
{
	class LuaException :public std::exception
	{
		int status_;
		std::string what_;
		const char* what_c_;
	public:
		LuaException(int status, const char* what)throw() :status_(status), what_c_(what) {}
		LuaException(int status, const std::string what) :status_(status), what_(what), what_c_(0) {}
		int status()const throw(){ return status_; }
		const char* what() const throw() { return what_c_ ? what_c_ : what_.c_str(); }

		~LuaException()throw(){}
	};
	class LuaRuntimeError :public LuaException {
	public:
		LuaRuntimeError(int status, const char* what)throw() :LuaException(status, what) {}
		LuaRuntimeError(int status, const std::string what) :LuaException(status, what) {}
	};
	class LuaMemoryError :public LuaException {
	public:
		LuaMemoryError(int status, const char* what)throw() :LuaException(status, what) {}
	};
	class LuaRunningError :public LuaException {
	public:
		LuaRunningError(int status, const char* what)throw() :LuaException(status, what) {}
		LuaRunningError(int status, const std::string what) :LuaException(status, what) {}
	};
	class LuaGCError :public LuaException {
	public:
		LuaGCError(int status, const char* what)throw() :LuaException(status, what) {}
		LuaGCError(int status, const std::string what) :LuaException(status, what) {}
	};
	class LuaUnknownError :public LuaException {
	public:
		LuaUnknownError(int status, const char* what)throw() :LuaException(status, what) {}
		LuaUnknownError(int status, const std::string what) :LuaException(status, what) {}
	};
	class LuaTypeMismatch :public LuaException {
	public:
		LuaTypeMismatch(const char* what)throw() :LuaException(0, what) {}
		LuaTypeMismatch(const std::string what) :LuaException(0, what) {}
	};

	class LuaSyntaxError :public LuaException {
	public:
		LuaSyntaxError(const std::string what) :LuaException(0, what) {}
	};

	namespace except
	{

		void checkErrorAndThrow(int status, lua_State *state)
		{
			if (status != 0)
			{
				const char* message=0;
				switch (status)
				{
				case LUA_OK:
				case LUA_YIELD:
					return;
				case LUA_ERRSYNTAX:
					message = lua_tostring(state, -1);
					throw LuaRuntimeError(status, message ? std::string(message) : "unknown syntax error");
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
			}
		}
	}
}
