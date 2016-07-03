// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/utility.hpp"
#include "kaguya/object.hpp"

namespace kaguya
{
	namespace nativefunction
	{
		namespace cpp03impl
		{
			
#if defined(_MSC_VER) && _MSC_VER <= 1700
			//can not detect callable at MSVC
			template< typename T>
			struct is_callable : traits::integral_constant<bool, true> {};
#else
			template< typename T>
			struct is_callable : traits::integral_constant<bool, false> {};
#endif

			inline int call(lua_State* state, void(*f)())
			{
				f();
				return 0;
			}
			template<>struct is_callable< void(*)()> : traits::integral_constant<bool, true> {};

			inline bool checkArgTypes(lua_State* state, void(*f)())
			{
				return true;
			}
			inline bool strictCheckArgTypes(lua_State* state, void(*f)())
			{
				return true;
			}
			inline std::string argTypesName(void(*f)())
			{
				return "";
			}
			inline int argCount(void(*f)())
			{
				return 0;
			}

			inline int call(lua_State* state,standard::function<void()> f)
			{
				f();
				return 0;
			}
			template<>struct is_callable<standard::function<void()> > : traits::integral_constant<bool, true> {};
			inline bool checkArgTypes(lua_State* state, standard::function<void()> f)
			{
				return true;
			}
			inline bool strictCheckArgTypes(lua_State* state, standard::function<void()> f)
			{
				return true;
			}
			inline std::string argTypesName(standard::function<void()> f)
			{
				return "";
			}
			inline int argCount(standard::function<void()> f)
			{
				return 0;
			}
#define KAGUYA_GET_OFFSET 
#define KAGUYA_GET_CONCAT_REP(N) ,lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(state, N KAGUYA_GET_OFFSET)
#define KAGUYA_GET_REP(N) lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(state, N KAGUYA_GET_OFFSET)

#define KAGUYA_STRICT_TYPECHECK_REP(N) && lua_type_traits<KAGUYA_PP_CAT(A,N)>::strictCheckType(state, N KAGUYA_GET_OFFSET)
#define KAGUYA_TYPECHECK_REP(N) && lua_type_traits<KAGUYA_PP_CAT(A,N)>::checkType(state, N KAGUYA_GET_OFFSET)
#define KAGUYA_TYPENAME_REP(N) + typeid(KAGUYA_PP_CAT(A,N)).name() + ","

#define KAGUYA_GET_REPEAT_CONCAT(N) KAGUYA_PP_REPEAT(N,KAGUYA_GET_CONCAT_REP)
#define KAGUYA_GET_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_GET_REP)

#define KAGUYA_FUNC_DEF(N) void (*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))
#define KAGUYA_FUNC_TYPE(N) void (*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))
#define KAGUYA_CALL_FN_DEF(N) \
			template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
			inline int call(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				f(KAGUYA_GET_REPEAT(N));\
				return 0;\
			}\
			template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N)> : traits::integral_constant<bool, true> {};

			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)
#undef KAGUYA_CALL_FN_DEF
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE
#define KAGUYA_FUNC_DEF(N) Ret (*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))
#define KAGUYA_FUNC_TYPE(N) Ret (*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))
#define KAGUYA_CALL_FN_DEF(N) \
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			inline int call(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return push_return_value<Ret>(state, f(KAGUYA_GET_REPEAT(N)));\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N)> : traits::integral_constant<bool, true> {};\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool checkArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_TYPECHECK_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool strictCheckArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_STRICT_TYPECHECK_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			std::string argTypesName(KAGUYA_FUNC_DEF(N))\
			{\
				return std::string() KAGUYA_PP_REPEAT(N,KAGUYA_TYPENAME_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			int argCount(KAGUYA_FUNC_DEF(N))\
			{\
				return N  KAGUYA_GET_OFFSET;\
			}

				KAGUYA_CALL_FN_DEF(0)
				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)

#undef KAGUYA_GET_OFFSET
#define KAGUYA_GET_OFFSET + 1
#undef KAGUYA_CALL_FN_DEF
#define KAGUYA_MEM_ATTRBUTE 
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE				
#define KAGUYA_FUNC_DEF(N) void (ThisType::*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))KAGUYA_MEM_ATTRBUTE
#define KAGUYA_FUNC_TYPE(N) void (ThisType::*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))KAGUYA_MEM_ATTRBUTE
#define KAGUYA_CALL_FN_DEF(N) \
			template<typename ThisType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			inline int call(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				KAGUYA_MEM_ATTRBUTE ThisType* this_ = lua_type_traits<KAGUYA_MEM_ATTRBUTE ThisType*>::get(state, 1); \
				if (!this_){throw LuaTypeMismatch("type mismatch!!");}\
				(this_->*f)(KAGUYA_GET_REPEAT(N));\
				return 0;\
			}\
			template<typename ThisType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N)> : traits::integral_constant<bool, true> {};

