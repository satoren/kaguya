// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <cstring>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"
#include "kaguya/object.hpp"
#include "kaguya/exception.hpp"

namespace kaguya
{
	//default implements
	template<typename T, typename Enable>
	bool lua_type_traits<T, Enable>::checkType(lua_State* l, int index)
	{
		return object_wrapper(l, index, metatableName<T>()) != 0;
	}
	template<typename T, typename Enable>
	bool lua_type_traits<T, Enable>::strictCheckType(lua_State* l, int index)
	{
		return object_wrapper(l, index, metatableName<T>()) != 0;
	}
	template<typename T, typename Enable>
	typename lua_type_traits<T, Enable>::get_type lua_type_traits<T, Enable>::get(lua_State* l, int index)
	{
		const typename traits::remove_reference<T>::type* pointer = get_const_pointer(l, index, types::typetag<typename traits::remove_reference<T>::type>());
		if (!pointer)
		{
			throw LuaTypeMismatch("type mismatch!!");
		}
		return *pointer;
	}
	template<typename T, typename Enable>
	int lua_type_traits<T, Enable>::push(lua_State* l, push_type v)
	{
		typedef ObjectWrapper<typename traits::remove_const_and_reference<T>::type> wrapper_type;
		void *storage = lua_newuserdata(l, sizeof(wrapper_type));
		new(storage) wrapper_type(v);
		class_userdata::setmetatable<T>(l);
		return 1;
	}
	template<typename T, typename Enable>
	int lua_type_traits<T, Enable>::push(lua_State* l, NCRT& v)
	{
		typedef ObjectPointerWrapper<T> wrapper_type;
		void *storage = lua_newuserdata(l, sizeof(wrapper_type));
		new(storage) wrapper_type(&v);
		class_userdata::setmetatable<T>(l);
		return 1;
	}

#if KAGUYA_USE_RVALUE_REFERENCE

	template<typename T, typename Enable>
	int lua_type_traits<T, Enable>::push(lua_State* l, NCRT&& v)
	{
		typedef ObjectWrapper<typename traits::remove_const_and_reference<T>::type> wrapper_type;
		void *storage = lua_newuserdata(l, sizeof(wrapper_type));
		new(storage) wrapper_type(std::forward<NCRT>(v));
		class_userdata::setmetatable<T>(l);
		return 1;
	}
#endif

	template<typename T> struct lua_type_traits<T
		, typename traits::enable_if<traits::is_const_reference<T>::value>::type> :lua_type_traits<typename traits::remove_const_reference<T>::type > {};



