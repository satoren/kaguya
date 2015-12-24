#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/error_handler.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"


namespace kaguya
{
	struct StackTop {};


	class TableKeyReference;
	class FunEvaluator;
	class mem_fun_binder;


	class LuaRef
	{
		friend class TableKeyReference;
		lua_State *state_;
		int ref_;

		void unref()
		{
			if (!isNilref())
			{
				luaL_unref(state_, LUA_REGISTRYINDEX, ref_);
				state_ = 0;
				ref_ = LUA_REFNIL;
			}
		}

		struct gettablekey
		{
			std::vector<LuaRef>& v_;
			gettablekey(std::vector<LuaRef>&v) :v_(v) {}
			void operator ()(LuaRef key, const LuaRef&)
			{
				v_.push_back(key);
			}
		};
		struct gettablevalue
		{
			std::vector<LuaRef>& v_;
			gettablevalue(std::vector<LuaRef>&v) :v_(v) {}
			void operator ()(const LuaRef&, LuaRef value)
			{
				v_.push_back(value);
			}
		};
		struct gettablemap
		{
			std::map<LuaRef, LuaRef>& m_;
			gettablemap(std::map<LuaRef, LuaRef>& m) :m_(m) {}
			void operator ()(LuaRef key, LuaRef value)
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
			int t = (value_type)lua_type(state_, -1);
			if (t != LUA_TTABLE)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return;
			}
			int kc = types::push(state_, key);//push table key
			int vc = types::push(state_, standard::forward<V>(value));//push value

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

	public:
		int thread_status()const
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

		struct NoMainCheck {};
		bool isNilref()const { return state_ == 0 || ref_ == LUA_REFNIL; }
		enum value_type
		{
			TYPE_NIL = LUA_TNIL,
			TYPE_BOOL = LUA_TBOOLEAN,
			TYPE_LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
			TYPE_NUMBER = LUA_TNUMBER,
			TYPE_STRING = LUA_TSTRING,
			TYPE_TABLE = LUA_TTABLE,
			TYPE_FUNCTION = LUA_TFUNCTION,
			TYPE_USERDATA = LUA_TUSERDATA,
			TYPE_THREAD = LUA_TTHREAD,
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
			int vc = types::push(state_, v);
			if (!pushCountCheck<T>(vc)) { return; }
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}
		template<typename T>
		LuaRef(lua_State* state, T v) : state_(state)
		{
			util::ScopedSavedStack save(state_);
			int vc = types::push(state_, v);
			if (!pushCountCheck<T>(vc)) { return; }
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			state_ = toMainThread(state_);
		}
		~LuaRef()
		{
			unref();
		}

		//push to Lua stack
		void push()const
		{
			lua_rawgeti(state_, LUA_REGISTRYINDEX, ref_);
		}
		void push(lua_State* state)const
		{
			if (isNilref())
			{
				lua_pushnil(state);
				return;
			}
#if LUA_VERSION_NUM >= 502
			if (state != state_)
			{//state check
				assert(toMainThread(state) == toMainThread(state_));
			}
#endif
			lua_rawgeti(state, LUA_REGISTRYINDEX, ref_);
		}

		template<typename T>
		bool typeTest()const
		{
			util::ScopedSavedStack save(state_);
			push();//push to stack
			return types::strictCheckType(state_, -1, types::typetag<T>());
		}
		template<typename T>
		bool weakTypeTest()const
		{
			util::ScopedSavedStack save(state_);
			push();//push to stack
			return types::checkType(state_, -1, types::typetag<T>());
		}

		template<typename T>
		typename traits::arg_get_type<T>::type get()const
		{
			typedef typename traits::arg_get_type<T>::type get_type;
			util::ScopedSavedStack save(state_);
			push(state_);
			if (!types::checkType(state_, -1, types::typetag<get_type>()))
			{
				throw LuaTypeMismatch(typeName() + std::string("is not ") + typeid(T).name());
			}
			return types::get(state_, -1, types::typetag<get_type>());
		}

		template<typename T>
		operator T()const {
			return get<T>();
		}

		operator bool()const {
			return !isNilref() && get<bool>() == true;
		}