				KAGUYA_CALL_FN_DEF(0)
				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)

#undef KAGUYA_MEM_ATTRBUTE
#define KAGUYA_MEM_ATTRBUTE const  
				KAGUYA_CALL_FN_DEF(0)
				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)
#undef KAGUYA_MEM_ATTRBUTE


#undef KAGUYA_CALL_FN_DEF
#define KAGUYA_MEM_ATTRBUTE 
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE
#define KAGUYA_FUNC_DEF(N) Ret (ThisType::*f)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))KAGUYA_MEM_ATTRBUTE
#define KAGUYA_FUNC_TYPE(N) Ret (ThisType::*)(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))KAGUYA_MEM_ATTRBUTE
#define KAGUYA_CALL_FN_DEF(N) \
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			inline int call(lua_State* state,KAGUYA_FUNC_DEF(N))\
			{\
				KAGUYA_MEM_ATTRBUTE ThisType* this_ = lua_type_traits<KAGUYA_MEM_ATTRBUTE ThisType*>::get(state, 1); \
				if (!this_){throw LuaTypeMismatch("type mismatch!!");}\
				return push_return_value<Ret>(state, (this_->*f)(KAGUYA_GET_REPEAT(N)));\
			}\
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N)> : traits::integral_constant<bool, true> {};\
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool checkArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return lua_type_traits<KAGUYA_MEM_ATTRBUTE ThisType*>::checkType(state, 1) \
					KAGUYA_PP_REPEAT(N,KAGUYA_TYPECHECK_REP);\
			}\
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool strictCheckArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return lua_type_traits<KAGUYA_MEM_ATTRBUTE ThisType*>::strictCheckType(state, 1) \
					KAGUYA_PP_REPEAT(N,KAGUYA_STRICT_TYPECHECK_REP);\
			}\
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			std::string argTypesName(KAGUYA_FUNC_DEF(N))\
			{\
				return typeid(ThisType).name() + std::string(",") KAGUYA_PP_REPEAT(N,KAGUYA_TYPENAME_REP);\
			}\
			template<typename ThisType,typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			int argCount(KAGUYA_FUNC_DEF(N))\
			{\
				return N  KAGUYA_GET_OFFSET;\
			}
				KAGUYA_CALL_FN_DEF(0)
				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)

#undef KAGUYA_MEM_ATTRBUTE
#define KAGUYA_MEM_ATTRBUTE const  
				KAGUYA_CALL_FN_DEF(0)
				KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)
#undef KAGUYA_MEM_ATTRBUTE

#undef KAGUYA_GET_OFFSET
#define KAGUYA_GET_OFFSET
#undef KAGUYA_CALL_FN_DEF
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE
#define KAGUYA_FUNC_DEF(N) const standard::function<void (KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>& f
#define KAGUYA_FUNC_TYPE(N) standard::function<void (KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>
#define KAGUYA_CALL_FN_DEF(N) \
			template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
			inline int call(lua_State* state,KAGUYA_FUNC_DEF(N))\
			{\
				f(KAGUYA_GET_REPEAT(N));\
				return 0;\
			}\
			template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N) > : traits::integral_constant<bool, true> {};

			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)
#undef KAGUYA_CALL_FN_DEF
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE
#define KAGUYA_FUNC_DEF(N) const standard::function<Ret (KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>& f
#define KAGUYA_FUNC_TYPE(N) standard::function<Ret (KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>
#define KAGUYA_CALL_FN_DEF(N) \
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			inline int call(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return push_return_value<Ret>(state,f(KAGUYA_GET_REPEAT(N)));\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>struct is_callable<KAGUYA_FUNC_TYPE(N)> : traits::integral_constant<bool, true> {};\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool checkArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_TYPECHECK_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool strictCheckArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_STRICT_TYPECHECK_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			std::string argTypesName(KAGUYA_FUNC_DEF(N))\
			{\
				return std::string() KAGUYA_PP_REPEAT(N,KAGUYA_TYPENAME_REP);\
			}\
			template<typename Ret KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			int argCount(KAGUYA_FUNC_DEF(N))\
			{\
				return N  KAGUYA_GET_OFFSET;\
			}
			KAGUYA_CALL_FN_DEF(0)
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)

			///! for data member
			//@{
			template<class MemType, class T>
			int call(lua_State* state, MemType T::* m)
			{
				T* this_ = lua_type_traits<T*>::get(state, 1);
				if (lua_gettop(state) == 1)
				{
					if (!this_)
					{
						const T* this_ = lua_type_traits<const T*>::get(state, 1);
						if (!this_)
						{
							throw LuaTypeMismatch("type mismatch!!");
						}
						if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
						{
							return util::push_args(state, standard::reference_wrapper<const MemType>(this_->*m));
						}
						else
						{
							return util::push_args(state, this_->*m);
						}
					}
					else
					{
						if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
						{
							return util::push_args(state, standard::reference_wrapper<MemType>(this_->*m));
						}
						else
						{
							return util::push_args(state, this_->*m);
						}
					}
				}
				else
				{
					if (!this_ || !lua_type_traits<MemType>::checkType(state, 2))
					{
						throw LuaTypeMismatch("type mismatch!!");
					}
					this_->*m = lua_type_traits<MemType>::get(state, 2);
					return 0;
				}
			}
