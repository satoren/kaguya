//generated header by luaref_function_call_generator.py
FunEvaluator LuaRef::operator()()
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  return FunEvaluator(state_,*this,args);
}
template<typename T1>
FunEvaluator LuaRef::operator()(T1 t1)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  args.push_back(LuaRef(state_,standard::forward<T7>(t7)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  args.push_back(LuaRef(state_,standard::forward<T7>(t7)));
  args.push_back(LuaRef(state_,standard::forward<T8>(t8)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){throw LuaTypeMismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  args.push_back(LuaRef(state_,standard::forward<T7>(t7)));
  args.push_back(LuaRef(state_,standard::forward<T8>(t8)));
  args.push_back(LuaRef(state_,standard::forward<T9>(t9)));
  return FunEvaluator(state_,*this,args);
}
