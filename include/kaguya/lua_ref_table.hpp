// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"

#include "kaguya/detail/lua_ref_impl.hpp"
#include "kaguya/detail/lua_table_def.hpp"

namespace kaguya
{
	class State;


	struct table_proxy
	{

#if KAGUYA_USE_CPP11
		template<typename V, typename KEY>
		static void set(lua_State* state, int table_index, const KEY& key, V&& value)
		{
			lua_type_traits<KEY>::push(state, key);
			lua_type_traits<typename traits::remove_reference<V>::type>::push(state, std::forward<V>(value));
			lua_settable(state, table_index);
		}
		template<typename V>
		static void set(lua_State* state, int table_index, const char* key, V&& value)
		{
			lua_type_traits<typename traits::remove_reference<V>::type>::push(state, std::forward<V>(value));
			lua_setfield(state, table_index, key);
		}
		template<typename V>
		static void set(lua_State* state, int table_index, const std::string& key, V&& value)
		{
			lua_type_traits<typename traits::remove_reference<V>::type>::push(state, std::forward<V>(value));
			lua_setfield(state, table_index, key.c_str());
		}

#endif
		template<typename V, typename KEY>
		static void set(lua_State* state, int table_index, const KEY& key, const V& value)
		{
			lua_type_traits<KEY>::push(state, key);
			lua_type_traits<V>::push(state, value);
			lua_settable(state, table_index);
		}
		template<typename V>
		static void set(lua_State* state, int table_index, const char* key, const V& value)
		{
			lua_type_traits<V>::push(state, value);
			lua_setfield(state, table_index, key);
		}
		template<typename V>
		static void set(lua_State* state, int table_index, const std::string& key, const V& value)
		{
			lua_type_traits<V>::push(state, value);
			lua_setfield(state, table_index, key.c_str());
		}

		template<typename KEY>
		static void get(lua_State* state, int table_index, const KEY& key)
		{
			lua_type_traits<KEY>::push(state, key);
			lua_gettable(state, table_index);
		}
		static void get(lua_State* state, int table_index, const char* key)
		{
			lua_getfield(state, table_index, key);
		}
		static void get(lua_State* state, int table_index, const std::string& key)
		{
			lua_getfield(state, table_index, key.c_str());
		}
	};


	/**
	* This class is the type returned by members of non-const LuaRef(Table) when directly accessing its elements.
	*/
	template<typename KEY>
	class TableKeyReference : public LuaVariantImpl<TableKeyReference<KEY> >
	{
	public:

		int pushStackIndex(lua_State* state)const
		{
			push(state);
			return lua_gettop(state);
		}
		lua_State* state()const { return state_; }

		friend class LuaRef;
		friend class State;
		
		//! this is not copy.same assign from referenced value.
		TableKeyReference& operator=(const TableKeyReference& src)
		{
			table_proxy::set(state_, table_index_, key_, src);
			return *this;
		}


		//! assign from T
		template<typename T>
		TableKeyReference& operator=(const T& src)
		{
			table_proxy::set(state_, table_index_, key_, src);

			return *this;
		}
#if KAGUYA_USE_CPP11
		template<typename T>
		TableKeyReference& operator=(T&& src)
		{
			table_proxy::set(state_, table_index_, key_, std::forward<T>(src));
			return *this;
		}
#endif

		bool isNilref()const {
			if (!state_)
			{
				return false;
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			return lua_isnoneornil(state_, -1);
		}

		//! register class metatable to lua and set to table
		template<typename T, typename P>
		void setClass(const ClassMetatable<T, P>& reg)
		{
			set_class(reg);
		}

		//! set function 
		template<typename T>
		void setFunction(T f)
		{
			table_proxy::set(state_, table_index_, key_, FunctorType(f));
		}

		//deprecated
		LuaRef getValue()const
		{
			util::ScopedSavedStack save(state_);
			push(state_);
			return lua_type_traits<LuaRef>::get(state_, -1);
		}


		int push(lua_State* state)const
		{
			if (lua_type(state_, table_index_) != LUA_TTABLE)
			{
				lua_pushnil(state);
				return 1;
			}

			table_proxy::get(state_, table_index_, key_);

			if (state_ != state)
			{
				lua_xmove(state_, state, 1);
			}
			return 1;

		}

		int push()const
		{
			return push(state_);
		}


		/**
		* @brief table->*"function_name"() in c++ and table:function_name(); in lua is same
		* @param function_name function_name in table
		*/
//		mem_fun_binder operator->*(const char* function_name)
//		{
//			return get<LuaRef>()->*(function_name);
//		}


		int type()const
		{
			util::ScopedSavedStack save(state_);
			push();
			return lua_type(state_, -1);
		}

		~TableKeyReference()
		{
			if (state_)
			{
				lua_settop(state_, stack_top_);
			}
		}

		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(const TableKeyReference& src) : state_(src.state_), stack_top_(src.stack_top_), table_index_(src.table_index_), key_(src.key_)
		{
			src.state_ = 0;
		}

		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(lua_State* state, int table_index, KEY key, int revstacktop) : state_(state), stack_top_(revstacktop), table_index_(table_index), key_(key)
		{
		}
	private:
		template<typename T, typename P>
		void set_class(const ClassMetatable<T, P>& reg)
		{
			LuaRef table(state_, NewTable());
			table.setMetatable(reg.registerClass(state_));
			*this = table;
		}


		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(lua_State* state, int table_index, const KEY& key, int revstacktop, const NoTypeCheck&) : state_(state), stack_top_(revstacktop), table_index_(table_index), key_(key)
		{
		}

		TableKeyReference(const LuaTable& table, const KEY& key) : state_(table.state()), stack_top_(lua_gettop(state_)), key_(key)
		{
			lua_type_traits<LuaRef>::push(state_, table);
			lua_type_traits<KEY>::push(state_, key);
			table_index_ = stack_top_ + 1;
		}
		TableKeyReference(const LuaRef& table, const KEY& key) : state_(table.state()), stack_top_(lua_gettop(state_)), key_(key)
		{
			lua_type_traits<LuaRef>::push(state_, table);
			lua_type_traits<KEY>::push(state_, key);
			table_index_ = stack_top_ + 1;
			if (lua_type(state_, table_index_) != LUA_TTABLE)
			{
				except::typeMismatchError(state_, "this is not table");
			}
		}

		mutable lua_State* state_;//mutable for RVO unsupported compiler
		int stack_top_;
		int table_index_;
		KEY key_;
	};

