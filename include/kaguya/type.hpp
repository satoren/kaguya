#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"

namespace kaguya
{
	namespace types
	{
		template<typename T>
		struct type_tag {};

		template<typename T>
		inline std::string metatable_name()
		{
			return typeid(T).name() + std::string("_kaguya_meta_type");
		}

		template<typename T>
		inline bool strict_check_type(lua_State* l, int index, type_tag<T> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool strict_check_type(lua_State* l, int index, type_tag<const T&> tag)
		{
			return strict_check_type(l, index, type_tag<T>());
		}
		template<typename T>
		inline bool strict_check_type(lua_State* l, int index, type_tag<T&> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool strict_check_type(lua_State* l, int index, type_tag<T*> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}

		inline bool strict_check_type(lua_State* l, int index, type_tag<bool> tag)
		{
			return lua_type(l, index) == LUA_TBOOLEAN;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<float> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strict_check_type(lua_State* l, int index, type_tag<double> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strict_check_type(lua_State* l, int index, type_tag<int> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strict_check_type(lua_State* l, int index, type_tag<const char*> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<std::string> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<const std::string&> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<const std::string> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<int64_t> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) !=0;
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<const T&> tag)
		{
			return check_type(l, index, type_tag<T>());
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T&> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T*> tag)
		{
			return luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}

		inline bool check_type(lua_State* l, int index, type_tag<bool> tag)
		{
			return lua_isboolean(l, index) != 0;
		}
		inline bool check_type(lua_State* l, int index, type_tag<float> tag)
		{
			return lua_isnumber(l, index) != 0;
		}

		inline bool check_type(lua_State* l, int index, type_tag<double> tag)
		{
			return lua_isnumber(l, index) != 0;
		}

		inline bool check_type(lua_State* l, int index, type_tag<int> tag)
		{
#if LUA_VERSION_NUM >= 503
			return lua_isinteger(l, index) != 0;
#else
			return lua_isnumber(l, index) != 0;
#endif
		}

		inline bool check_type(lua_State* l, int index, type_tag<const char*> tag)
		{
			return lua_isstring(l, index) != 0;
		}
		inline bool check_type(lua_State* l, int index, type_tag<std::string> tag)
		{
			return lua_isstring(l, index) != 0;
		}
		inline bool check_type(lua_State* l, int index, type_tag<const std::string&> tag)
		{
			return check_type(l, index, type_tag<std::string>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<const std::string> tag)
		{
			return check_type(l, index, type_tag<std::string>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<int64_t> tag)
		{
#if LUA_VERSION_NUM >= 503
			return lua_isinteger(l, index) != 0;
#else
			return lua_isnumber(l, index) != 0;
#endif
		}
		
		template<typename T>
		inline T get(lua_State* l, int index, type_tag<T> tag = type_tag<T>())
		{
			return *((T*)luaL_checkudata(l, index, metatable_name<T>().c_str()));
		}
		template<typename T>
		inline T get(lua_State* l, int index, type_tag<const T&> tag = type_tag<const T&>())
		{
			return get(l, index, type_tag<T>());
		}
		template<typename T>
		inline T& get(lua_State* l, int index, type_tag<T&> tag = type_tag<T&>())
		{
			return *((T*)luaL_checkudata(l, index, metatable_name<T>().c_str()));
		}
		template<typename T>
		inline T* get(lua_State* l, int index, type_tag<T*> tag = type_tag<T*>())
		{
			return (T*)luaL_testudata(l, index, metatable_name<T>().c_str());
		}

		inline bool get(lua_State* l, int index, type_tag<bool> tag = type_tag<bool>())
		{
			return lua_toboolean(l, index) != 0;
		}
		inline float get(lua_State* l, int index, type_tag<float> tag = type_tag<float>())
		{
			return float(lua_tonumber(l, index));
		}

		inline double get(lua_State* l, int index, type_tag<double> tag = type_tag<double>())
		{
			return lua_tonumber(l, index);
		}

		inline int get(lua_State* l, int index, type_tag<int> tag = type_tag<int>())
		{
#if LUA_VERSION_NUM >= 503
			return int(lua_tointeger(l, index));
#else
			return int(lua_tonumber(l, index));
#endif
		}

		inline const char* get(lua_State* l, int index, type_tag<const char*> tag = type_tag<const char*>())
		{
			return lua_tolstring(l, index, 0);
		}
		inline std::string get(lua_State* l, int index, type_tag<std::string> tag = type_tag<std::string>())
		{
			size_t size = 0;
			const char* buffer = lua_tolstring(l, index, &size);
			return std::string(buffer, size);
		}
		inline std::string get(lua_State* l, int index, type_tag<const std::string&> tag = type_tag<const std::string&>())
		{
			return get(l, index, type_tag<std::string>());
		}
		inline std::string get(lua_State* l, int index, type_tag<const std::string> tag = type_tag<const std::string>())
		{
			return get(l, index, type_tag<std::string>());
		}
		inline int64_t get(lua_State* l, int index, type_tag<int64_t> tag = type_tag<int64_t>())
		{
#if LUA_VERSION_NUM >= 503
			return int64_t(lua_tointeger(l, index));
#else
			return int64_t(lua_tonumber(l, index));
#endif
		}

		struct newtable_tag {};


		inline int push(lua_State* l, bool v)
		{
			lua_pushboolean(l, v);
			return 1;
		}
		inline int push(lua_State* l, float v)
		{
			lua_pushnumber(l, v);
			return 1;
		}
		inline int push(lua_State* l, double v)
		{
			lua_pushnumber(l, v);
			return 1;
		}
		inline int push(lua_State* l, int v)
		{
#if LUA_VERSION_NUM >= 503
			lua_pushinteger(l, v);
#else
			lua_pushnumber(l, v);
#endif
			return 1;
		}
		inline int push(lua_State* l, int64_t v)
		{
#if LUA_VERSION_NUM >= 503
			lua_pushinteger(l, v);
#else
			lua_pushnumber(l, v);
#endif
			return 1;
		}
		inline int push(lua_State* l, const char* v)
		{
			lua_pushstring(l, v);
			return 1;
		}
		inline int push(lua_State* l, std::string& s)
		{
			lua_pushlstring(l, s.data(), s.size());
			return 1;
		}
		inline int push(lua_State* l, const std::string& s)
		{
			lua_pushlstring(l, s.data(), s.size());
			return 1;
		}
		inline int push(lua_State* l, newtable_tag)
		{
			lua_newtable(l);
			return 1;
		}

		template<typename T>
		inline int push(lua_State* l, const T& v)
		{
			void *storage = lua_newuserdata(l, sizeof(T));
			new(storage) T(v);
			luaL_setmetatable(l, types::metatable_name<T>().c_str());
			return 1;
		}

		template<typename T>
		inline int push(lua_State* l, T& v)
		{
			lua_pushlightuserdata(l, &v);
			luaL_setmetatable(l, metatable_name<T>().c_str());
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, T* v)
		{
			lua_pushlightuserdata(l, v);
			luaL_setmetatable(l, metatable_name<T>().c_str());
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l,const T* v)
		{
			lua_pushlightuserdata(l, v);
			luaL_setmetatable(l, metatable_name<const T>().c_str());
			return 1;
		}
//#include "kaguya/gen/push_tuple.inl"


#include "kaguya/gen/constructor.inl"

		template<typename T>inline void destructor(T* pointer)
		{
			pointer->~T();
		}


	}

};
