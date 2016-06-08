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
	template<typename KEY>
	class TableKeyReference;
	class MemberFunctionBinder;

	namespace detail {

		template<typename Derived>
		class LuaVariantImpl :public LuaTableImpl<Derived>,
			public LuaTableOrUserDataImpl<Derived>,
			public detail::LuaFunctionImpl<Derived>,
			public detail::LuaThreadImpl<Derived>,
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
			using LuaBasicTypeFunctions<Derived>::type;
			using LuaBasicTypeFunctions<Derived>::typeName;

			template<typename T>
			bool typeTest()const
			{
				lua_State* state = state_();
				util::ScopedSavedStack save(state);
				return lua_type_traits<T>::strictCheckType(state, pushStackIndex_(state));
			}
			template<typename T>
			bool weakTypeTest()const
			{
				lua_State* state = state_();
				util::ScopedSavedStack save(state);
				return lua_type_traits<T>::checkType(state, pushStackIndex_(state));
			}



#if KAGUYA_USE_CPP11
			template<class...Args> FunctionResults operator()(Args&&... args);
#else
#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_FUNCTION_ARGS_DEF(N)
#define KAGUYA_PP_FARG(N) const KAGUYA_PP_CAT(A,N)& KAGUYA_PP_CAT(a,N)

#define KAGUYA_OP_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline FunctionResults operator()(KAGUYA_FUNCTION_ARGS_DEF(N));

			KAGUYA_OP_FN_DEF(0)

#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_FUNCTION_ARGS_DEF
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>
#define KAGUYA_FUNCTION_ARGS_DEF(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG)

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PUSH_ARG_DEF(N) KAGUYA_PP_CAT(a,N) 

				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
#undef KAGUYA_OP_FN_DEF
#undef KAGUYA_TEMPLATE_PARAMETER

#undef KAGUYA_FUNCTION_ARGS_DEF
#undef KAGUYA_PUSH_ARG_DEF
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_CALL_DEF
#undef KAGUYA_OP_FN_DEF
#endif
		};
	}
}