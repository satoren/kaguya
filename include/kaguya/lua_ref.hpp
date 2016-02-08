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
	struct StackTop {};

	class LuaUserData;
	class LuaTable;
	class LuaFunction;
	class LuaThread;
	class TableKeyReference;
	class FunctionResults;
	class mem_fun_binder;

	class LuaRef;

	template<>
	struct lua_type_traits<LuaRef>
	{
		typedef LuaRef get_type;
		typedef const LuaRef& push_type;

		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return false;
		}

		static get_type get(lua_State* l, int index);
		static int push(lua_State* l, push_type v);
	};
	template<>	struct lua_type_traits<const LuaRef&> :lua_type_traits<LuaRef> {};

	namespace util
	{
		template<class Result>
		inline Result get_result(lua_State* l,int startindex);
	}
	/**
	* Reference of Lua any type value.
	*/
	class LuaRef
	{
		friend class LuaUserData;
		friend class LuaTable;
		friend class LuaFunction;
		friend class LuaThread;
	private:
		lua_State *state_;
		int ref_;

		typedef void (LuaRef::*bool_type)() const;
		void this_type_does_not_support_comparisons() const {}

		void unref()
		{
			if (!isNilref())
			{
				luaL_unref(state_, LUA_REGISTRYINDEX, ref_);
				state_ = 0;
				ref_ = LUA_REFNIL;
			}
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


		template<typename K, typename V>
		void setFieldImpl(K key, V value)
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return;
			}
			util::ScopedSavedStack save(state_);
			push(state_);//push table to stack
			int t = lua_type(state_, -1);
			if (t != LUA_TTABLE)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return;
			}
			int kc = lua_type_traits<K>::push(state_, standard::forward<K>(key));//push table key
			int vc = lua_type_traits<V>::push(state_, standard::forward<V>(value));//push value

			if (!pushCountCheck<K>(kc) || !pushCountCheck<V>(vc)) { return; }
			lua_settable(state_, -3);//thistable[key] = value
		}

		static lua_State* toMainThread(lua_State* state)
		{
#if LUA_VERSION_NUM >= 502
			if (state)
			{
				lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
				lua_State* mainthread = lua_tothread(state, -1);
				lua_pop(state, 1);
				if (mainthread)
				{
					return mainthread;
				}
			}
#endif
			return state;
		}

		template<typename T>
		bool pushCountCheck(int count)
		{
			if (count != 1)
			{
				if (count > 1) { except::typeMismatchError(state_, std::string("can not push multiple value:") + typeid(T).name()); }
				if (count == 0) { except::typeMismatchError(state_, std::string("can not push ") + typeid(T).name() + " value"); }
				return false;
			}
			return true;
		}
		int lua_resume_compat(lua_State *L, int nargs)
		{
			if (nargs < 0) { nargs = 0; }
#if LUA_VERSION_NUM >= 502
			return lua_resume(L, 0, nargs);
#else
			return lua_resume(L, nargs);
#endif
		}

	public:
		lua_State *state()const { return state_; };

		struct NoMainCheck {};
		bool isNilref()const { return state_ == 0 || ref_ == LUA_REFNIL; }

		//! value type of Lua Reference
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

		LuaRef(const LuaRef& src) :state_(src.state_)
		{
			if (!src.isNilref())
			{
				src.push(state_);
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			else
			{
				ref_ = LUA_REFNIL;
			}
		}
		LuaRef& operator =(const LuaRef& src)
		{
			unref();
			state_ = src.state_;
			if (!src.isNilref())
			{
				src.push(state_);
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			else
			{
				ref_ = LUA_REFNIL;
			}
			return *this;
		}
#if KAGUYA_USE_RVALUE_REFERENCE
		LuaRef(LuaRef&& src)throw() :state_(0), ref_(LUA_REFNIL)
		{
			swap(src);
		}
		LuaRef& operator =(LuaRef&& src)throw()
		{
			swap(src);
			return *this;
		}
#endif

		LuaRef() :state_(0), ref_(LUA_REFNIL) {}
		LuaRef(lua_State* state) :state_(state), ref_(LUA_REFNIL) {}


		LuaRef(lua_State* state, StackTop, NoMainCheck) :state_(state), ref_(LUA_REFNIL)
		{
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}

		LuaRef(lua_State* state, StackTop) :state_(state), ref_(LUA_REFNIL)
		{
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			state_ = toMainThread(state_);
		}

		void swap(LuaRef& other)throw()
		{
			std::swap(state_, other.state_);
			std::swap(ref_, other.ref_);
		}

		template<typename T>
		LuaRef(lua_State* state, T v, NoMainCheck) : state_(state)
		{
			util::ScopedSavedStack save(state_);
			int vc = lua_type_traits<T>::push(state_, standard::forward<T>(v));
			if (!pushCountCheck<T>(vc)) { return; }
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}
		template<typename T>
		LuaRef(lua_State* state, T v) : state_(state)
		{
			util::ScopedSavedStack save(state_);
			int vc = lua_type_traits<T>::push(state_, standard::forward<T>(v));
			if (!pushCountCheck<T>(vc)) { return; }
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			state_ = toMainThread(state_);
		}
		~LuaRef()
		{
			unref();
		}

		//push to Lua stack
		int push()const
		{
			return push(state_);
		}
		int push(lua_State* state)const
		{
			if (isNilref())
			{
				lua_pushnil(state);
				return 1;
			}
#if LUA_VERSION_NUM >= 502
			if (state != state_)
			{//state check
				assert(toMainThread(state) == toMainThread(state_));
			}
#endif
			lua_rawgeti(state, LUA_REGISTRYINDEX, ref_);
			return 1;
		}

		template<typename T>
		bool typeTest()const
		{
			util::ScopedSavedStack save(state_);
			push();//push to stack
			return lua_type_traits<T>::strictCheckType(state_, -1);
		}
		template<typename T>
		bool weakTypeTest()const
		{
			util::ScopedSavedStack save(state_);
			push();//push to stack
			return lua_type_traits<T>::checkType(state_, -1);
		}

		template<typename T>
		typename lua_type_traits<T>::get_type get()const
		{
			typedef typename lua_type_traits<T>::get_type get_type;
			if (!state_)
			{
				throw LuaTypeMismatch(std::string("no reference "));
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			if (!lua_type_traits<get_type>::checkType(state_, -1))
			{
				throw LuaTypeMismatch(typeName() + std::string("is not ") + typeid(T).name());
			}
			return lua_type_traits<get_type>::get(state_, -1);
		}

		template<typename T>
		operator T()const {
			return get<T>();
		}



		operator bool_type() const
		{
			return (!isNilref() && get<bool>() == true) ? &LuaRef::this_type_does_not_support_comparisons : 0;
		}

		/**
		* @name function type
		*/
		//@{

		/**
		* set function environment table
		*/
		bool setFunctionEnv(const LuaTable& env);
		/**
		* set function environment to new table
		*/
		bool setFunctionEnv(NewTable env);
		/**
		* get function environment table
		*/
		LuaTable getFunctionEnv();

		//@}


#if KAGUYA_USE_CPP11
		template<class Result, class...Args> Result call(Args... args)
		{
			int argstart = lua_gettop(state_) + 1;
			util::ScopedSavedStack save(state_, argstart - 1);
			push(state_);
			util::push_args(state_, standard::forward<Args>(args)...);
			int argnum = lua_gettop(state_) - argstart;
			int result = lua_pcall(state_, argnum, LUA_MULTRET, 0);
			except::checkErrorAndThrow(result, state_);
			return util::get_result<Result>(state_, argstart);
		}
		template<class Result, class...Args> Result resume(Args... args)
		{
			lua_State* cor = get<lua_State*>();
			int argstart = lua_gettop(cor) + 1;
			if (argstart > 1 && lua_status(cor) != LUA_YIELD)
			{
				argstart -= 1;
			}
			util::ScopedSavedStack save(cor, argstart-1);
			util::push_args(cor, standard::forward<Args>(args)...);
			int argnum = lua_gettop(cor) - argstart;
			int result = lua_resume_compat(cor, argnum);
			except::checkErrorAndThrow(result, cor);
			return util::get_result<Result>(cor, argstart);
		}

		template<class...Args> FunctionResults operator()(Args... args);
#else

#define KAGUYA_PP_TEMPLATE(N) ,KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) KAGUYA_PP_CAT(A,N) KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_ARG_DEF(N) ,standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N))
#define KAGUYA_CALL_DEF(N) \
		template<class Result KAGUYA_PP_REPEAT(N,KAGUYA_PP_TEMPLATE)>\
		Result call(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			int argstart = lua_gettop(state_) + 1;\
			util::ScopedSavedStack save(state_, argstart - 1);\
			push(state_);\
			util::push_args(state_ KAGUYA_PP_REPEAT(N,KAGUYA_PUSH_ARG_DEF));\
			int argnum = lua_gettop(state_) - argstart;\
			int result = lua_pcall(state_, argnum, LUA_MULTRET, 0);\
			except::checkErrorAndThrow(result, state_);\
			return util::get_result<Result>(state_, argstart);\
		}


		KAGUYA_CALL_DEF(0)
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_DEF)

#define KAGUYA_RESUME_DEF(N) \
		template<class Result  KAGUYA_PP_REPEAT(N,KAGUYA_PP_TEMPLATE)>\
		Result resume(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			lua_State* cor = get<lua_State*>();\
			int argstart = lua_gettop(cor) + 1;\
			if (argstart > 1 && lua_status(cor) != LUA_YIELD)\
			{\
				argstart -= 1;\
			}\
			util::ScopedSavedStack save(cor, argstart - 1);\
			util::push_args(cor KAGUYA_PP_REPEAT(N, KAGUYA_PUSH_ARG_DEF));\
			int argnum = lua_gettop(cor) - argstart;\
			int result = lua_resume_compat(cor, argnum);\
			except::checkErrorAndThrow(result, cor);\
			return util::get_result<Result>(cor, argstart);\
		}

			KAGUYA_RESUME_DEF(0)
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_RESUME_DEF)

#undef KAGUYA_PUSH_DEF
#undef KAGUYA_PUSH_ARG_DEF
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_RESUME_DEF

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_OP_FN_DEF(N) \
		template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)> \
		FunctionResults operator()(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG));

			/**
			* @name operator()
			* @brief If type is function, call lua function.
			If type is lua thread,start or resume lua thread.
			Otherwise send error message to error handler
			* @param arg... function args
			*/
			//@{
			FunctionResults operator()();
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
			//@}
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_CALL_DEF
#undef KAGUYA_OP_FN_DEF

