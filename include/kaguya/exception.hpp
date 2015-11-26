#pragma once

#include <exception>

#include "kaguya/utils.hpp"



namespace kaguya
{
	class lua_exception :public std::exception
	{
		int status_;
		std::string what_;
		const char* what_c_;
	public:
		lua_exception(int status, const char* what)throw() :status_(status), what_c_(what) {}
		lua_exception(int status, const std::string what) :status_(status), what_(what), what_c_(0) {}
		int status()const throw(){ return status_; }
		const char* what() const throw() { return what_c_ ? what_c_ : what_.c_str(); }

		~lua_exception()throw(){}
	};
	class lua_runtime_error :public lua_exception {
	public:
		lua_runtime_error(int status, const char* what)throw() :lua_exception(status, what) {}
		lua_runtime_error(int status, const std::string what) :lua_exception(status, what) {}
	};
	class lua_memory_error :public lua_exception {
	public:
		lua_memory_error(int status, const char* what)throw() :lua_exception(status, what) {}
	};
	class lua_running_error :public lua_exception {
	public:
		lua_running_error(int status, const char* what)throw() :lua_exception(status, what) {}
		lua_running_error(int status, const std::string what) :lua_exception(status, what) {}
	};
	class lua_gc_error :public lua_exception {
	public:
		lua_gc_error(int status, const char* what)throw() :lua_exception(status, what) {}
		lua_gc_error(int status, const std::string what) :lua_exception(status, what) {}
	};
	class lua_unknown_error :public lua_exception {
	public:
		lua_unknown_error(int status, const char* what)throw() :lua_exception(status, what) {}
		lua_unknown_error(int status, const std::string what) :lua_exception(status, what) {}
	};
	class lua_type_mismatch :public lua_exception {
	public:
		lua_type_mismatch(const char* what)throw() :lua_exception(0, what) {}
		lua_type_mismatch(const std::string what) :lua_exception(0, what) {}
	};

	namespace except
	{

		void check_error_and_throw(int status, lua_State *state)
		{
			if (status != 0)
			{
				switch (status)
				{
				case LUA_OK:
					return;
				case LUA_ERRRUN:
					throw lua_runtime_error(status, std::string(lua_tostring(state, -1)));
				case LUA_ERRMEM:
					throw lua_memory_error(status, "lua memory allocation error");
				case LUA_ERRERR:
					throw lua_running_error(status, std::string(lua_tostring(state, -1)));
				case LUA_ERRGCMM:
					throw lua_gc_error(status, std::string(lua_tostring(state, -1)));
				default:
					throw lua_unknown_error(status, "lua unknown error");
				}
			}
		}
	}
}
