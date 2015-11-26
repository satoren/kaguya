//generated header by luaref_function_call_generator.py
fun_evaluator LuaRef::operator()()
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  return fun_evaluator(state_,*this,args);
}
template<typename T1>
fun_evaluator LuaRef::operator()(T1 t1)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  args.push_back(LuaRef(state_,standard::forward<T7>(t7)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
  std::vector<LuaRef> args;
  args.push_back(LuaRef(state_,standard::forward<T1>(t1)));
  args.push_back(LuaRef(state_,standard::forward<T2>(t2)));
  args.push_back(LuaRef(state_,standard::forward<T3>(t3)));
  args.push_back(LuaRef(state_,standard::forward<T4>(t4)));
  args.push_back(LuaRef(state_,standard::forward<T5>(t5)));
  args.push_back(LuaRef(state_,standard::forward<T6>(t6)));
  args.push_back(LuaRef(state_,standard::forward<T7>(t7)));
  args.push_back(LuaRef(state_,standard::forward<T8>(t8)));
  return fun_evaluator(state_,*this,args);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
fun_evaluator LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
if(type() != TYPE_FUNCTION){throw lua_type_mismatch("is not function");}
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
  return fun_evaluator(state_,*this,args);
}
