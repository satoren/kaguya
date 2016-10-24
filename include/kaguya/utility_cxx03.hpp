// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <string>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace util
	{
		///! 
		struct null_type {};


#define KAGUYA_PP_STRUCT_TDEF_REP(N) KAGUYA_PP_CAT(typename A,N) = null_type
#define KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_STRUCT_TDEF_REP)

		template<KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT(KAGUYA_FUNCTION_MAX_ARGS)>
			struct TypeTuple {
		};

		template<typename F>struct TypeTupleSize;

#define KAGUYA_TYPE_TUPLE_SIZE_DEF(N) \
		template< KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
		struct TypeTupleSize<TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)> >\
		{\
			static const size_t value = N;\
		};\

		KAGUYA_TYPE_TUPLE_SIZE_DEF(0)
		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_TYPE_TUPLE_SIZE_DEF)
#undef KAGUYA_TYPE_TUPLE_SIZE_DEF

		template<typename Ret, typename typetuple>struct CFuntionType;
#define KAGUYA_CFUNCTION_TYPE_DEF(N) \
		template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct CFuntionType<Ret ,TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)> >\
		{\
			typedef Ret (*type)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N));\
		};\

		KAGUYA_CFUNCTION_TYPE_DEF(0)
		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_CFUNCTION_TYPE_DEF)
#undef KAGUYA_CFUNCTION_TYPE_DEF


		template<typename Ret , KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT(KAGUYA_FUNCTION_MAX_ARGS)>
			struct FunctionSignatureType {
				typedef Ret result_type; 
				typedef TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(KAGUYA_FUNCTION_MAX_ARGS)> argument_type_tuple;
				static const size_t argument_count = TypeTupleSize<argument_type_tuple>::value;
				typedef typename CFuntionType<Ret, argument_type_tuple>::type c_function_type;
		};
		
		template <typename T, typename Enable = void>
		struct FunctionSignature ;

#define KAGUYA_MEMBER_FUNCTION_SIGNATURE_DEF(N) \
		template <typename T, typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct FunctionSignature<Ret(T::*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))> {\
			typedef FunctionSignatureType<Ret, T&  KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> type;\
		};\
		template <typename T, typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct FunctionSignature<Ret(T::*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)) const> {\
			typedef FunctionSignatureType<Ret, const T&  KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> type;\
		};\


#define KAGUYA_FUNCTION_SIGNATURE_DEF(N) \
		template<class Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct FunctionSignature<Ret(*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))> {\
			typedef FunctionSignatureType<Ret  KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> type;\
		};\
		template<class Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct FunctionSignature<Ret(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))> {\
			typedef FunctionSignatureType<Ret  KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> type;\
		};\
		template<class Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
		struct FunctionSignature<standard::function<Ret(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))> > {\
			typedef FunctionSignatureType<Ret  KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> type;\
		};\

		KAGUYA_MEMBER_FUNCTION_SIGNATURE_DEF(0)
		KAGUYA_PP_REPEAT_DEF(KAGUYA_PP_DEC(KAGUYA_FUNCTION_MAX_ARGS), KAGUYA_MEMBER_FUNCTION_SIGNATURE_DEF)
#undef KAGUYA_MEMBER_FUNCTION_SIGNATURE_DEF
		KAGUYA_FUNCTION_SIGNATURE_DEF(0)
		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_FUNCTION_SIGNATURE_DEF)
#undef KAGUYA_FUNCTION_SIGNATURE_DEF

		template<typename F>
		struct FunctionResultType
		{
			typedef typename FunctionSignature<F>::type::result_type type;
		};

		template<std::size_t remain, class result, bool flag = remain <= 0>
		struct TypeIndexGet
		{
		};

#define KAGUYA_TYPE_INDEX_GET_DEF(N) \
		template<std::size_t remain, class arg  KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N) >\
		struct TypeIndexGet<remain, TypeTuple<arg KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)>, true>\
		{\
			typedef arg type;\
		};\
		template<std::size_t remain, class arg  KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N) >\
		struct TypeIndexGet<remain, TypeTuple<arg KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)>, false>\
			: TypeIndexGet<remain - 1, TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)> >\
		{\
		};\

//		KAGUYA_TYPE_INDEX_GET_DEF(0);
		KAGUYA_PP_REPEAT_DEF(KAGUYA_PP_DEC(KAGUYA_FUNCTION_MAX_ARGS), KAGUYA_TYPE_INDEX_GET_DEF)
#undef KAGUYA_TYPE_INDEX_GET_DEF

		template<std::size_t N, typename F>
		struct ArgumentType
		{
			typedef typename TypeIndexGet<N, typename FunctionSignature<F>::type::argument_type_tuple>::type type;
		};
		
		
		namespace detail
		{

#define KAGUYA_INVOKE_HELPER_DEF(N) \
			template<class F,class ThisType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N),typename T>\
			typename FunctionResultType<F>::type invoke_helper(typename FunctionResultType<F>::type(T::*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)), ThisType this_ KAGUYA_PP_ARG_DEF_REPEAT_CONCAT(N))\
			{\
				return (this_.*f)(KAGUYA_PP_ARG_REPEAT(N));\
			}\
			template<class F,class ThisType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N),typename T>\
			typename FunctionResultType<F>::type invoke_helper(typename FunctionResultType<F>::type(T::*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))const,ThisType this_ KAGUYA_PP_ARG_DEF_REPEAT_CONCAT(N))\
			{\
				return (this_.*f)(KAGUYA_PP_ARG_REPEAT(N));\
			}\
			template<class F KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			typename FunctionResultType<F>::type invoke_helper(F f KAGUYA_PP_ARG_DEF_REPEAT_CONCAT(N))\
			{\
				return f(KAGUYA_PP_ARG_REPEAT(N));\
			}\

			KAGUYA_INVOKE_HELPER_DEF(0)
			KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_INVOKE_HELPER_DEF)