#if defined(_MSC_VER) && _MSC_VER <= 1700
			//can not detect callable at MSVC
#else
			template<class MemType, class T>struct is_callable<MemType T::*> : traits::integral_constant<bool, true> {}; 
#endif

			template<class MemType, class T>
			bool checkArgTypes(lua_State* state, MemType T::* m)
			{
				bool thistypecheck = lua_type_traits<T>::checkType(state, 1);
				if (thistypecheck && lua_gettop(state) == 2)
				{
					return lua_type_traits<MemType>::checkType(state, 2);
				}
				return thistypecheck;
			}
			template<class MemType, class T>
			bool strictCheckArgTypes(lua_State* state, MemType T::* m)
			{
				bool thistypecheck = lua_type_traits<T>::strictCheckType(state, 1);
				if (thistypecheck && lua_gettop(state) == 2)
				{
					return lua_type_traits<MemType>::strictCheckType(state, 2);
				}
				return thistypecheck;
			}
			template<class MemType, class T>
			std::string argTypesName(MemType T::*)
			{
				return std::string(typeid(T*).name()) + ",[OPT] " + typeid(MemType).name();
			}
			template<class MemType, class T>
			int argCount(MemType T::*)
			{
				return 2;
			}
			//@}

			///! for constructor
			//@{
			struct null_type {};

			template<class ClassType, class A1 = null_type, class A2 = null_type
				, class A3 = null_type, class A4 = null_type, class A5 = null_type
				, class A6 = null_type, class A7 = null_type, class A8 = null_type,
			class A9 = null_type>struct constructor_signature_type {
			};

#undef KAGUYA_CALL_FN_DEF
#undef KAGUYA_FUNC_DEF
#undef KAGUYA_FUNC_TYPE
#define KAGUYA_FUNC_DEF(N)  constructor_signature_type<ClassType KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)>
#define KAGUYA_CALL_FN_DEF(N) \
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			inline int call(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				typedef ObjectWrapper<ClassType> wrapper_type;\
				void *storage = lua_newuserdata(state, sizeof(wrapper_type));\
				new(storage) wrapper_type(KAGUYA_GET_REPEAT(N));\
				class_userdata::setmetatable<ClassType>(state);\
				return 1;\
			}\
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>struct is_callable<KAGUYA_FUNC_DEF(N)> : traits::integral_constant<bool, true> {}; \
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool checkArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_TYPECHECK_REP);\
			}\
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			bool strictCheckArgTypes(lua_State* state, KAGUYA_FUNC_DEF(N))\
			{\
				return true KAGUYA_PP_REPEAT(N,KAGUYA_STRICT_TYPECHECK_REP);\
			}\
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			std::string argTypesName(KAGUYA_FUNC_DEF(N))\
			{\
				return std::string() KAGUYA_PP_REPEAT(N,KAGUYA_TYPENAME_REP);\
			}\
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			int argCount(KAGUYA_FUNC_DEF(N))\
			{\
				return N KAGUYA_GET_OFFSET;\
			}
			KAGUYA_CALL_FN_DEF(0)
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_FN_DEF)


				//@}
			template<class ClassType, class FunType=void> struct functionToConstructorSignature;
			
#define KAGUYA_F_TO_CONSIG_TYPE_DEF(N)  constructor_signature_type<ClassType KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)>
#define KAGUYA_F_TO_CONSIG_DEF(N) \
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			struct functionToConstructorSignature<ClassType(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>\
			{\
				typedef KAGUYA_F_TO_CONSIG_TYPE_DEF(N) type;\
			};\
			template<typename ClassType KAGUYA_PP_TEMPLATE_DEF_REPEAT_CONCAT(N)>\
			struct functionToConstructorSignature<ClassType,ClassType(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>\
			{\
				typedef KAGUYA_F_TO_CONSIG_TYPE_DEF(N) type;\
			};

			KAGUYA_F_TO_CONSIG_DEF(0)
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_F_TO_CONSIG_DEF)
		}
		using cpp03impl::call;
		using cpp03impl::checkArgTypes;
		using cpp03impl::strictCheckArgTypes;
		using cpp03impl::argTypesName;
		using cpp03impl::argCount;
		using cpp03impl::constructor_signature_type;
		using cpp03impl::functionToConstructorSignature;
		using cpp03impl::is_callable;
	}
}