	template<typename REF> struct lua_type_traits < REF
		, typename traits::enable_if<traits::is_lvalue_reference<REF>::value && !traits::is_const<typename traits::remove_reference<REF>::type>::value>::type >
	{
		typedef void Registerable;

		typedef REF get_type;
		typedef REF push_type;
		typedef typename traits::remove_reference<REF>::type T;

		static bool strictCheckType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<T>()) != 0;
		}
		static bool checkType(lua_State* l, int index)
		{
			if (lua_type(l, index) == LUA_TLIGHTUSERDATA)
			{
				return true;
			}
			return object_wrapper(l, index, metatableName<T>()) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			T* pointer =  get_pointer(l, index, types::typetag<T>());
			if (!pointer)
			{
				throw LuaTypeMismatch("type mismatch!!");
			}
			return *pointer;
		}
		static int push(lua_State* l, push_type v)
		{
			if (!available_metatable<T>(l))
			{
				lua_pushlightuserdata(l, const_cast<typename traits::remove_const<T>::type*>(&v));
			}
			else
			{
				typedef ObjectPointerWrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(&v);
				class_userdata::setmetatable<T>(l);
			}
			return 1;
		}
	};

	template<typename PTR> struct lua_type_traits < PTR
		, typename traits::enable_if<traits::is_pointer<typename traits::remove_const_reference<PTR>::type>::value && !traits::is_function<typename traits::remove_pointer<PTR>::type>::value>::type >
	{
		typedef void Registerable;

		typedef PTR get_type;
		typedef PTR push_type;
		typedef typename traits::remove_pointer<PTR>::type T;

		static bool strictCheckType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<T>()) != 0;
		}
		static bool checkType(lua_State* l, int index)
		{
			int type = lua_type(l, index);
			if (type == LUA_TLIGHTUSERDATA
				|| type == LUA_TNIL
				|| type == LUA_TNONE
				|| (type == LUA_TNUMBER && lua_tonumber(l, index) == 0)) //allow zero for nullptr;
			{
				return true;
			}
			return object_wrapper(l, index, metatableName<T>()) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			int type = lua_type(l, index);
			if (type == LUA_TUSERDATA || type == LUA_TLIGHTUSERDATA)
			{
				if (traits::is_const<T>::value)
				{
					return const_cast<get_type>(get_const_pointer(l, index, types::typetag<T>()));
				}
				else
				{
					return get_pointer(l, index, types::typetag<T>());
				}
			}

			if (type == LUA_TNIL
				|| type == LUA_TNONE
				|| (type == LUA_TNUMBER && lua_tonumber(l, index) == 0)) //allow zero for nullptr;
			{
				return 0;
			}
			throw LuaTypeMismatch("type mismatch!!");
			return 0;
		}
		static int push(lua_State* l, push_type v)
		{
			if (!v)
			{
				lua_pushnil(l);
			}
			else if (!available_metatable<T>(l))
			{
				lua_pushlightuserdata(l, const_cast<typename traits::remove_const<T>::type*>(v));
			}
			else
			{
				typedef ObjectPointerWrapper<T> wrapper_type;
				void *storage = lua_newuserdata(l, sizeof(wrapper_type));
				new(storage) wrapper_type(v);
				class_userdata::setmetatable<T>(l);
			}
			return 1;
		}
	};

	///! traits for bool
	template<>	struct lua_type_traits<bool> {
		typedef bool get_type;
		typedef bool push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TBOOLEAN;
		}
		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static bool get(lua_State* l, int index)
		{
			return lua_toboolean(l, index) != 0;
		}
		static int push(lua_State* l, bool s)
		{
			lua_pushboolean(l, s);
			return 1;
		}
	};

	template<>	struct lua_type_traits<void> {
		typedef void* get_type;
		typedef void* push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return true;
		}
		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static get_type get(lua_State* l, int index)
		{
			return 0;
		}
		static int push(lua_State* l, push_type s)
		{
			return 0;
		}
	};


	///! traits for reference_wrapper
	template<typename T> struct lua_type_traits<standard::reference_wrapper<T> > {
		typedef const standard::reference_wrapper<T>& push_type;

		static int push(lua_State* l, push_type v)
		{
			return lua_type_traits<T*>::push(l, &v.get());
		}
	};

	///! traits for shared_ptr
	template<typename T> struct lua_type_traits<standard::shared_ptr<T> > {
		typedef const standard::shared_ptr<T>& push_type;
		typedef standard::shared_ptr<T> get_type;

		static bool checkType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<get_type>()) != 0 ||
				lua_isnil(l, index);
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<get_type>()) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			if (lua_isnil(l, index)) {
				return get_type();
			}
			const get_type* pointer = get_const_pointer(l, index, types::typetag<get_type>());
			if (!pointer)
			{
				throw LuaTypeMismatch("type mismatch!!");
			}
			return *pointer;
		}

		static int push(lua_State* l, push_type v)
		{
			typedef ObjectSmartPointerWrapper<standard::shared_ptr<T> > wrapper_type;
			void *storage = lua_newuserdata(l, sizeof(wrapper_type));
			new(storage) wrapper_type(v);
			class_userdata::setmetatable<T>(l);
			return 1;
		}
	};

