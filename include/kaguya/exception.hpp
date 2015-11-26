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

	namespace except
	{

		void checkErrorAndThrow(int status, lua_State *state)
		{
			if (status != 0)
			{
				switch (status)
				{
				case LUA_OK:
					return;
				case LUA_ERRRUN:
					throw LuaRuntimeError(status, std::string(lua_tostring(state, -1)));
				case LUA_ERRMEM:
					throw LuaMemoryError(status, "lua memory allocation error");
				case LUA_ERRERR:
					throw LuaRunningError(status, std::string(lua_tostring(state, -1)));
				case LUA_ERRGCMM:
					throw LuaGCError(status, std::string(lua_tostring(state, -1)));
				default:
					throw LuaUnknownError(status, "lua unknown error");
				}
			}
		}
	}
}
