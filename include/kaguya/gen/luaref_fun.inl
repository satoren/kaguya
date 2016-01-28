//generated header by luaref_function_call_generator.py
inline FunEvaluator LuaRef::operator()()
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1>
inline FunEvaluator LuaRef::operator()(T1 t1)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(state_,standard::forward<T5>(t5));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(state_,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(state_,standard::forward<T6>(t6));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(state_,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(state_,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(state_,standard::forward<T7>(t7));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(state_,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(state_,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(state_,standard::forward<T7>(t7));
  lua_type_traits<T8>::push(state_,standard::forward<T8>(t8));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  int argstart = lua_gettop(state_);
  lua_type_traits<T1>::push(state_,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(state_,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(state_,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(state_,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(state_,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(state_,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(state_,standard::forward<T7>(t7));
  lua_type_traits<T8>::push(state_,standard::forward<T8>(t8));
  lua_type_traits<T9>::push(state_,standard::forward<T9>(t9));
  int argnum = lua_gettop(state_) - argstart;
  std::vector<LuaRef> args;
  args.reserve(argnum);
  for (int i = 0; i < argnum; ++i)
    args.push_back(LuaRef(state_, StackTop()));
  std::reverse(args.begin(), args.end());
#if KAGUYA_USE_RVALUE_REFERENCE
  return FunEvaluator(state_,*this,std::move(args));
#else
  return FunEvaluator(state_,*this,args);
#endif
}