	template<typename KEY>
	inline std::ostream& operator<<(std::ostream& os, const TableKeyReference<KEY>& ref)
	{
		lua_State* state = ref.state();
		util::ScopedSavedStack save(state);
		int stackIndex = ref.pushStackIndex(state);
		util::stackValueDump(os, state, stackIndex);
		return os;
	}

	template<typename T>
	inline bool LuaFunctionImpl<T>::setFunctionEnv(const LuaTable& env)
	{
		util::ScopedSavedStack save(state_());
		int stackIndex = pushStackIndex_(state_());
		env.push();
#if LUA_VERSION_NUM >= 502
		lua_setupvalue(state_(), stackIndex, 1);
#else
		lua_setfenv(state_(), stackIndex);
#endif
		return true;
	}
	template<typename T>
	inline bool LuaFunctionImpl<T>::setFunctionEnv(NewTable env)
	{
		return setFunctionEnv(LuaTable(state_()));
	}

	template<typename T>
	inline LuaTable LuaFunctionImpl<T>::getFunctionEnv()
	{
		util::ScopedSavedStack save(state_());
		int stackIndex = pushStackIndex_(state_());

#if LUA_VERSION_NUM >= 502
		lua_getupvalue(state_(), stackIndex, 1);
#else
		lua_getfenv(state_(), stackIndex);
#endif
		return LuaTable(state_(), StackTop());
	}

	template<typename T>
	bool LuaTableOrUserDataImpl<T>::setMetatable(const LuaTable& table)
	{
		lua_State* state = state_();
		if (!state)
		{
			except::typeMismatchError(state, "is nil");
			return false;
		}
		util::ScopedSavedStack save(state);
		int stackindex = pushStackIndex_(state);
		int t = lua_type(state, stackindex);
		if (t != LUA_TTABLE && t != LUA_TUSERDATA)
		{
			except::typeMismatchError(state, lua_typename(state, t) + std::string("is not table"));
			return false;
		}
		table.push();
		return lua_setmetatable(state, stackindex) != 0;
	}
	template<typename T>
	LuaTable LuaTableOrUserDataImpl<T>::getMetatable()const
	{
		lua_State* state = state_();
		if (!state)
		{
			except::typeMismatchError(state, "is nil");
			return LuaRef(state);
		}
		util::ScopedSavedStack save(state);
		int stackindex = pushStackIndex_(state);
		int t = lua_type(state, stackindex);
		if (t != LUA_TTABLE && t != LUA_TUSERDATA)
		{
			except::typeMismatchError(state, lua_typename(state, t) + std::string("is not table"));
			return LuaRef(state);
		}
		lua_getmetatable(state, stackindex);
		return LuaTable(state, StackTop());
	}
	template<typename T>
	mem_fun_binder LuaTableOrUserDataImpl<T>::operator->*(const char* function_name)
	{
		push_(state_());
		return mem_fun_binder(LuaRef(state_(), StackTop()), function_name);
	}


	template<typename T> template <typename KEY>
	LuaStackRef LuaTableOrUserDataImpl<T>::getField(const KEY& key)const
	{
		lua_State* state = state_();
		if (!state)
		{
			except::typeMismatchError(state, "is nil");
			return LuaStackRef();
		}
		if (push_(state))//push table
		{
			lua_type_traits<KEY>::push(state, key);//push key
			lua_gettable(state, -2);//get table[key]
			lua_remove(state, -3);//remove table
			return LuaStackRef(state, -1,true);
		}
		return LuaStackRef();
	}

