#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/exception.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"

namespace kaguya
{

	/**
	* Reference of Lua function.
	*/
	class LuaFunction :public LuaRef
	{
		void typecheck()
		{
			if (type() != TYPE_FUNCTION)
			{
				except::typeMismatchError(state_, "not function");
				LuaRef::unref();
			}
		}
		//hide other type functions
		using LuaRef::threadStatus;
		using LuaRef::isThreadDead;

		using LuaRef::getField;
		using LuaRef::setField;
		using LuaRef::keys;
		using LuaRef::values;
		using LuaRef::map;
		using LuaRef::operator[];
		using LuaRef::foreach_table;
		using LuaRef::operator->*;
		using LuaRef::costatus;
		using LuaRef::getMetatable;
		using LuaRef::setMetatable;



	public:
		KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(LuaFunction);
		KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(LuaFunction);

		/**
		* @name operator()
		* @brief call lua function.
		* @param arg... function args
		*/
		using LuaRef::operator();
		using LuaRef::setFunctionEnv;
		using LuaRef::getFunctionEnv;
		
		/**
		* @name loadstring
		* @brief load lua code .
		* @param state pointer to lua_State
		* @param luacode string 
		*/
		static LuaFunction loadstring(lua_State* state, const std::string& luacode)
		{
			return loadstring(state,luacode.c_str());
		}
		/**
		* @name loadstring
		* @brief load lua code .
		* @param state pointer to lua_State
		* @param luacode string
		*/
		static LuaFunction loadstring(lua_State* state, const char* luacode)
		{
			util::ScopedSavedStack save(state);
			int status = luaL_loadstring(state, luacode);

			if (status)
			{
				ErrorHandler::instance().handle(status, state);
				return LuaRef(state);
			}
			return LuaFunction(state, StackTop());
		}


		/**
		* @name loadfile
		* @brief If there are no errors,compiled file as a Lua function and return.
		*  Otherwise send error message to error handler and return nil reference
		* @param file  file path of lua script
		* @return reference of lua function
		*/
		static LuaFunction loadfile(lua_State* state, const std::string& file)
		{
			return loadfile(state,file.c_str());
		}
		static LuaFunction loadfile(lua_State* state, const char* file)
		{
			util::ScopedSavedStack save(state);

			int status = luaL_loadfile(state, file);

			if (status)
			{
				ErrorHandler::instance().handle(status, state);
				return LuaRef(state);
			}
			return LuaFunction(state, StackTop());
		}
	};

	/**
	* Reference of Lua thread(==coroutine).
	*/
	class LuaThread :public LuaRef
	{
		void typecheck()
		{
			if (type() != TYPE_THREAD)
			{
				except::typeMismatchError(state_, "not lua thread");
				LuaRef::unref();
			}
		}

		//hide other type functions
		using LuaRef::setFunctionEnv;
		using LuaRef::getFunctionEnv;
		using LuaRef::getField;
		using LuaRef::setField;
		using LuaRef::keys;
		using LuaRef::values;
		using LuaRef::map;
		using LuaRef::operator[];
		using LuaRef::foreach_table;
		using LuaRef::operator->*;
		using LuaRef::getMetatable;
		using LuaRef::setMetatable;
	public:
		KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(LuaThread);
		KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(LuaThread);


		/**
		* create new thread.
		* @param state lua_State pointer
		*/
		LuaThread(lua_State* state) :LuaRef(state, NewThread())
		{
		}

		/**
		* resume lua thread.
		* @param arg... function args
		*/
		using LuaRef::operator();

		/**
		* resume lua thread.
		* @return threadStatus
		*/
		using LuaRef::threadStatus;

		/**
		* resume lua thread.
		* @return if thread can resume,return true.Otherwise return false.
		*/
		using LuaRef::isThreadDead;

		/**
		* resume lua thread.
		* @return coroutine status.
		*/
		using LuaRef::costatus;
	};


