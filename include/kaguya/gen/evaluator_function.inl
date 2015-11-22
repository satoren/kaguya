//generated header by evaluator_function_generator.py
Evaluator operator()()
{
  int previousTop = lua_gettop(state_);
  getTable();
  return Evaluator(state_,0,previousTop,error_handler_);
}
template<typename T1>
Evaluator operator()(T1 t1)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  return Evaluator(state_,1,previousTop,error_handler_);
}
template<typename T1,typename T2>
Evaluator operator()(T1 t1,T2 t2)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  return Evaluator(state_,2,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3>
Evaluator operator()(T1 t1,T2 t2,T3 t3)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  return Evaluator(state_,3,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  return Evaluator(state_,4,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  types::push(state_,static_cast<T5>(t5));
  return Evaluator(state_,5,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  types::push(state_,static_cast<T5>(t5));
  types::push(state_,static_cast<T6>(t6));
  return Evaluator(state_,6,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  types::push(state_,static_cast<T5>(t5));
  types::push(state_,static_cast<T6>(t6));
  types::push(state_,static_cast<T7>(t7));
  return Evaluator(state_,7,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  types::push(state_,static_cast<T5>(t5));
  types::push(state_,static_cast<T6>(t6));
  types::push(state_,static_cast<T7>(t7));
  types::push(state_,static_cast<T8>(t8));
  return Evaluator(state_,8,previousTop,error_handler_);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
Evaluator operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  int previousTop = lua_gettop(state_);
  getTable();
  types::push(state_,static_cast<T1>(t1));
  types::push(state_,static_cast<T2>(t2));
  types::push(state_,static_cast<T3>(t3));
  types::push(state_,static_cast<T4>(t4));
  types::push(state_,static_cast<T5>(t5));
  types::push(state_,static_cast<T6>(t6));
  types::push(state_,static_cast<T7>(t7));
  types::push(state_,static_cast<T8>(t8));
  types::push(state_,static_cast<T9>(t9));
  return Evaluator(state_,9,previousTop,error_handler_);
}
