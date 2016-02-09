#ifndef KAGUYA_DELEGATE_FIRST_ARG
#define KAGUYA_DELEGATE_FIRST_ARG
#define KAGUYA_DELEGATE_FIRST_ARG_C
#else
#define KAGUYA_DELEGATE_FIRST_ARG_C KAGUYA_DELEGATE_FIRST_ARG,
#endif

#if KAGUYA_USE_CPP11
		template<class... Args>
		FunctionResults operator()(Args&&... args)
		{
			return KAGUYA_DELEGATE_LUAREF(KAGUYA_DELEGATE_FIRST_ARG_C standard::forward<Args>(args)...);
		}

		template<class Result, class... Args>
		Result call(Args&&... args)
		{
			return KAGUYA_DELEGATE_LUAREF.template call<Result>(KAGUYA_DELEGATE_FIRST_ARG_C standard::forward<Args>(args)...);
		}
		template<class Result, class... Args>
		Result resume(Args&&... args)
		{
			return KAGUYA_DELEGATE_LUAREF.template resume<Result>(KAGUYA_DELEGATE_FIRST_ARG_C standard::forward<Args>(args)...);
		}

#else

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) const KAGUYA_PP_CAT(A,N)& KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_ARG_DEF(N) standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N))
#define KAGUYA_OP_FN_DEF(N) \
		template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)> \
		FunctionResults operator()(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			return KAGUYA_DELEGATE_LUAREF(KAGUYA_DELEGATE_FIRST_ARG_C KAGUYA_PP_REPEAT_ARG(N, KAGUYA_PUSH_ARG_DEF));\
		}

		/**
		* @name operator()
		* @brief If type is function, call lua function.
		If type is lua thread,start or resume lua thread.
		Otherwise send error message to error handler
		* @param arg... function args
		*/
		//@{
		FunctionResults operator()()
		{
			return KAGUYA_DELEGATE_LUAREF(KAGUYA_DELEGATE_FIRST_ARG);
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
		//@}

#define KAGUYA_CALL_DEF(N) \
		template<class Result,KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)> \
		Result call(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			return KAGUYA_DELEGATE_LUAREF.template call<Result>(KAGUYA_DELEGATE_FIRST_ARG_C KAGUYA_PP_REPEAT_ARG(N, KAGUYA_PUSH_ARG_DEF));\
		}
#define KAGUYA_RESUME_DEF(N) \
		template<class Result,KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)> \
		Result resume(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			return KAGUYA_DELEGATE_LUAREF.template resume<Result>(KAGUYA_DELEGATE_FIRST_ARG_C KAGUYA_PP_REPEAT_ARG(N, KAGUYA_PUSH_ARG_DEF));\
		}

		template<class Result>
		Result call()
		{
			return KAGUYA_DELEGATE_LUAREF.template call<Result>(KAGUYA_DELEGATE_FIRST_ARG);
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_CALL_DEF)

		template<class Result>
		Result resume()
		{
			return KAGUYA_DELEGATE_LUAREF.template resume<Result>(KAGUYA_DELEGATE_FIRST_ARG);
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_RESUME_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_PUSH_ARG_DEF
#undef KAGUYA_OP_FN_DEF
#undef KAGUYA_CALL_DEF
#undef KAGUYA_RESUME_DEF
#endif
			
#undef KAGUYA_DELEGATE_FIRST_ARG
#undef KAGUYA_DELEGATE_FIRST_ARG_C