	class FunctionResults
	{
		std::vector<LuaRef> results_;
	public:
		FunctionResults()
		{
			results_.resize(1);//for empty value
		}
		FunctionResults(std::vector<LuaRef> results)
		{
			std::swap(results_, results);
			if (results_.empty())
			{
				results_.resize(1);//for empty value
			}
		}


		template<typename T>
		typename lua_type_traits<T>::get_type get()const
		{
			return results_.front().get<typename lua_type_traits<T>::get_type>();
		}
		template<typename T>
		operator T()const {
			return get<T>();
		}

		template<typename T>
		bool operator == (const T v)const
		{
			return get<T>() == v;
		}
		template<typename T>
		bool operator != (const T v)const
		{
			return !((*this) == v);
		}
		bool operator == (const char* v)const { return get<std::string>() == v; }

		const std::vector<LuaRef>& get_result(int unused=0)const
		{
			return results_;
		}
#if KAGUYA_USE_CPP11
		template<class... Args>
		FunctionResults operator()(Args&&... args)
		{
			return results_.front()(standard::forward<Args>(args)...);
		}
#else
		/** call lua-function from lua-function result
		* template<class... Args>FunctionResults operator()(Args... args);
		*/
#define KAGUYA_DELEGATE_LUAREF results_.front()
#include "kaguya/gen/delegate_to_luaref.inl"
#undef KAGUYA_DELEGATE_LUAREF
#endif
	};

	template<>	struct lua_type_traits<FunctionResults> {
		static int push(lua_State* l, const FunctionResults& ref)
		{
			const std::vector<LuaRef>& v = ref.get_result();
			for (std::vector<LuaRef>::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				it->push();
			}
			return static_cast<int>(v.size());
		}
	};
	
	template<>	struct lua_type_traits<LuaFunction> {
		typedef LuaFunction get_type;
		typedef LuaFunction push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isfunction(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isfunction(l, index) || lua_isnil(l, index);
		}
		static LuaFunction get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaFunction(l, StackTop());
		}
		static int push(lua_State* l, const LuaFunction& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaFunction&> :lua_type_traits<LuaFunction> {};

	template<>	struct lua_type_traits<LuaThread>{
		typedef LuaThread get_type;
		typedef LuaThread push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isthread(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isthread(l, index) || lua_isnil(l, index);
		}
		static LuaThread get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaThread(l, StackTop());
		}
		static int push(lua_State* l, const LuaThread& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaThread&> :lua_type_traits<LuaThread> {};
	

	inline LuaRef toLuaRef(const LuaFunction& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline LuaRef toLuaRef(const LuaThread& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline LuaRef toLuaRef(const FunctionResults& ref)
	{
		return ref.get<LuaRef>();
	}

	//!FunEvaluator is deleted. typedef for compatibility
	KAGUYA_DEPRECATED typedef	FunctionResults FunEvaluator;

	/**
	* @brief table and function binder.
	* state["table"]->*"fun"() is table:fun() in Lua
	* @param arg... function args
	*/
	class mem_fun_binder
	{
	public:
		template<class T>
		mem_fun_binder(LuaRef self, T key) :self_(self), fun_(self_.getField(key))
		{
		}

#if KAGUYA_USE_CPP11
		/**
		* @name operator()
		* @brief call function with self.
		* @param arg... function args
		*/
		template<typename... Args>
		FunctionResults operator()(Args... args)
		{
			return fun_(self_, args...);
		}
#else
		/**
		* @name operator()
		* @brief call function with self.
		* @param arg... function args
		*/
		//@{
#include "kaguya/gen/luaref_mem_fun_def.inl"
		//@}

#endif
	private:
		LuaRef self_;//Table or Userdata
		LuaFunction fun_;
	};

	inline mem_fun_binder LuaRef::operator->*(const char* key)
	{
		return mem_fun_binder(*this, key);
	}
}

namespace kaguya
{
#include "kaguya/gen/luaref_fun.inl"
}
