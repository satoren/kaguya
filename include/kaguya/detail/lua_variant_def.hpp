// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kaguya/detail/lua_function_def.hpp"
#include "kaguya/detail/lua_table_def.hpp"


namespace kaguya
{
	class LuaRef;
	class LuaTable;
	class TableKeyReference;
	class mem_fun_binder;

	template<typename Derived>
	class LuaVariantImpl :public LuaTableImpl<Derived>,
		public LuaTableOrUserDataImpl<Derived>,
		public LuaFunctionImpl<Derived>, 
		public LuaThreadImpl<Derived>, 
		public LuaBasicTypeFunctions<Derived>
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
		int type() const
		{
			lua_State* state = state_();
			util::ScopedSavedStack save(state);
			return lua_type(state, pushStackIndex_(state));
		}
		const char* typeName()const
		{
			return lua_typename(state_(), type());
		}
		
#if KAGUYA_USE_CPP11
		template<class...Args> FunctionResults operator()(Args&&... args)
		{
			int t = type();
			if (t == LUA_TTHREAD)
			{
				return resume<FunctionResults>(std::forward<Args>(args)...);
			}
			else if (t == LUA_TFUNCTION)
			{
				return call<FunctionResults>(std::forward<Args>(args)...);
			}
			else
			{
				except::typeMismatchError(state_(), " is not function or thread");
				return FunctionResults(state_());
			}
		}
#else
		#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_FUNCTION_ARGS_DEF(N)
#define KAGUYA_CALL_ARGS(N)
#define KAGUYA_PP_FARG(N) const KAGUYA_PP_CAT(A,N)& KAGUYA_PP_CAT(a,N)

#define KAGUYA_OP_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline FunctionResults operator()(KAGUYA_FUNCTION_ARGS_DEF(N))\
	{\
			int t = type();\
			if (t == LUA_TTHREAD)\
			{\
				return resume<FunctionResults>(KAGUYA_CALL_ARGS(N)); \
			}\
			else if (t == LUA_TFUNCTION)\
			{\
				return call<FunctionResults>(KAGUYA_CALL_ARGS(N)); \
			}\
			else\
			{\
				except::typeMismatchError(state_(), " is not function or thread");\
				return FunctionResults(state_());\
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
#define KAGUYA_PUSH_ARG_DEF(N) KAGUYA_PP_CAT(a,N) 

			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
#undef KAGUYA_OP_FN_DEF
#undef KAGUYA_TEMPLATE_PARAMETER

#undef KAGUYA_CALL_ARGS
#undef KAGUYA_FUNCTION_ARGS_DEF
#undef KAGUYA_PUSH_ARG_DEF
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_CALL_DEF
#undef KAGUYA_OP_FN_DEF
#endif


		using LuaTableImpl<Derived>::operator[];
		using LuaTableOrUserDataImpl<Derived>::operator[];
	};

}