#endif
		/**
		* @name coroutine type
		*/
		//@{
		/**
		* @return state status
		*/
			int threadStatus()const
		{
			if (isNilref())
			{
				except::typeMismatchError(state_, "is nil");
				return LUA_ERRRUN;
			}
			util::ScopedSavedStack save(state_);
			lua_State* thread = get<lua_State*>();

			if (!thread)
			{
				except::typeMismatchError(state_, "is not thread");
				return LUA_ERRRUN;
			}
			return lua_status(thread);
		}

		//! deprecate
		int thread_status()const
		{
			return threadStatus();
		}

		/**
		* status of coroutine
		*/
		enum coroutine_status
		{
			COSTAT_RUNNING,//!< coroutine is running
			COSTAT_SUSPENDED,//!< coroutine is suspended
			COSTAT_NORMAL,//!<
			COSTAT_DEAD,//!< coroutine is dead
		};

		/**
		* @return coroutine status
		*/
		coroutine_status costatus(lua_State *l = 0)const
		{
			if (isNilref())
			{
				except::typeMismatchError(state_, "is nil");
				return COSTAT_DEAD;
			}

			lua_State* thread = get<lua_State*>();
			if (!thread)
			{
				except::typeMismatchError(state_, "is not thread");
				return COSTAT_DEAD;
			}
			else if (thread == l)
			{
				return COSTAT_RUNNING;
			}
			else
			{
				switch (lua_status(thread))
				{
				case LUA_YIELD:
					return COSTAT_SUSPENDED;
				case 0://LUA_OK
				{
					if (lua_gettop(thread) == 0)
					{
						return COSTAT_DEAD;
					}
					else
					{
						return COSTAT_SUSPENDED;
					}
				}
				default:
					break;
				}
			}
			return COSTAT_DEAD;

		}

		/**
		* @return if coroutine status is dead, return true. Otherwise return false
		*/
		bool isThreadDead()const
		{
			return costatus() == COSTAT_DEAD;
		}
		//@}


		/**
		* @name table type
		*/
		//@{

		bool setMetatable(const LuaTable& table);

		LuaTable getMetatable()const;

		/**
		* @brief table->*"function_name"() in c++ and table:function_name(); in lua is same
		* @param function_name function_name in table
		*/
		mem_fun_binder operator->*(const char* function_name);


		/**
		* @brief value = table[key];or table[key] = value;
		* @param key key of table
		* @return reference of field value
		*/
		TableKeyReference operator[](const LuaRef& key);
		/**
		* @brief value = table[key];or table[key] = value;
		* @param key key of table
		* @return reference of field value
		*/
		TableKeyReference operator[](const char* key);
		/**
		* @brief value = table[key];or table[key] = value;
		* @param key key of table
		* @return reference of field value
		*/
		TableKeyReference operator[](const std::string& key);
		/**
		* @brief value = table[index];or table[index] = value;
		* @param index index of table
		* @return reference of field value
		*/
		TableKeyReference operator[](int index);

		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		LuaRef operator[](const LuaRef& key)const
		{
			return getField(key);
		}
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		LuaRef operator[](const char* key)const
		{
			return getField(key);
		}
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		LuaRef operator[](const std::string& key)const
		{
			return getField(key);
		}
		/**
		* @brief value = table[key];
		* @param index index of table
		* @return reference of field value
		*/
		LuaRef operator[](int index)const
		{
			return getField(index);
		}

		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T>
		typename lua_type_traits<T>::get_type getField(const LuaRef& key)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			key.push(state_);
			lua_gettable(state_, -2);
			return lua_type_traits<T>::get(state_, -1);
		}
		LuaRef getField(const LuaRef& key)const {
			return getField<LuaRef>(key);
		}
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T>
		typename lua_type_traits<T>::get_type getField(const char* str)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			lua_pushstring(state_, str);
			lua_gettable(state_, -2);
			return lua_type_traits<T>::get(state_, -1);
		}
		LuaRef getField(const char* str)const
		{
			return getField<LuaRef>(str);
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
		LuaRef getField(const std::string& str)const
		{
			return getField<LuaRef>(str);
		}
		/**
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		template<typename T>
		typename lua_type_traits<T>::get_type getField(int index)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push();
			int t = lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			lua_type_traits<int>::push(state_, index);
			lua_gettable(state_, -2);
			return lua_type_traits<T>::get(state_, -1);
		}
		LuaRef getField(int index)const
		{
			return getField<LuaRef>(index);
		}
		/**
		* @brief table[key] = value;
		*/
		template<typename T>
		void setField(int key, T value)
		{
			setFieldImpl<int, T>(key, standard::forward<T>(value));
		}

		/**
		* @brief table[key] = value;
		*/
		template<typename T>
		void setField(const char* key, T value)
		{
			setFieldImpl<const char*, T>(key, standard::forward<T>(value));
		}
		/**
		* @brief table[key] = value;
		*/
		template<typename T>
		void setField(const std::string& key, T value)
		{
			setField(key.c_str(), standard::forward<T>(value));
		}

		/**
		* @brief table[key] = value;
		*/
		template<typename T>
		void setField(const LuaRef& key, T value)
		{
			setFieldImpl<LuaRef, T>(key, standard::forward<T>(value));
		}

		/**
		* @brief foreach table fields
		*/
		template < class K, class V, class Fun> void foreach_table(Fun f)const
		{
			if (ref_ == LUA_REFNIL)
			{
				return;
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				return;
			}
			int top = lua_gettop(state_);
			lua_pushnil(state_);
			while (lua_next(state_, top) != 0)
			{
				typename lua_type_traits<V>::get_type value = lua_type_traits<V>::get(state_, -1);
				typename lua_type_traits<K>::get_type key = lua_type_traits<K>::get(state_, -2);
				f(key, value);
				lua_pop(state_, 1);//pop value
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
		std::vector<LuaRef> keys()const { return keys<LuaRef>(); }
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
		std::vector<LuaRef> values()const { return values<LuaRef>(); }
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
		std::map<LuaRef, LuaRef> map()const { return map<LuaRef, LuaRef>(); }
		//@}

		enum value_type type() const
		{
			if (ref_ == LUA_REFNIL)
			{
				return TYPE_NIL;
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			return (value_type)lua_type(state_, -1);
		}
		std::string typeName()const
		{
			return lua_typename(state_, type());
		}
		/**
		* @name relational operators
		* @brief
		*/
		//@{
		bool operator==(const LuaRef& other)const
		{
			value_type other_type = other.type();
			value_type this_type = type();
			if (other_type != this_type) { return false; }
			if (other_type == TYPE_NIL) { return true; }
			util::ScopedSavedStack save(state_);
			other.push(state_);
			push();
#if LUA_VERSION_NUM >= 502
			return lua_compare(state_, -1, -2, LUA_OPEQ) != 0;
#else
			return lua_equal(state_, -1, -2) != 0;
#endif
		}
		bool operator<(const LuaRef& other)const
		{
			value_type other_type = other.type();
			value_type this_type = type();
			if (other_type != this_type) { return this_type < other_type; }
			if (other_type == TYPE_NIL) { return false; }
			util::ScopedSavedStack save(state_);
			other.push(state_);
			push(state_);
#if LUA_VERSION_NUM >= 502
			return lua_compare(state_, -1, -2, LUA_OPLT) != 0;
#else
			return lua_lessthan(state_, -1, -2) != 0;
#endif
		}
		bool operator<=(const LuaRef& other)const
		{
			value_type other_type = other.type();
			value_type this_type = type();
			if (other_type != this_type) { return this_type < other_type; }
			if (other_type == TYPE_NIL) { return true; }
			util::ScopedSavedStack save(state_);
			other.push(state_);
			push();
#if LUA_VERSION_NUM >= 502
			return lua_compare(state_, -1, -2, LUA_OPLE) != 0;
#else
			return lua_equal(state_, -1, -2) != 0 || lua_lessthan(state_, -1, -2) != 0;
#endif
		}
		bool operator>=(const LuaRef& other)const
		{
			return other <= *this;
		}
		bool operator>(const LuaRef& other)const
		{
			return other < *this;
		}
		bool operator!=(const LuaRef& other)const
		{
			return !(other == *this);
		}

		const void* native_pointer()const
		{
			util::ScopedSavedStack save(state_);
			push(state_);
			return lua_topointer(state_, -1);
		}
		//@}
		static void putindent(std::ostream& os, int indent)
		{
			while (indent-- > 0)
			{
				os << "  ";
			}
		}

		void dump(std::ostream& os)const
		{
			std::set<const void*> table;
			dump_impl(os,0, table);
		}
		private:
		void dump_impl(std::ostream& os, int nest, std::set<const void*>& outtable)const
		{
			switch (type())
			{
			case LuaRef::TYPE_NIL:
				os << "nil";
				break;
			case LuaRef::TYPE_BOOL:
				os << get<bool>();
				break;
			case LuaRef::TYPE_NUMBER:
				os << get<double>();
				break;
			case LuaRef::TYPE_STRING:
				os << "'" << get<std::string>() << "'";
				break;
			case LuaRef::TYPE_TABLE:
			{
				const void* ptr= native_pointer();
				if (outtable.count(ptr))
				{
					os << "{" << ptr << "}" << std::endl;
					return;
				}
				outtable.insert(ptr);
				os << "{";
				std::map<LuaRef, LuaRef> table = map();
				bool first = true;
				for (std::map<LuaRef, LuaRef>::iterator it = table.begin(); it != table.end(); it++)
				{
					if (first) { first = false; }
					else { os << ","; }
					it->first.dump_impl(os, nest + 1, outtable);
					os << " = ";
					it->second.dump_impl(os, nest + 1, outtable);
				}
				os << "}";
			}
			break;
			case LuaRef::TYPE_LIGHTUSERDATA:
			case LuaRef::TYPE_FUNCTION:
			case LuaRef::TYPE_USERDATA:
			case LuaRef::TYPE_THREAD:
				os << typeName() << "(" << native_pointer() << ")";
				break;
			default:
				os << "unknown type";
				break;
			}
		}
	};

	template<typename T>
	bool operator == (const LuaRef& lhs, const T& rhs)
	{
		try
		{
			return lhs.get<const T&>() == rhs;
		}
		catch (const LuaTypeMismatch&)
		{
			return false;
		}
		return false;
	}
	template<typename T>
	bool operator != (const LuaRef& lhs, const T& rhs)
	{
		return !(lhs == rhs);
	}
	inline 	bool operator == (const LuaRef& lhs, const char* rhs) { return lhs.get<std::string>() == rhs; }

	template<typename T>
	bool operator == (const T& lhs, const LuaRef& rhs)
	{
		try
		{
			return lhs == rhs.get<const T&>();
		}
		catch (const LuaTypeMismatch&)
		{
			return false;
		}
		return false;
	}
	template<typename T>
	bool operator != (const T& lhs, const LuaRef& rhs)
	{
		return !(lhs == rhs);
	}
	inline bool operator == (const char* lhs, const LuaRef& rhs) { return lhs == rhs.get<std::string>(); }


	inline lua_type_traits<LuaRef>::get_type lua_type_traits<LuaRef>::get(lua_State* l, int index)
	{
		lua_pushvalue(l, index);
		return LuaRef(l, StackTop());
	}
	inline int lua_type_traits<LuaRef>::push(lua_State* l, lua_type_traits<LuaRef>::push_type v)
	{
		v.push(l);
		return 1;
	}
	inline std::ostream& operator<<(std::ostream& os, const LuaRef& ref)
	{
		ref.dump(os);
		return os;
	}


	namespace util
	{

		template<class Result>
		inline Result get_result_impl(lua_State* l, int startindex, types::typetag<Result>)
		{
			return lua_type_traits<Result>::get(l, startindex);
		}
		template<>
		inline std::vector<LuaRef> get_result_impl<std::vector<LuaRef> >(lua_State* l, int startindex, types::typetag<std::vector<LuaRef> >)
		{
			std::vector<LuaRef> results;
			int num = lua_gettop(l);
			if (startindex-1 < num)
			{
				results.reserve(num);
			}
			for (int i = startindex - 1; i < num; ++i)
			{
				results.push_back(LuaRef(l, StackTop()));
			}
			std::reverse(results.begin(), results.end());
			return results;
		}
#if KAGUYA_USE_CPP11
		inline standard::tuple<> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}
		template<typename T, typename... TYPES>
		inline standard::tuple<T, TYPES...> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<T, TYPES...> > tag)
		{
			return standard::tuple_cat(standard::tuple<T>(lua_type_traits<T>::get(l, index)),
				get_result_tuple_impl(l, index + 1, types::typetag<standard::tuple<TYPES...> >()));
		}
		template<typename... TYPES>
		inline standard::tuple<TYPES...> get_result_impl(lua_State* l, int startindex, types::typetag<standard::tuple<TYPES...> > tag)
		{
			return get_result_tuple_impl<TYPES...>(l, startindex, tag);
		}
#else

		inline standard::tuple<> get_result_impl(lua_State *l, int startindex, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_TARG(N) KAGUYA_PP_CAT(A,N)
#define KAGUYA_GET_DEF(N) lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(l, N + startindex - 1) 
#define KAGUYA_GET_TUPLE_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> get_result_impl(lua_State *l,int startindex,types::typetag<standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> >)\
		{\
			return standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG) >(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_GET_DEF));\
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_GET_TUPLE_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_TARG
#undef KAGUYA_GET_DEF
#undef KAGUYA_GET_TUPLE_DEF
#endif

		template<class Result>
		inline Result get_result(lua_State* l,int startindex)
		{
			return get_result_impl(l, startindex, types::typetag<Result>());
		}
		template<>
		inline void get_result<void>(lua_State* l, int startindex)
		{
		}


	}
}