		bool setFunctionEnv(const LuaRef& env)
		{
			util::ScopedSavedStack save(state_);
			if (env.type() != TYPE_TABLE)
			{
				except::typeMismatchError(state_, "env is not table:" + typeName());
				return false;
			}
			if (type() != TYPE_FUNCTION)
			{
				except::typeMismatchError(state_, "this is not function" + typeName());
				return false;
			}
			push();
			env.push();
			lua_setupvalue(state_, -2, 1);
			return true;
		}
		bool setFunctionEnv(NewTable env)
		{
			return setFunctionEnv(LuaRef(state_, env));
		}
		LuaRef getFunctionEnv()
		{
			util::ScopedSavedStack save(state_);
			if (type() != TYPE_FUNCTION)
			{
				except::typeMismatchError(state_, "this is not function" + typeName());
				return LuaRef(state_);
			}
			push();
			lua_getupvalue(state_, -1, 1);
			return LuaRef(state_, StackTop());
		}

#include "kaguya/gen/luaref_fun_def.inl"

		mem_fun_binder operator->*(const char* key);

		TableKeyReference operator[](const LuaRef& key);
		TableKeyReference operator[](const char* str);
		TableKeyReference operator[](const std::string& str);
		TableKeyReference operator[](int index);

		const LuaRef operator[](const LuaRef& key)const
		{
			return getField(key);
		}
		const LuaRef operator[](const char* str)const
		{
			return getField(str);
		}
		const LuaRef operator[](const std::string& str)const
		{
			return getField(str);
		}
		const LuaRef operator[](int index)const
		{
			return getField(index);
		}

		LuaRef getField(const LuaRef& key)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = (value_type)lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			key.push(state_);
			lua_gettable(state_, -2);
			return LuaRef(state_, StackTop(), NoMainCheck());
		}
		LuaRef getField(const char* str)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = (value_type)lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			types::push(state_, str);
			lua_gettable(state_, -2);
			return LuaRef(state_, StackTop());
		}
		LuaRef getField(const std::string& str)const
		{
			return getField(str.c_str());
		}
		LuaRef getField(int index)const
		{
			if (ref_ == LUA_REFNIL)
			{
				except::typeMismatchError(state_, "is nil");
				return LuaRef(state_);
			}
			util::ScopedSavedStack save(state_);
			push();
			int t = (value_type)lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				except::typeMismatchError(state_, typeName() + "is not table");
				return LuaRef(state_);
			}
			types::push(state_, index);
			lua_gettable(state_, -2);
			return LuaRef(state_, StackTop(), NoMainCheck());
		}
		template<typename T>
		void setField(int key, T value)
		{
			setFieldImpl<int, T>(key, value);
		}

		template<typename T>
		void setField(const char* key, T value)
		{
			setFieldImpl<const char*, T>(key, value);
		}
		template<typename T>
		void setField(const std::string& key, T value)
		{
			setField(key.c_str(), value);
		}
		template<typename T>
		void setField(const LuaRef& key, T value)
		{
			setFieldImpl<LuaRef, T>(key, value);
		}

		template<class Fun>void foreach_table(Fun f)const
		{
			if (ref_ == LUA_REFNIL)
			{
				return;
			}
			util::ScopedSavedStack save(state_);
			push(state_);
			int t = (value_type)lua_type(state_, -1);
			if (t != LUA_TTABLE && t != LUA_TUSERDATA)
			{
				return;
			}
			int top = lua_gettop(state_);
			lua_pushnil(state_);
			while (lua_next(state_, top) != 0)
			{
				LuaRef value(state_, StackTop(), NoMainCheck());
				lua_pushvalue(state_, -1);//push key to stack for next
				LuaRef key(state_, StackTop(), NoMainCheck());
				f(key, value);
			}
		}
		std::vector<LuaRef> keys()const
		{
			std::vector<LuaRef> res;
			foreach_table(gettablekey(res));
			return res;
		}
		std::vector<LuaRef> values()const
		{
			std::vector<LuaRef> res;
			foreach_table(gettablevalue(res));
			return res;
		}
		std::map<LuaRef, LuaRef> map()const
		{
			std::map<LuaRef, LuaRef> res;
			foreach_table(gettablemap(res));
			return res;
		}

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
	bool operator == (const LuaRef& lhs, const char* rhs) { return lhs.get<std::string>() == rhs; }

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
	bool operator == (const char* lhs, const LuaRef& rhs) { return lhs == rhs.get<std::string>(); }



	typedef LuaRef Selector;

	namespace types
	{
		template<>
		inline bool checkType(lua_State* l, int index, typetag<LuaRef>)
		{
			return true;
		}
		template<>
		inline LuaRef get(lua_State* l, int index, typetag<LuaRef> tag)
		{
			lua_pushvalue(l, index);
			return LuaRef(l, StackTop());
		}
		template<>
		inline int push(lua_State* l, const LuaRef& ref)
		{
			ref.push(l);
			return 1;
		}
	}

	namespace traits
	{

		template<>
		struct arg_get_type<const LuaRef& > {
			typedef LuaRef type;
		};

	}
}
