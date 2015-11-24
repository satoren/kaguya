#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"

namespace kaguya
{
	struct ThisType {};//for selector

	namespace types
	{
		template<typename T>
		struct type_tag {};
		

		template<class T>
		struct meta_pointer_wrapper { 
			meta_pointer_wrapper(T* p) :ptr(p){}
			T* ptr; 
		};


		bool available_metatable(lua_State* l, const char* t)
		{
			utils::ScopedSavedStack save(l);
			luaL_getmetatable(l,t);
			return LUA_TNIL != lua_type(l, -1);			
		}

		template<typename T>
		inline std::string metatable_name()
		{
			return typeid(T*).name() + std::string("_kaguya_meta_type");
		}

		template<class T>
		T* get_pointer(lua_State* l, int index, type_tag<T> tag)
		{
			int type = lua_type(l, index);
			if (type == LUA_TTABLE)//allow table for __gc
			{
				return 0;
			}
			if (type == LUA_TLIGHTUSERDATA || !available_metatable(l, metatable_name<T>().c_str()))
			{
				return (T*)lua_topointer(l, index);
			}
			else if (lua_topointer(l, index) == 0)
			{
				return 0;
			}
			else
			{
				T* ptr = (T*)luaL_testudata(l, index, metatable_name<T>().c_str());
				if (!ptr)
				{
					standard::shared_ptr<T>* shared_ptr = reinterpret_cast<standard::shared_ptr<T>*>(luaL_testudata(l, index, metatable_name<standard::shared_ptr<T> >().c_str()));
					if (shared_ptr) { ptr = shared_ptr->get(); }
				}
				if (!ptr)
				{
					meta_pointer_wrapper<T>* ptr_wrapper = reinterpret_cast<meta_pointer_wrapper<T>*>(luaL_testudata(l, index, metatable_name<meta_pointer_wrapper<T> >().c_str()));
					if (ptr_wrapper) { ptr = ptr_wrapper->ptr; }
				}

				return ptr;
			}
			return 0;
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
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| luaL_testudata(l, index, metatable_name<T>().c_str()) != 0
				|| !available_metatable(l, metatable_name<T>().c_str())
				|| luaL_testudata(l, index, metatable_name<standard::shared_ptr<T> >().c_str()) != 0
				|| luaL_testudata(l, index, metatable_name<meta_pointer_wrapper<T> >().c_str()) != 0;
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

		inline bool strict_check_type(lua_State* l, int index, type_tag<short> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<unsigned short> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<signed char> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<unsigned char> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<int> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<unsigned int> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<int64_t> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strict_check_type(lua_State* l, int index, type_tag<uint64_t> tag)
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

		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T> tag)
		{
			return  lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatable_name<T>().c_str())
				|| luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<const T&> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatable_name<T>().c_str())
				|| check_type(l, index, type_tag<T>());
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T&> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatable_name<T>().c_str())
				|| luaL_testudata(l, index, metatable_name<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool check_type(lua_State* l, int index, type_tag<T*> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| lua_type(l, index) == LUA_TTABLE //allow table for __gc
				|| lua_topointer(l, index) == 0
				|| !available_metatable(l, metatable_name<T>().c_str())
				|| luaL_testudata(l, index, metatable_name<T>().c_str())
				|| luaL_testudata(l, index, metatable_name<standard::shared_ptr<T> >().c_str())
				|| luaL_testudata(l, index, metatable_name<meta_pointer_wrapper<T> >().c_str());
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

		inline bool check_type(lua_State* l, int index, type_tag<int64_t> tag)
		{
#if LUA_VERSION_NUM >= 503
			return lua_isinteger(l, index) != 0;
#else
			return lua_isnumber(l, index) != 0;
#endif
		}
		inline bool check_type(lua_State* l, int index, type_tag<unsigned int> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<int> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<unsigned short> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<short> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<unsigned char> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
		}
		inline bool check_type(lua_State* l, int index, type_tag<signed char> tag)
		{
			return check_type(l, index, type_tag<int64_t>());
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

		template<typename T>
		inline T get(lua_State* l, int index, type_tag<T> tag = type_tag<T>())
		{
			T* pointer = get_pointer(l, index, type_tag<T>());
			if (!pointer)
			{
				throw std::invalid_argument("type mismatch!!");
			}
			return *pointer;
		}
		template<typename T>
		inline T get(lua_State* l, int index, type_tag<const T&> tag = type_tag<const T&>())
		{
			return get(l, index, type_tag<T>());
		}
		template<typename T>
		inline T& get(lua_State* l, int index, type_tag<T&> tag = type_tag<T&>())
		{
			T* pointer = get_pointer(l, index, type_tag<T>());
			if (!pointer)
			{
				throw std::invalid_argument("type mismatch!!");
			}
			return *pointer;
		}
		template<typename T>
		inline T* get(lua_State* l, int index, type_tag<T*> tag = type_tag<T*>())
		{
			return get_pointer(l, index, type_tag<T>());
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
		
		inline int64_t get(lua_State* l, int index, type_tag<int64_t> tag = type_tag<int64_t>())
		{
#if LUA_VERSION_NUM >= 503
			return int64_t(lua_tointeger(l, index));
#else
			return int64_t(lua_tonumber(l, index));
#endif
		}
		inline unsigned int get(lua_State* l, int index, type_tag<unsigned int> tag = type_tag<unsigned int>())
		{
			return (unsigned int)(get(l, index, type_tag<int64_t>()));
		}
		inline int get(lua_State* l, int index, type_tag<int> tag = type_tag<int>())
		{
			return int(get(l, index, type_tag<int64_t>()));
		}
		inline unsigned int get(lua_State* l, int index, type_tag<unsigned short> tag = type_tag<unsigned short>())
		{
			return (unsigned short)(get(l, index, type_tag<int64_t>()));
		}
		inline int get(lua_State* l, int index, type_tag<short> tag = type_tag<short>())
		{
			return short(get(l, index, type_tag<int64_t>()));
		}
		inline unsigned int get(lua_State* l, int index, type_tag<unsigned char> tag = type_tag<unsigned char>())
		{
			return (unsigned char)(get(l, index, type_tag<int64_t>()));
		}
		inline int get(lua_State* l, int index, type_tag<signed char> tag = type_tag<signed char>())
		{
			return(signed char)(get(l, index, type_tag<int64_t>()));
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
		inline int push(lua_State* l, int64_t v)
		{
#if LUA_VERSION_NUM >= 503
			lua_pushinteger(l, v);
#else
			lua_pushnumber(l, v);
#endif
			return 1;
		}
		inline int push(lua_State* l, uint64_t v)
		{
			return push(l, int64_t(v));
		}

		inline int push(lua_State* l, int v)
		{
			return push(l, int64_t(v));
		}
		inline int push(lua_State* l, unsigned int v)
		{
			return push(l, int64_t(v));
		}
		inline int push(lua_State* l, short v)
		{
			return push(l, int(v));
		}
		inline int push(lua_State* l, unsigned short v)
		{
			return push(l, int(v));
		}
		inline int push(lua_State* l, signed char v)
		{
			return push(l, int(v));
		}
		inline int push(lua_State* l, unsigned char v)
		{
			return push(l, int(v));
		}
		inline int push(lua_State* l, const char* v)
		{
			lua_pushstring(l, v);
			return 1;
		}
		inline int push(lua_State* l, std::string& s)
		{
			lua_pushlstring(l, s.c_str(), s.size());
			return 1;
		}
		inline int push(lua_State* l, const std::string& s)
		{
			lua_pushlstring(l, s.c_str(), s.size());
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

#if defined(_HAS_RVALUE_REFERENCES) || defined(__cpp_rvalue_references)
		inline int push(lua_State* l, std::string&& s)
		{
			lua_pushlstring(l, s.data(), s.size());
			return 1;
		}
#endif
		template<typename T>
		inline int push(lua_State* l, T& v)
		{
			if (!available_metatable(l, metatable_name<T>().c_str()))
			{
				lua_pushlightuserdata(l, &v);
			}
			else
			{
				typedef meta_pointer_wrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(&v);
				luaL_setmetatable(l, metatable_name<wrapper_type>().c_str());
			}
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, T* v)
		{
			if (!available_metatable(l, metatable_name<T>().c_str()))
			{
				lua_pushlightuserdata(l, v);
			}
			else
			{
				typedef meta_pointer_wrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(v);
				luaL_setmetatable(l, metatable_name<wrapper_type>().c_str());
			}
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, const T* v)
		{
			if (!available_metatable(l, metatable_name<T>().c_str()))
			{
				lua_pushlightuserdata(l, v);
			}
			else
			{
				typedef meta_pointer_wrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(v);
				luaL_setmetatable(l, metatable_name<wrapper_type>().c_str());
			}
			return 1;
		}
#include "kaguya/gen/push_tuple.inl"


#include "kaguya/gen/constructor.inl"

		template<typename T>inline void destructor(T* pointer)
		{
			if (pointer)
			{
				pointer->~T();
			}
		}


	}

};