	template<typename T> template<typename KEY>
	LuaStackRef LuaTableOrUserDataImpl<T>::operator[](KEY key)const
	{
		return getField(key);
	}

	template<typename T>
	std::vector<LuaRef> LuaTableOrUserDataImpl<T>::values()const
	{
		return values<LuaRef>();
	}
	template<typename T>
	std::vector<LuaRef> LuaTableOrUserDataImpl<T>::keys()const
	{
		return keys<LuaRef>();
	}
	template<typename T>
	std::map<LuaRef, LuaRef> LuaTableOrUserDataImpl<T>::map()const
	{
		return map<LuaRef, LuaRef>();
	}

	template<typename T> template <typename K>
	TableKeyReference<K> LuaTableOrUserDataImpl<T>::operator[](K key)
	{
		lua_State* state = state_();
		int stack_top = lua_gettop(state);
		int stackindex = pushStackIndex_(state);
		return TableKeyReference<K>(state, stackindex, key, stack_top);
	}


	template<typename KEY>
	struct lua_type_traits<TableKeyReference<KEY> > {
		static int push(lua_State* l, const TableKeyReference<KEY>& ref)
		{
			ref.push(l);
			return 1;
		}
	};

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	template<typename T, typename A>
	struct lua_type_traits<std::vector<T, A> >
	{
		typedef std::vector<T, A> get_type;
		typedef const std::vector<T, A>& push_type;

		struct checkTypeForEach
		{
			checkTypeForEach(bool& valid) :valid_(valid) {}
			bool& valid_;
			bool operator()(const LuaStackRef& k, const LuaStackRef& v)
			{
				valid_ = k.typeTest<size_t>() && v.weakTypeTest<T>();
				return valid_;
			}
		};
		struct strictCheckTypeForEach
		{
			strictCheckTypeForEach(bool& valid) :valid_(valid) {}
			bool& valid_;
			bool operator()(const LuaStackRef& k, const LuaStackRef& v)
			{
				valid_ = k.typeTest<size_t>() && v.typeTest<T>();
				return valid_;
			}
		};

		static bool checkType(lua_State* l, int index)
		{
			LuaStackRef table(l, index);
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			
			bool valid = true;
			table.foreach_table_breakable<LuaStackRef, LuaStackRef>(checkTypeForEach(valid));
			return valid;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			LuaStackRef table(l, index);
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }

			bool valid = true;
			table.foreach_table_breakable<LuaStackRef, LuaStackRef>(strictCheckTypeForEach(valid));
			return valid;
		}

		static get_type get(lua_State* l, int index)
		{
			if (lua_type(l, index) != LUA_TTABLE)
			{
				except::typeMismatchError(l, std::string("type mismatch"));
				return get_type();
			}
			return LuaStackRef(l, index).values<T>();
		}
		static int push(lua_State* l, push_type v)
		{
			LuaRef table(l, NewTable(int(v.size()), 0));

			int count = 1;//array is 1 origin in Lua
			for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				table.setField(count++, *it);
			}
			table.push(l);
			return 1;
		}
	};
#endif

#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	template<typename K, typename V, typename C, typename A>
	struct lua_type_traits<std::map<K, V, C, A> >
	{
		typedef std::map<K, V, C, A> get_type;
		typedef const std::map<K, V, C, A>& push_type;

		struct checkTypeForEach
		{
			checkTypeForEach(bool& valid) :valid_(valid) {}
			bool& valid_;
			bool operator()(const LuaStackRef& k, const LuaStackRef& v)
			{
				valid_ = k.weakTypeTest<K>() && v.weakTypeTest<V>();
				return valid_;
			}
		};
		struct strictCheckTypeForEach
		{
			strictCheckTypeForEach(bool& valid) :valid_(valid) {}
			bool& valid_;
			bool operator()(const LuaStackRef& k, const LuaStackRef& v)
			{
				valid_ = k.typeTest<K>() && v.typeTest<V>();
				return valid_;
			}
		};
		static bool checkType(lua_State* l, int index)
		{
			LuaStackRef table(l, index);
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }

			bool valid = true;
			table.foreach_table_breakable<LuaStackRef, LuaStackRef>(checkTypeForEach(valid));
			return valid;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			LuaStackRef table(l, index);
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			
			bool valid = true;
			table.foreach_table_breakable<LuaStackRef, LuaStackRef>(strictCheckTypeForEach(valid));
			return valid;
		}

		static get_type get(lua_State* l, int index)
		{
			if (lua_type(l, index) != LUA_TTABLE)
			{
				except::typeMismatchError(l, std::string("type mismatch"));
				return get_type();
			}
			return LuaStackRef(l, index).map<K, V>();
		}
		static int push(lua_State* l, push_type v)
		{
			LuaRef table(l, NewTable(0, int(v.size())));
			for (typename std::map<K, V>::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				table.setField(it->first, it->second);
			}
			table.push(l);
			return 1;
		}
	};
#endif
}
