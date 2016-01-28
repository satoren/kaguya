//generated header by luaref_function_call_generator.py
inline FunctionResults LuaRef::operator()()
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1>
inline FunctionResults LuaRef::operator()(T1 t1)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(exec_state,standard::forward<T5>(t5));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(exec_state,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(exec_state,standard::forward<T6>(t6));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(exec_state,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(exec_state,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(exec_state,standard::forward<T7>(t7));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(exec_state,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(exec_state,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(exec_state,standard::forward<T7>(t7));
  lua_type_traits<T8>::push(exec_state,standard::forward<T8>(t8));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
inline FunctionResults LuaRef::operator()(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  util::ScopedSavedStack save(state_);
  push(state_);
  int typ = lua_type(state_, -1);
  lua_State* exec_state = state_;
  if(typ == TYPE_FUNCTION){}
  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}
  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}
  int argstart = lua_gettop(exec_state);
  lua_type_traits<T1>::push(exec_state,standard::forward<T1>(t1));
  lua_type_traits<T2>::push(exec_state,standard::forward<T2>(t2));
  lua_type_traits<T3>::push(exec_state,standard::forward<T3>(t3));
  lua_type_traits<T4>::push(exec_state,standard::forward<T4>(t4));
  lua_type_traits<T5>::push(exec_state,standard::forward<T5>(t5));
  lua_type_traits<T6>::push(exec_state,standard::forward<T6>(t6));
  lua_type_traits<T7>::push(exec_state,standard::forward<T7>(t7));
  lua_type_traits<T8>::push(exec_state,standard::forward<T8>(t8));
  lua_type_traits<T9>::push(exec_state,standard::forward<T9>(t9));
  int argnum = lua_gettop(exec_state) - argstart;
  return FunctionResults(funInvoke(exec_state,argnum));
}
