template<typename RetType>
RetType returnvalue_(lua_State* state, int retindex, types::typetag<RetType> tag)
{
	return FunctionResults(state, retindex);
}
void returnvalue_(lua_State* state, int retindex, types::typetag<void> tag)
{
}

#if KAGUYA_USE_CPP11
template<class Result, class...Args> Result call(Args&&... args)
{
	int argstart = lua_gettop(state_) + 1;
	push(state_);
	util::push_args(state_, standard::forward<Args>(args)...);
	int argnum = lua_gettop(state_) - argstart;
	int result = lua_pcall(state_, argnum, LUA_MULTRET, 0);
	except::checkErrorAndThrow(result, state_);
	return returnvalue_(state_, argstart, types::typetag<Result>());
}
template<class Result, class...Args> Result resume(Args&&... args)
{
	lua_State* cor = get<lua_State*>();
	int argstart = lua_gettop(cor) + 1;
	if (argstart > 1 && lua_status(cor) != LUA_YIELD)
	{
		argstart -= 1;
	}
	util::push_args(cor, standard::forward<Args>(args)...);
	int argnum = lua_gettop(cor) - argstart;
	int result = util::lua_resume_compat(cor, argnum);
	except::checkErrorAndThrow(result, cor);
	return returnvalue_(cor, argstart, types::typetag<Result>());
}

template<class...Args> FunctionResults operator()(Args&&... args)
{
	int t = type();
	if (t == LUA_TTHREAD)
	{
		return resume<FunctionResults>(standard::forward<Args>(args)...);
	}
	else if (t == LUA_TFUNCTION)
	{
		return call<FunctionResults>(standard::forward<Args>(args)...);
	}
	else
	{
		except::typeMismatchError(state_, " is not function or thread");
		return FunctionResults(state_);
	}
}
#else

#define KAGUYA_PP_TEMPLATE(N) ,KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) KAGUYA_PP_CAT(A,N) KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_ARG_DEF(N) ,standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N))
#define KAGUYA_CALL_DEF(N) \
		template<class Result KAGUYA_PP_REPEAT(N,KAGUYA_PP_TEMPLATE)>\
		Result call(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			int argstart = lua_gettop(state_) + 1;\
			push(state_);\
			util::push_args(state_ KAGUYA_PP_REPEAT(N,KAGUYA_PUSH_ARG_DEF));\
			int argnum = lua_gettop(state_) - argstart;\
			int result = lua_pcall(state_, argnum, LUA_MULTRET, 0);\
			except::checkErrorAndThrow(result, state_);\
			return returnvalue_(state_, argstart, types::typetag<Result>());\
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
			util::push_args(cor KAGUYA_PP_REPEAT(N, KAGUYA_PUSH_ARG_DEF));\
			int argnum = lua_gettop(cor) - argstart;\
			int result = util::lua_resume_compat(cor, argnum);\
			except::checkErrorAndThrow(result, cor);\
			return returnvalue_(cor, argstart, types::typetag<Result>());\
		}

KAGUYA_RESUME_DEF(0)
KAGUYA_PP_REPEAT_DEF(9, KAGUYA_RESUME_DEF)

#undef KAGUYA_PUSH_DEF
#undef KAGUYA_PUSH_ARG_DEF
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_RESUME_DEF

#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_FUNCTION_ARGS_DEF(N)
#define KAGUYA_CALL_ARGS(N)

#define KAGUYA_OP_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline FunctionResults operator()(KAGUYA_FUNCTION_ARGS_DEF(N))\
	{\
		int t = type(); \
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
			except::typeMismatchError(state_, " is not function or thread"); \
			return FunctionResults(state_); \
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
#define KAGUYA_PUSH_ARG_DEF(N) standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N)) 

KAGUYA_PP_REPEAT_DEF(9, KAGUYA_OP_FN_DEF)
#undef KAGUYA_OP_FN_DEF
#undef KAGUYA_TEMPLATE_PARAMETER

#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_CALL_DEF
#undef KAGUYA_OP_FN_DEF
//@}
#endif