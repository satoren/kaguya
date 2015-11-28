#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"

namespace kaguya
{
	struct NewTable {};
	struct NewThread {};
	struct GlobalTable {};

	namespace types
	{
		template<typename T>
		struct typetag {};


		template<class T>
		struct MetaPointerWrapper {
			MetaPointerWrapper(T* p) :ptr(p) {}
			T* ptr;
		};


		bool available_metatable(lua_State* l, const char* t)
		{
			util::ScopedSavedStack save(l);
			luaL_getmetatable(l, t);
			return LUA_TNIL != lua_type(l, -1);
		}

		//faster but no human readable
		//struct metatableName_t
		//{
		//	char metaname[16 + sizeof(intptr_t) * 2];//XXXXXXXX(ptrlen*(8/7))_kaguya_type
		//	const char* c_str()const { return metaname; }
		//	operator std::string()const { return metaname; }
		//};
		//template<typename T>
		//inline metatableName_t metatableName()
		//{
		//	metatableName_t result;
		//	intptr_t ptrvalue = intptr_t(typeid(T*).name());
		//	size_t pos =0;
		//	for (size_t s = 0; s < sizeof(intptr_t) * 8; s+=7, pos++)
		//	{
		//		result.metaname[pos] = 0x80 | ((ptrvalue >> s) & 0x7F);
		//	}
		//	memcpy(&result.metaname[pos], "_kaguya_type", strlen("_kaguya_type"));
		//	result.metaname[pos+ strlen("_kaguya_type")] = '\0';
		//	return result;
		//}

		template<typename T>
		inline std::string metatableName()
		{
			return typeid(T*).name() + std::string("_kaguya_type");
		}

		template<class T>
		T* get_pointer(lua_State* l, int index, typetag<T> tag)
		{
			int type = lua_type(l, index);
			if (type == LUA_TTABLE)//allow table for __gc
			{
				return 0;
			}
			if (type == LUA_TLIGHTUSERDATA || !available_metatable(l, metatableName<T>().c_str()))
			{
				return (T*)lua_topointer(l, index);
			}
			else if (lua_topointer(l, index) == 0)
			{
				return 0;
			}
			else
			{
				T* ptr = (T*)luaL_testudata(l, index, metatableName<T>().c_str());
				if (!ptr)
				{
					standard::shared_ptr<T>* shared_ptr = reinterpret_cast<standard::shared_ptr<T>*>(luaL_testudata(l, index, metatableName<standard::shared_ptr<T> >().c_str()));
					if (shared_ptr) { ptr = shared_ptr->get(); }
				}
				if (!ptr)
				{
					MetaPointerWrapper<T>* ptr_wrapper = reinterpret_cast<MetaPointerWrapper<T>*>(luaL_testudata(l, index, metatableName<MetaPointerWrapper<T> >().c_str()));
					if (ptr_wrapper) { ptr = ptr_wrapper->ptr; }
				}

				return ptr;
			}
			return 0;
		}