#define KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(CLASSNAME) \
CLASSNAME() :LuaRef()\
{\
}\
CLASSNAME(const CLASSNAME& ref) :LuaRef(ref)\
{\
}\
template<typename T>\
CLASSNAME(const T& ref) :LuaRef(toLuaRef(ref))\
{\
}\
CLASSNAME(const LuaRef& ref) :LuaRef(ref)\
{\
	typecheck();\
}\
CLASSNAME(lua_State* state, StackTop top) :LuaRef(state, top)\
{\
}\
CLASSNAME& operator=(const LuaRef& ref)\
{\
	static_cast<LuaRef&>(*this) = static_cast<const LuaRef&>(ref);\
	typecheck();\
	return *this;\
}\
CLASSNAME& operator=(const CLASSNAME& ref)\
{\
	static_cast<LuaRef&>(*this) = static_cast<const LuaRef&>(ref);\
	return *this;\
}\
bool operator==(const CLASSNAME& other)const\
{\
	return static_cast<const LuaRef&>(*this) == static_cast<const LuaRef&>(other);\
}\
bool operator!=(const CLASSNAME& other)const\
{\
	return !(*this == other);\
}\
bool operator<=(const CLASSNAME& other)const\
{\
	return static_cast<const LuaRef&>(*this) <= static_cast<const LuaRef&>(other);\
}\
bool operator<(const CLASSNAME& other)const\
{\
	return static_cast<const LuaRef&>(*this) < static_cast<const LuaRef&>(other);\
}\
bool operator>=(const CLASSNAME& other)const\
{\
	return other <= *this;\
}\
bool operator>(const CLASSNAME& other)const\
{\
	return other < *this;\
}


#if KAGUYA_USE_RVALUE_REFERENCE

#define KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(CLASSNAME) \
CLASSNAME(LuaRef&& src)throw() :LuaRef(standard::forward<LuaRef>(src))\
{\
	typecheck(); \
}\
CLASSNAME& operator =(LuaRef&& src)throw()\
{\
	static_cast<LuaRef&>(*this) = standard::forward<LuaRef>(src);\
	typecheck(); \
	return *this;\
}\
CLASSNAME(CLASSNAME&& src)throw() :LuaRef(standard::forward<LuaRef>(src))\
{\
}\
CLASSNAME& operator =(CLASSNAME&& src)throw()\
{\
static_cast<LuaRef&>(*this) = standard::forward<LuaRef>(src); \
return *this; \
}
#else
#define KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(CLASSNAME)
#endif

#if KAGUYA_USE_CPP11
#else
namespace std
{
	template <> inline void swap(kaguya::LuaRef& a, kaguya::LuaRef& b)
	{
		a.swap(b);
	}
}
#endif
