// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include "kaguya/config.hpp"
#include "kaguya/error_handler.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"


namespace kaguya
{
	class LuaRef;
	class LuaStackRef;
	class LuaTable;
	template<typename KEY>
	class TableKeyReference;
	class MemberFunctionBinder;

	namespace detail
	{
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

			template<typename K, typename A>
			struct gettablekey
			{
				typedef K key_type;
				typedef void value_type;
				std::vector<K, A>& v_;
				gettablekey(std::vector<K, A>&v) :v_(v) {}
				void operator ()(K key, const void*)
				{
					v_.push_back(key);
				}
			};
			template<typename V, typename A>
			struct gettablevalue
			{
				typedef void key_type;
				typedef V value_type;
				std::vector<V, A>& v_;
				gettablevalue(std::vector<V, A>&v) :v_(v) {}
				void operator ()(const void*, V value)
				{
					v_.push_back(value);
				}
			};
			template<typename K, typename V, typename C, typename A>
			struct gettablemap
			{
				typedef K key_type;
				typedef V value_type;
				std::map<K, V, C, A>& m_;
				gettablemap(std::map<K, V, C, A>& m) :m_(m) {}
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
			MemberFunctionBinder operator->*(const char* function_name);

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
					return get_type();
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, key);
				lua_gettable(state, stackIndex);
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
			LuaStackRef getField(const KEY& key)const;


			/**
			* @brief value = rawget(table,key);
			* @param key key of table
			* @return reference of field value
			*/
			template<typename T, typename KEY>
			typename lua_type_traits<T>::get_type getRawField(const KEY& key)const
			{
				lua_State* state = state_();
				typedef typename lua_type_traits<T>::get_type get_type;
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return get_type();
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, key);
				lua_rawget(state, stackIndex);
				return lua_type_traits<T>::get(state, -1);
			}
			template<typename KEY>
			LuaStackRef getRawField(const KEY& key)const;
			/**
			* @brief value = rawget(table,key);
			* @param key key of table
			* @return reference of field value
			*/
			template<typename T>
			typename lua_type_traits<T>::get_type getRawField(luaInt key)const
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return typename lua_type_traits<T>::get_type();
				}
				util::ScopedSavedStack save(state);
				lua_rawgeti(state, pushStackIndex_(state), key);
				return lua_type_traits<T>::get(state, -1);
			}

			/**
			* @brief foreach table fields
			*/
			template < class K, class V, class Fun> void foreach_table(Fun f)const
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				lua_pushnil(state);
				while (lua_next(state, stackIndex) != 0)
				{
					lua_pushvalue(state, -2);//backup key
					lua_insert(state, -3);

					typename lua_type_traits<V>::get_type value = lua_type_traits<V>::get(state, -1);
					typename lua_type_traits<K>::get_type key = lua_type_traits<K>::get(state, -2);
					f(key, value);
					lua_pop(state, 2);//pop key and value
				}
			}


			/**
			* @brief foreach table fields
			*/
			template < class K, class V, class Fun> void foreach_table_breakable(Fun f)const
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				lua_pushnil(state);
				while (lua_next(state, stackIndex) != 0)
				{
					lua_pushvalue(state, -2);//backup key
					lua_insert(state, -3);

					typename lua_type_traits<V>::get_type value = lua_type_traits<V>::get(state, -1);
					typename lua_type_traits<K>::get_type key = lua_type_traits<K>::get(state, -2);
					bool cont = f(key, value);
					lua_pop(state, 2);//pop key and value
					if (!cont)
					{
						break;
					}
				}
			}

			/**
			* @brief If type is table or userdata, return keys.
			* @return field keys
			*/
			template<typename K, typename A>
			std::vector<K, A> keys()const
			{
				std::vector<K, A> res;
				util::ScopedSavedStack save(state_());
				int stackIndex = pushStackIndex_(state_());
				size_t size = lua_rawlen(state_(), stackIndex);
				res.reserve(size);
				foreach_table<K, void>(gettablekey<K, A>(res));
				return res;
			}
			template<typename K >
			std::vector<K> keys()const
			{
				return keys<K, std::allocator<K> >();
			}
			std::vector<LuaRef> keys()const;
			/**
			* @brief If type is table or userdata, return values.
			* @return field value
			*/
			template<typename V, typename A>
			std::vector<V, A> values()const
			{
				std::vector<V, A> res;
				util::ScopedSavedStack save(state_());
				int stackIndex = pushStackIndex_(state_());
				size_t size = lua_rawlen(state_(), stackIndex);
				res.reserve(size);
				foreach_table<void, V>(gettablevalue<V, A>(res));
				return res;
			}
			template<typename V >
			std::vector<V> values()const
			{
				return values<V, std::allocator<V> >();
			}
			std::vector<LuaRef> values()const;
			/**
			* @brief If type is table or userdata, return key value pair.
			* @return key value pair
			*/
			template<typename K, typename V, typename C, typename A >
			std::map<K, V, C, A> map()const
			{
				std::map<K, V, C, A> res;
				foreach_table<K, V>(gettablemap<K, V, C, A>(res));
				return res;
			}
			template<typename K, typename V, typename C >
			std::map<K, V, C> map()const
			{
				return map < K, V, C, std::allocator<std::pair<const K, V> > >();
			}
			template<typename K, typename V>
			std::map<K, V> map()const
			{
				return map<K, V, std::less<K> >();
			}
			std::map<LuaRef, LuaRef> map()const;
			/**
			* @brief value = table[key];
			* @param key key of table
			* @return reference of field value
			*/
			template<typename K>
			LuaStackRef operator[](K key)const;



			/**
			* @brief value = table[key];or table[key] = value;
			* @param key key of table
			* @return reference of field value
			*/
			template<typename K>
			TableKeyReference<K> operator[](K key);
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
			* @brief table[key] = value;
			*/
			template<typename K, typename V>
			bool setField(const K& key, const V& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, key);//push table key
				util::one_push(state, value);//push value
				lua_settable(state, stackIndex);//thistable[key] = value
				return true;
			}

			template<typename V>
			bool setField(const char* key, const V& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, value);//push value
				lua_setfield(state, stackIndex, key);//thistable[key] = value
				return true;
			}
			template<typename V>
			bool setField(const std::string& key, const V& value)
			{
				return setField(key.c_str(), value);
			}
#if KAGUYA_USE_CPP11
			/**
			* @brief table[key] = value;
			*/
			template<typename K, typename V>
			bool setField(K&& key, V&& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, std::forward<K>(key));//push table key
				util::one_push(state, std::forward<V>(value));//push value
				lua_settable(state, stackIndex);//thistable[key] = value
				return true;
			}
			template<typename V>
			bool setField(const char* key, V&& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, std::forward<V>(value));//push value
				lua_setfield(state, stackIndex, key);//thistable[key] = value
				return true;
			}
			template<typename K, typename V>
			bool setRawField(K&& key, V&& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, key);//push table key
				util::one_push(state, value);//push value
				lua_rawset(state, stackIndex);//thistable[key] = value
				return true;
			}
#endif

			template<typename K, typename V>
			bool setRawField(const K& key, const V& value)
			{
				lua_State* state = state_();
				if (!state)
				{
					except::typeMismatchError(state, "is nil");
					return false;
				}
				util::ScopedSavedStack save(state);
				int stackIndex = pushStackIndex_(state);
				util::one_push(state, key);//push table key
				util::one_push(state, value);//push value
				lua_rawset(state, stackIndex);//thistable[key] = value
				return true;
			}
		};
	}
}