#if KAGUYA_USE_CPP11
	///! traits for unique_ptr
	template<typename T,typename Deleter> struct lua_type_traits<std::unique_ptr<T, Deleter> > {
		typedef std::unique_ptr<T, Deleter>&& push_type;
		typedef std::unique_ptr<T, Deleter> get_type;

		static bool checkType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<get_type>()) != 0 ||
				lua_isnil(l, index);
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return object_wrapper(l, index, metatableName<get_type>()) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			if (lua_isnoneornil(l, index)) {
				return 0;
			}
			const get_type* pointer = get_const_pointer(l, index, types::typetag<get_type>());
			if (!pointer)
			{
				throw LuaTypeMismatch("type mismatch!!");
			}
			return *pointer;
		}

		static int push(lua_State* l, push_type v)
		{
			typedef ObjectSmartPointerWrapper<std::unique_ptr<T,Deleter> > wrapper_type;
			void *storage = lua_newuserdata(l, sizeof(wrapper_type));
			new(storage) wrapper_type(std::forward<push_type>(v));
			class_userdata::setmetatable<T>(l);
			return 1;
		}
	};
	///! traits for nullptr
	template<>	struct lua_type_traits<std::nullptr_t> {
		typedef const std::nullptr_t& push_type;
		typedef std::nullptr_t get_type;

		static bool checkType(lua_State* l, int index)
		{
			return lua_isnoneornil(l, index);
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isnil(l, index);
		}
		static get_type get(lua_State* l, int index)
		{
			if (!lua_isnoneornil(l, index)) {
				throw LuaTypeMismatch("type mismatch!!");
			}
			return nullptr;
		}

		static int push(lua_State* l, const std::nullptr_t& v)
		{
			lua_pushnil(l);
			return 1;
		}
	};
#endif

	///! traits for 
	template<>	struct lua_type_traits<ObjectWrapperBase*> {
		typedef ObjectWrapperBase* get_type;
		typedef ObjectWrapperBase* push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return object_wrapper(l, index) != 0;
		}
		static bool checkType(lua_State* l, int index)
		{
			return object_wrapper(l, index) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			return object_wrapper(l, index);
		}
	};

	///! traits for native type of Luathread
	template<>	struct lua_type_traits<lua_State*> {
		typedef lua_State* get_type;
		typedef lua_State* push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isthread(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isthread(l, index);
		}
		static lua_State* get(lua_State* l, int index)
		{
			return lua_tothread(l, index);
		}
	};

	template<typename T> struct lua_type_traits < T
		, typename traits::enable_if<traits::is_floating_point<T>::value>::type >
	{
		typedef typename traits::remove_const_reference<T>::type get_type;
		typedef lua_Number push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TNUMBER;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isnumber(l, index) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			return static_cast<T>(lua_tonumber(l, index));
		}
		static int push(lua_State* l, lua_Number s)
		{
			lua_pushnumber(l, s);
			return 1;
		}
	};

	template<typename T> struct lua_type_traits<T
		, typename traits::enable_if<traits::is_integral<T>::value>::type>
	{
		typedef typename traits::remove_const_reference<T>::type get_type;
#if LUA_VERSION_NUM >= 503
		typedef lua_Integer push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isinteger(l, index) != 0;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isnumber(l, index) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			return static_cast<T>(lua_tointeger(l, index));
		}
		static int push(lua_State* l, lua_Integer s)
		{
			lua_pushinteger(l, s);
			return 1;
		}
#else
		typedef typename lua_type_traits<lua_Number>::push_type push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type_traits<lua_Number>::strictCheckType(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_type_traits<lua_Number>::checkType(l, index);
		}
		static get_type get(lua_State* l, int index)
		{
			return static_cast<get_type>(lua_type_traits<lua_Number>::get(l, index));
		}
		static int push(lua_State* l, push_type s)
		{
			return lua_type_traits<lua_Number>::push(l, s);
		}
#endif
	};

	template<typename T> struct lua_type_traits<T
		, typename traits::enable_if<traits::is_enum<T>::value>::type>
	{
		typedef typename traits::remove_const_reference<T>::type get_type;
		typedef typename traits::remove_const_reference<T>::type push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type_traits<luaInt>::strictCheckType(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_type_traits<luaInt>::checkType(l, index);
		}
		static get_type get(lua_State* l, int index)
		{
			return static_cast<get_type>(lua_type_traits<luaInt>::get(l, index));
		}
		static int push(lua_State* l, push_type s)
		{
			return lua_type_traits<luaInt>::push(l, static_cast<typename lua_type_traits<int64_t>::push_type>(s));
		}
	};


	template<>	struct lua_type_traits<const char*> {
		typedef std::string get_type;
		typedef const char* push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isstring(l, index) != 0;
		}
		static const char* get(lua_State* l, int index)
		{
			return lua_tostring(l, index);
		}
		static int push(lua_State* l, const char* s)
		{
			lua_pushstring(l, s);
			return 1;
		}
	};

	template<int N>	struct lua_type_traits<char[N]> {
		typedef std::string get_type;
		typedef const char* push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isstring(l, index) != 0;
		}
		static const char* get(lua_State* l, int index)
		{
			return lua_tostring(l, index);
		}
		static int push(lua_State* l, const char s[N])
		{
			lua_pushstring(l, s);
			return 1;
		}
	};
	template<int N>	struct lua_type_traits<const char[N]> :lua_type_traits<const char*> {};

	template<>	struct lua_type_traits<std::string> {
		typedef std::string get_type;
		typedef const std::string& push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TSTRING;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isstring(l, index) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			size_t size = 0;
			const char* buffer = lua_tolstring(l, index, &size);
			return std::string(buffer, size);
		}
		static int push(lua_State* l, const std::string& s)
		{
			lua_pushlstring(l, s.c_str(), s.size());
			return 1;
		}
	};

