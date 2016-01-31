#pragma once

#include <vector>
#include <map>
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
		using LuaRef::resume;



	public:
		KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(LuaFunction);
		KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(LuaFunction);

		/**
		* @name operator()
		* @brief call lua function.
		* @param arg... function args
		*/
		using LuaRef::operator();
		using LuaRef::call;
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
			return loadstring(state, luacode.c_str());
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
			return loadfile(state, file.c_str());
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
		using LuaRef::call;
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


		using LuaRef::resume;

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

		const std::vector<LuaRef>& get_result(int unused = 0)const
		{
			return results_;
		}

#define KAGUYA_DELEGATE_LUAREF results_.front()
#include "kaguya/delegate_to_luaref.inl"
#undef KAGUYA_DELEGATE_LUAREF
	};


	std::ostream& operator<<(std::ostream& os, const FunctionResults& res)
	{
		std::vector<LuaRef> results = res.get_result();
		
		for (std::vector<LuaRef>::iterator it = results.end(); it != results.end(); ++it)
		{
			it->dump(os); os << ",";
		}
		
		return os;
	}

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
	template <unsigned I>
	LuaRef get(const FunctionResults& res) { return (res.get_result().size() > I)?res.get_result()[I]: LuaRef(); }

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

	template<>	struct lua_type_traits<LuaThread> {
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

#define KAGUYA_DELEGATE_LUAREF fun_
#define KAGUYA__DELEGATE_FIRST_ARG self_
#include "kaguya/delegate_to_luaref.inl"
#undef KAGUYA__DELEGATE_FIRST_ARG
#undef KAGUYA_DELEGATE_LUAREF

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
#if KAGUYA_USE_CPP11
	template<class...Args> FunctionResults LuaRef::operator()(Args... args)
	{
		int t = type();
		if (t == TYPE_THREAD)
		{
			return FunctionResults(resume<std::vector<LuaRef> >(standard::forward<Args>(args)...));
		}
		else if (t == TYPE_FUNCTION)
		{
			return FunctionResults(call<std::vector<LuaRef> >(standard::forward<Args>(args)...));
		}
		else
		{
			except::typeMismatchError(state_, " is not function or thread");
			return FunctionResults();
		}
	}
#else
#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_FUNCTION_ARGS_DEF(N)
#define KAGUYA_CALL_ARGS(N)

#define KAGUYA_OP_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline FunctionResults LuaRef::operator()(KAGUYA_FUNCTION_ARGS_DEF(N))\
	{\
		int t = type(); \
		if (t == TYPE_THREAD)\
		{\
			return FunctionResults(resume<std::vector<LuaRef> >(KAGUYA_CALL_ARGS(N))); \
		}\
		else if (t == TYPE_FUNCTION)\
		{\
			return FunctionResults(call<std::vector<LuaRef> >(KAGUYA_CALL_ARGS(N))); \
		}\
		else\
		{\
			except::typeMismatchError(state_, " is not function or thread"); \
			return FunctionResults(); \
		}\
	}

	KAGUYA_OP_FN_DEF(0)

#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_FUNCTION_ARGS_DEF
#undef KAGUYA_CALL_ARGS
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>
#define KAGUYA_FUNCTION_ARGS_DEF(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG)
#define KAGUYA_CALL_ARGS(N) KAGUYA_PP_REPEAT_ARG(N, KAGUYA_PUSH_ARG_DEF)

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) KAGUYA_PP_CAT(A,N) KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_ARG_DEF(N) standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N)) 

		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
#undef KAGUYA_OP_FN_DEF
#endif
}
