//generated header by luaref_function_call_generator.py
inline FunEvaluator LuaRef::operator()()
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(0);
  return FunEvaluator(state_,*this,args);
}
template<typename T1>
inline FunEvaluator LuaRef::operator()(T1 t1)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(1);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(2);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(3);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(4);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(5);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(6);
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  return FunEvaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(7);
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
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(8);
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
inline FunEvaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  value_type typ = type();
  if(typ != TYPE_FUNCTION && typ != TYPE_THREAD){except::typeMismatchError(state_, "is not function");return FunEvaluator(state_);}
  std::vector<LuaRef> args;
  args.reserve(9);
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