#include "kaguya/gen/push_tuple.inl"

	struct NewTable {
		NewTable() :reserve_array_(0), reserve_record_(0) {}
		NewTable(int reserve_array, int reserve_record_) :reserve_array_(reserve_array), reserve_record_(reserve_record_) {}
		int reserve_array_;
		int reserve_record_;
	};
	struct NewThread {};
	struct GlobalTable {};
	struct NilValue {};

	struct NoTypeCheck {};

	template<>	struct lua_type_traits<NewTable> {
		static int push(lua_State* l, const NewTable& table)
		{
			lua_createtable(l, table.reserve_array_, table.reserve_record_);
			return 1;
		}
	};
	template<>	struct lua_type_traits<NewThread> {
		static int push(lua_State* l, const NewThread&)
		{
			lua_newthread(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<NilValue> {
		static int push(lua_State* l, const NilValue&)
		{
			lua_pushnil(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<GlobalTable> {
		static int push(lua_State* l, const GlobalTable&)
		{
#if LUA_VERSION_NUM >= 502
			lua_pushglobaltable(l);
#else
			lua_pushvalue(l, LUA_GLOBALSINDEX);
#endif
			return 1;
		}
	};



	template<typename Derived>
	class LuaBasicTypeFunctions
	{
	public:
		enum value_type
		{
			TYPE_NIL = LUA_TNIL,//!< nil type
			TYPE_BOOL = LUA_TBOOLEAN,//!< boolean type
			TYPE_LIGHTUSERDATA = LUA_TLIGHTUSERDATA,//!< light userdata type
			TYPE_NUMBER = LUA_TNUMBER,//!< number type
			TYPE_STRING = LUA_TSTRING,//!< string type
			TYPE_TABLE = LUA_TTABLE,//!< table type
			TYPE_FUNCTION = LUA_TFUNCTION,//!< function type
			TYPE_USERDATA = LUA_TUSERDATA,//!< userdata type
			TYPE_THREAD = LUA_TTHREAD,//!< thread(coroutine) type
		};

		bool isNilref_()const { return state_() == 0 || type() == LUA_REFNIL; }
		
		
		/**
		* @brief Equivalent to `#` operator for strings and tables with no metamethods.
		* Follows Lua's reference manual documentation of `lua_rawlen`, ie. types other
		* than tables, strings or userdatas return 0.
		* @return Size of table, string length or userdata memory block size.
		*/
		size_t size() const {
			lua_State* state = state_();
			util::ScopedSavedStack save(state);
			int index = pushStackIndex_(state);
#if LUA_VERSION_NUM >= 502
			return lua_rawlen(state, index);
#else

			int type = lua_type(state,index);
			if(type != TYPE_STRING && type != TYPE_TABLE && type != TYPE_USERDATA && type != TYPE_LIGHTUSERDATA)
			{
				return 0;
			}
			return lua_objlen(state, index);
#endif
		}

		//return type
		int type() const
		{
			lua_State* state = state_();
			if (!state)
			{
				return LUA_TNONE;
			}
			util::ScopedSavedStack save(state);
			return lua_type(state, pushStackIndex_(state));
		}

		//return type name
		const char* typeName()const
		{
			return lua_typename(state_(), type());
		}

	private:
		lua_State* state_()const
		{
			return static_cast<const Derived*>(this)->state();
		}
		int pushStackIndex_(lua_State* state)const
		{
			return static_cast<const Derived*>(this)->pushStackIndex(state);
		}
	};
};