#undef KAGUYA_INVOKE_HELPER_DEF
		}

		
#define KAGUYA_INVOKE_DEF(N) \
		template<class F KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			typename FunctionResultType<F>::type invoke(F f KAGUYA_PP_ARG_DEF_REPEAT_CONCAT(N))\
		{\
			return detail::invoke_helper<F KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)>(f KAGUYA_PP_ARG_REPEAT_CONCAT(N)); \
		}\
		
		KAGUYA_INVOKE_DEF(0)
		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_INVOKE_DEF)
#undef KAGUYA_INVOKE_DEF

#undef KAGUYA_PP_STRUCT_TDEF_REP
#undef KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT

		namespace conv_helper_detail
		{
#define KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_REP(N) || lua_type_traits<KAGUYA_PP_CAT(A,N)>::checkType(state, index)
#define KAGUYA_CONVERTIBLE_REG_HELPER_STRICT_CHECK_TYPE_REP(N) || lua_type_traits<KAGUYA_PP_CAT(A,N)>::strictCheckType(state, index)
#define KAGUYA_CONVERTIBLE_REG_HELPER_ST_GET_REP(N) if (lua_type_traits<KAGUYA_PP_CAT(A,N)>::strictCheckType(state, index)){ return lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(state, index);}else
#define KAGUYA_CONVERTIBLE_REG_HELPER_GET_REP(N) if (lua_type_traits<KAGUYA_PP_CAT(A,N)>::checkType(state, index)){ return lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(state, index);}else

			template<typename To>
			bool checkType(lua_State* state, int index, TypeTuple<> t)
			{
				return false;
			}
#define KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_DEF(N)\
			template<typename To,KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)> \
			bool checkType(lua_State* state, int index, TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>)\
			{\
				return false KAGUYA_PP_REPEAT(N, KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_REP);\
			}

			KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_DEF)
#undef KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_DEF
			template<typename To>
			bool strictCheckType(lua_State* state, int index, TypeTuple<> t)
			{
				return false;
			}
#define KAGUYA_CONVERTIBLE_REG_HELPER_ST_CHECK_TYPE_DEF(N)\
			template<typename To,KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)> \
			bool strictCheckType(lua_State* state, int index, TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>)\
			{\
				return false KAGUYA_PP_REPEAT(N, KAGUYA_CONVERTIBLE_REG_HELPER_STRICT_CHECK_TYPE_REP);\
			}
			KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_CONVERTIBLE_REG_HELPER_ST_CHECK_TYPE_DEF)
#undef KAGUYA_CONVERTIBLE_REG_HELPER_ST_CHECK_TYPE_DEF
#define KAGUYA_CONVERTIBLE_REG_HELPER_GET_DEF(N)\
			template<typename To,KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)> \
			To get(lua_State* state, int index, TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>)\
			{\
				KAGUYA_PP_REPEAT(N, KAGUYA_CONVERTIBLE_REG_HELPER_ST_GET_REP);\
				KAGUYA_PP_REPEAT(N, KAGUYA_CONVERTIBLE_REG_HELPER_GET_REP);\
				throw LuaTypeMismatch();\
			}
			KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_ARGS, KAGUYA_CONVERTIBLE_REG_HELPER_GET_DEF)
#undef KAGUYA_CONVERTIBLE_REG_HELPER_GET_DEF

#undef KAGUYA_CONVERTIBLE_REG_HELPER_CHECK_TYPE_REP
#undef KAGUYA_CONVERTIBLE_REG_HELPER_STRICT_CHECK_TYPE_REP
#undef KAGUYA_CONVERTIBLE_REG_HELPER_ST_GET_REP
#undef KAGUYA_CONVERTIBLE_REG_HELPER_GET_REP
		}

#define KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REP(N) KAGUYA_PP_CAT(typename A,N) = null_type
#define KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REP)

		template<typename To, KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REPEAT(KAGUYA_FUNCTION_MAX_ARGS)>
		struct ConvertibleRegisterHelper {
			typedef To get_type;

			typedef TypeTuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(KAGUYA_FUNCTION_MAX_ARGS)> conv_types;

			static bool checkType(lua_State* state, int index)
			{
				return conv_helper_detail::checkType<To>(state, index, conv_types());
			}
			static bool strictCheckType(lua_State* state, int index)
			{
				return conv_helper_detail::strictCheckType<To>(state, index, conv_types());
			}
			static get_type get(lua_State* state, int index)
			{
				return conv_helper_detail::get<To>(state, index, conv_types());
			}
		};
#undef KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REP
#undef KAGUYA_PP_CONVERTIBLE_REG_HELPER_DEF_REPEAT

	}

}
