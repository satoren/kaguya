// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <algorithm>
#include <ostream>
#include "kaguya/config.hpp"
#include "kaguya/error_handler.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"


namespace kaguya
{
	class LuaRef;
	class LuaTable;
	class TableKeyReference;
	class mem_fun_binder;

	template<typename Derived>
	class LuaTableOrUserDataImpl
	{
	private:
		lua_State* state_()const
		{
			return static_cast<const Derived*>(this)->state();
		}
		int pushStackIndex_(lua_State* state)const
		{
			return static_cast<const Derived*>(this)->pushStackIndex(state);
		}
		int push_(lua_State* state)const
		{
			return static_cast<const Derived*>(this)->push(state);
		}

		template<typename K>
		struct gettablekey
		{
			typedef K key_type;
			typedef void value_type;
			std::vector<K>& v_;
			gettablekey(std::vector<K>&v) :v_(v) {}
			void operator ()(K key, const void*)
			{
				v_.push_back(key);
			}
		};
		template<typename V>
		struct gettablevalue
		{
			typedef void key_type;
			typedef V value_type;
			std::vector<V>& v_;
			gettablevalue(std::vector<V>&v) :v_(v) {}
			void operator ()(const void*, V value)
			{
				v_.push_back(value);
			}
		};
		template<typename K, typename V>
		struct gettablemap
		{
			typedef K key_type;
			typedef V value_type;
			std::map<K, V>& m_;
			gettablemap(std::map<K, V>& m) :m_(m) {}
			void operator ()(K key, V value)
			{
				m_[key] = value;
			}
		};


	public:

		bool setMetatable(const LuaTable& table);
		LuaTable getMetatable()const;


		/**
		* @brief table->*"function_name"() in c++ and table:function_name(); in lua is same
		* @param function_name function_name in table
		*/
		mem_fun_binder operator->*(const char* function_name);

		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T, typename KEY>
		typename lua_type_traits<T>::get_type getField(const KEY& key)const
		{
			lua_State* state = state_();
			typedef typename lua_type_traits<T>::get_type get_type;
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return lua_type_traits<T>::get_type();
			}
			util::ScopedSavedStack save(state);
			lua_type_traits<KEY>::push_(state, index);
			lua_gettable(state_(), pushStackIndex_(state));
			return lua_type_traits<T>::get(state, -1);
		}
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T>
		typename lua_type_traits<T>::get_type getField(const char* str)const
		{
			lua_State* state = state_();
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return typename lua_type_traits<T>::get_type();
			}
			util::ScopedSavedStack save(state);
			lua_getfield(state, pushStackIndex_(state), str);
			return lua_type_traits<T>::get(state, -1);
		}

		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T>
		typename lua_type_traits<T>::get_type getField(const std::string& str)const
		{
			return getField(str.c_str());
		}
		template<typename KEY>
		LuaRef getField(const KEY& key)const;

		/**
		* @brief foreach table fields
		*/
		template < class K, class V, class Fun> void foreach_table(Fun f)const
		{
			if (!state_())
			{
				except::typeMismatchError(state_(), "is nil");
				return;
			}
			util::ScopedSavedStack save(state_());
			int stackIndex = pushStackIndex_(state_());
			lua_pushnil(state_());
			while (lua_next(state_(), stackIndex) != 0)
			{
				typename lua_type_traits<V>::get_type value = lua_type_traits<V>::get(state_(), -1);
				typename lua_type_traits<K>::get_type key = lua_type_traits<K>::get(state_(), -2);
				f(key, value);
				lua_pop(state_(), 1);//pop value
			}
		}
		/**
		* @brief If type is table or userdata, return keys.
		* @return field keys
		*/
		template<typename K>
		std::vector<K> keys()const
		{
			std::vector<K> res;
			foreach_table<K, void>(gettablekey<K>(res));
			return res;
		}
		std::vector<LuaRef> keys()const;
		/**
		* @brief If type is table or userdata, return values.
		* @return field value
		*/
		template<typename V>
		std::vector<V> values()const
		{
			std::vector<V> res;
			foreach_table<void, V>(gettablevalue<V>(res));
			return res;
		}
		std::vector<LuaRef> values()const;
		/**
		* @brief If type is table or userdata, return key value pair.
		* @return key value pair
		*/
		template<typename K, typename V>
		std::map<K, V> map()const
		{
			std::map<K, V> res;
			foreach_table<K, V>(gettablemap<K, V>(res));
			return res;
		}
		std::map<LuaRef, LuaRef> map()const;
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename K>
		LuaRef operator[](const K& key)const;
		//@}
	};

	template<typename Derived>
	class LuaTableImpl
	{
	private:
		lua_State* state_()const
		{
			return static_cast<const Derived*>(this)->state();
		}
		int pushStackIndex_(lua_State* state)const
		{
			return static_cast<const Derived*>(this)->pushStackIndex(state);
		}
	public:

		/**
		* @brief value = table[key];or table[key] = value;
		* @param key key of table
		* @return reference of field value
		*/
		template<typename K>
		TableKeyReference operator[](const K& key);

		/**
		* @brief table[key] = value;
		*/
		template<typename K, typename V>
		void setField(const K& key, const V& value)
		{
			lua_State* state = state_();
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return;
			}
			util::ScopedSavedStack save(state);
			int stackIndex = pushStackIndex_(state);
			int kc = util::push_args(state, key);//push table key
			int vc = util::push_args(state, value);//push value

			if (!pushCountCheck<K>(state, kc) || !pushCountCheck<V>(state, vc)) { return; }
			lua_settable(state, stackIndex);//thistable[key] = value
		}

		template<typename V>
		void setField(const char* key, const V& value)
		{
			lua_State* state = state_();
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return;
			}
			util::ScopedSavedStack save(state);
			int stackIndex = pushStackIndex_(state);
			int vc = util::push_args(state, value);//push value

			if (!pushCountCheck<V>(state, vc)) { return; }
			lua_setfield(state, stackIndex, key);//thistable[key] = value
		}
		template<typename V>
		void setField(const std::string& key,const V& value)
		{
			setField(key.c_str(), value);
		}
#if KAGUYA_USE_CPP11
		/**
		* @brief table[key] = value;
		*/
		template<typename K, typename V>
		void setField(K&& key, V&& value)
		{
			lua_State* state = state_();
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return;
			}
			util::ScopedSavedStack save(state);
			int stackIndex = pushStackIndex_(state);
			int kc = util::push_args(state, std::forward<K>(key));//push table key
			int vc = util::push_args(state, std::forward<V>(value));//push value

			if (!pushCountCheck<K>(state_(), kc) || !pushCountCheck<V>(state, vc)) { return; }
			lua_settable(state, stackIndex);//thistable[key] = value
		}
		template<typename V>
		void setField(const char* key, V&& value)
		{
			lua_State* state = state_();
			if (!state)
			{
				except::typeMismatchError(state, "is nil");
				return;
			}
			util::ScopedSavedStack save(state);
			int stackIndex = pushStackIndex_(state);
			int vc = util::push_args(state, std::forward<V>(value));//push value

			if (!pushCountCheck<V>(state, vc)) { return; }
			lua_setfield(state, stackIndex, key);//thistable[key] = value
		}
		template<typename V>
		void setField(const std::string& key, V&& value)
		{
			setField(key.c_str(), std::forward<V>(value));
		}
#endif
	};

}