		template<typename T>
		inline bool strictCheckType(lua_State* l, int index, typetag<T> tag)
		{
			return luaL_testudata(l, index, metatableName<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool strictCheckType(lua_State* l, int index, typetag<const T&> tag)
		{
			return strictCheckType(l, index, typetag<T>());
		}
		template<typename T>
		inline bool strictCheckType(lua_State* l, int index, typetag<T&> tag)
		{
			return luaL_testudata(l, index, metatableName<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool strictCheckType(lua_State* l, int index, typetag<T*> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| luaL_testudata(l, index, metatableName<T>().c_str()) != 0
				|| !available_metatable(l, metatableName<T>().c_str())
				|| luaL_testudata(l, index, metatableName<standard::shared_ptr<T> >().c_str()) != 0
				|| luaL_testudata(l, index, metatableName<MetaPointerWrapper<T> >().c_str()) != 0;
		}

		inline bool strictCheckType(lua_State* l, int index, typetag<bool> tag)
		{
			return lua_type(l, index) == LUA_TBOOLEAN;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<float> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strictCheckType(lua_State* l, int index, typetag<double> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strictCheckType(lua_State* l, int index, typetag<short> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<unsigned short> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<signed char> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<unsigned char> tag)
		{//need check range?
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<int> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<unsigned int> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<int64_t> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<uint64_t> tag)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}

		inline bool strictCheckType(lua_State* l, int index, typetag<const char*> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<std::string> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<const std::string&> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		inline bool strictCheckType(lua_State* l, int index, typetag<const std::string> tag)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}

		template<typename T>
		inline bool checkType(lua_State* l, int index, typetag<T> tag)
		{
			return  lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatableName<T>().c_str())
				|| luaL_testudata(l, index, metatableName<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool checkType(lua_State* l, int index, typetag<const T&> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatableName<T>().c_str())
				|| checkType(l, index, typetag<T>());
		}
		template<typename T>
		inline bool checkType(lua_State* l, int index, typetag<T&> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| !available_metatable(l, metatableName<T>().c_str())
				|| luaL_testudata(l, index, metatableName<T>().c_str()) != 0;
		}
		template<typename T>
		inline bool checkType(lua_State* l, int index, typetag<T*> tag)
		{
			return lua_type(l, index) == LUA_TLIGHTUSERDATA
				|| lua_type(l, index) == LUA_TTABLE //allow table for __gc
				|| lua_topointer(l, index) == 0
				|| !available_metatable(l, metatableName<T>().c_str())
				|| luaL_testudata(l, index, metatableName<T>().c_str())
				|| luaL_testudata(l, index, metatableName<standard::shared_ptr<T> >().c_str())
				|| luaL_testudata(l, index, metatableName<MetaPointerWrapper<T> >().c_str());
		}

		inline bool checkType(lua_State* l, int index, typetag<bool> tag)
		{
			return lua_isboolean(l, index) != 0;
		}
		inline bool checkType(lua_State* l, int index, typetag<float> tag)
		{
			return lua_isnumber(l, index) != 0;
		}

		inline bool checkType(lua_State* l, int index, typetag<double> tag)
		{
			return lua_isnumber(l, index) != 0;
		}

		inline bool checkType(lua_State* l, int index, typetag<int64_t> tag)
		{
#if LUA_VERSION_NUM >= 503
			return lua_isinteger(l, index) != 0;
#else
			return lua_isnumber(l, index) != 0;
#endif
		}
		inline bool checkType(lua_State* l, int index, typetag<unsigned int> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}
		inline bool checkType(lua_State* l, int index, typetag<int> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}
		inline bool checkType(lua_State* l, int index, typetag<unsigned short> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}
		inline bool checkType(lua_State* l, int index, typetag<short> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}
		inline bool checkType(lua_State* l, int index, typetag<unsigned char> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}
		inline bool checkType(lua_State* l, int index, typetag<signed char> tag)
		{
			return checkType(l, index, typetag<int64_t>());
		}

		inline bool checkType(lua_State* l, int index, typetag<const char*> tag)
		{
			return lua_isstring(l, index) != 0;
		}
		inline bool checkType(lua_State* l, int index, typetag<std::string> tag)
		{
			return lua_isstring(l, index) != 0;
		}
		inline bool checkType(lua_State* l, int index, typetag<const std::string&> tag)
		{
			return checkType(l, index, typetag<std::string>());
		}
		inline bool checkType(lua_State* l, int index, typetag<const std::string> tag)
		{
			return checkType(l, index, typetag<std::string>());
		}

		template<typename T>
		inline T get(lua_State* l, int index, typetag<T> tag = typetag<T>())
		{
			T* pointer = get_pointer(l, index, typetag<T>());
			if (!pointer)
			{
				throw std::invalid_argument("type mismatch!!");
			}
			return *pointer;
		}
		template<typename T>
		inline T get(lua_State* l, int index, typetag<const T&> tag = typetag<const T&>())
		{
			return get(l, index, typetag<T>());
		}
		template<typename T>
		inline T& get(lua_State* l, int index, typetag<T&> tag = typetag<T&>())
		{
			T* pointer = get_pointer(l, index, typetag<T>());
			if (!pointer)
			{
				throw std::invalid_argument("type mismatch!!");
			}
			return *pointer;
		}
		template<typename T>
		inline T* get(lua_State* l, int index, typetag<T*> tag = typetag<T*>())
		{
			return get_pointer(l, index, typetag<T>());
		}


		inline lua_State* get(lua_State* l, int index, typetag<lua_State> tag = typetag<lua_State>())
		{
			return lua_tothread(l, index);
		}


		inline bool get(lua_State* l, int index, typetag<bool> tag = typetag<bool>())
		{
			return lua_toboolean(l, index) != 0;
		}
		inline float get(lua_State* l, int index, typetag<float> tag = typetag<float>())
		{
			return float(lua_tonumber(l, index));
		}

		inline double get(lua_State* l, int index, typetag<double> tag = typetag<double>())
		{
			return lua_tonumber(l, index);
		}

		inline int64_t get(lua_State* l, int index, typetag<int64_t> tag = typetag<int64_t>())
		{
#if LUA_VERSION_NUM >= 503
			return int64_t(lua_tointeger(l, index));
#else
			return int64_t(lua_tonumber(l, index));
#endif
		}
		inline unsigned int get(lua_State* l, int index, typetag<unsigned int> tag = typetag<unsigned int>())
		{
			return (unsigned int)(get(l, index, typetag<int64_t>()));
		}
		inline int get(lua_State* l, int index, typetag<int> tag = typetag<int>())
		{
			return int(get(l, index, typetag<int64_t>()));
		}
		inline unsigned int get(lua_State* l, int index, typetag<unsigned short> tag = typetag<unsigned short>())
		{
			return (unsigned short)(get(l, index, typetag<int64_t>()));
		}
		inline int get(lua_State* l, int index, typetag<short> tag = typetag<short>())
		{
			return short(get(l, index, typetag<int64_t>()));
		}
		inline unsigned int get(lua_State* l, int index, typetag<unsigned char> tag = typetag<unsigned char>())
		{
			return (unsigned char)(get(l, index, typetag<int64_t>()));
		}
		inline int get(lua_State* l, int index, typetag<signed char> tag = typetag<signed char>())
		{
			return(signed char)(get(l, index, typetag<int64_t>()));
		}

		inline const char* get(lua_State* l, int index, typetag<const char*> tag = typetag<const char*>())
		{
			return lua_tolstring(l, index, 0);
		}
		inline std::string get(lua_State* l, int index, typetag<std::string> tag = typetag<std::string>())
		{
			size_t size = 0;
			const char* buffer = lua_tolstring(l, index, &size);
			return std::string(buffer, size);
		}
		inline std::string get(lua_State* l, int index, typetag<const std::string&> tag = typetag<const std::string&>())
		{
			return get(l, index, typetag<std::string>());
		}
		inline std::string get(lua_State* l, int index, typetag<const std::string> tag = typetag<const std::string>())
		{
			return get(l, index, typetag<std::string>());
		}



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
		inline int push(lua_State* l, NewTable)
		{
			lua_newtable(l);
			return 1;
		}

		inline int push(lua_State* l, NewThread)
		{
			lua_newthread(l);
			return 1;
		}
		

		inline int push(lua_State* l, GlobalTable)
		{
			lua_pushglobaltable(l);
			return 1;
		}


		template<typename T>
		inline int push(lua_State* l, const T& v)
		{
			void *storage = lua_newuserdata(l, sizeof(T));
			new(storage) T(v);
			luaL_setmetatable(l, types::metatableName<T>().c_str());
			return 1;
		}

#if KAGUYA_USE_RVALUE_REFERENCE
		inline int push(lua_State* l, std::string&& s)
		{
			lua_pushlstring(l, s.data(), s.size());
			return 1;
		}
#endif
		template<typename T>
		inline int push(lua_State* l, T& v)
		{
			if (!available_metatable(l, metatableName<T>().c_str()))
			{
				lua_pushlightuserdata(l, &v);
			}
			else
			{
				typedef MetaPointerWrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(&v);
				luaL_setmetatable(l, metatableName<wrapper_type>().c_str());
			}
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, T* v)
		{
			if (!available_metatable(l, metatableName<T>().c_str()))
			{
				lua_pushlightuserdata(l, v);
			}
			else
			{
				typedef MetaPointerWrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(v);
				luaL_setmetatable(l, metatableName<wrapper_type>().c_str());
			}
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, const T* v)
		{
			if (!available_metatable(l, metatableName<T>().c_str()))
			{
				lua_pushlightuserdata(l, v);
			}
			else
			{
				typedef MetaPointerWrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(v);
				luaL_setmetatable(l, metatableName<wrapper_type>().c_str());
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
