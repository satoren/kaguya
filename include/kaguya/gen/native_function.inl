//generated header by function_generator.py
template<typename Ret>
struct function_caller0:baseInvoker{
  typedef standard::function<Ret()> func_type;
  func_type func_;
  function_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    Ret r = func_();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret>
baseInvoker* create(standard::function<Ret ()> fun)
{
  typedef function_caller0<Ret> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1>
struct function_caller1:baseInvoker{
  typedef standard::function<Ret(T1)> func_type;
  func_type func_;
  function_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    Ret r = func_(t1);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
baseInvoker* create(standard::function<Ret (T1)> fun)
{
  typedef function_caller1<Ret,T1> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2>
struct function_caller2:baseInvoker{
  typedef standard::function<Ret(T1,T2)> func_type;
  func_type func_;
  function_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    Ret r = func_(t1,t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
baseInvoker* create(standard::function<Ret (T1,T2)> fun)
{
  typedef function_caller2<Ret,T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct function_caller3:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3)> func_type;
  func_type func_;
  function_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    Ret r = func_(t1,t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
baseInvoker* create(standard::function<Ret (T1,T2,T3)> fun)
{
  typedef function_caller3<Ret,T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct function_caller4:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4)> func_type;
  func_type func_;
  function_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    Ret r = func_(t1,t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4)> fun)
{
  typedef function_caller4<Ret,T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct function_caller5:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5)> func_type;
  func_type func_;
  function_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    Ret r = func_(t1,t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5)> fun)
{
  typedef function_caller5<Ret,T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct function_caller6:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6)> func_type;
  func_type func_;
  function_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    Ret r = func_(t1,t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6)> fun)
{
  typedef function_caller6<Ret,T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct function_caller7:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7)> func_type;
  func_type func_;
  function_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7)> fun)
{
  typedef function_caller7<Ret,T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct function_caller8:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7,T8)> func_type;
  func_type func_;
  function_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7,T8)> fun)
{
  typedef function_caller8<Ret,T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct function_caller9:baseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7,T8,T9)> func_type;
  func_type func_;
  function_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7,T8,T9)> fun)
{
  typedef function_caller9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
struct void_function_caller0:baseInvoker{
  typedef standard::function<void()> func_type;
  func_type func_;
  void_function_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    func_();
    return 0;
  }
};
baseInvoker* create(standard::function<void ()> fun)
{
  typedef void_function_caller0 caller_type;
  return new caller_type(fun);
}
template<typename T1>
struct void_function_caller1:baseInvoker{
  typedef standard::function<void(T1)> func_type;
  func_type func_;
  void_function_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    func_(t1);
    return 0;
  }
};
template<typename T1>
baseInvoker* create(standard::function<void (T1)> fun)
{
  typedef void_function_caller1<T1> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2>
struct void_function_caller2:baseInvoker{
  typedef standard::function<void(T1,T2)> func_type;
  func_type func_;
  void_function_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    func_(t1,t2);
    return 0;
  }
};
template<typename T1,typename T2>
baseInvoker* create(standard::function<void (T1,T2)> fun)
{
  typedef void_function_caller2<T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3>
struct void_function_caller3:baseInvoker{
  typedef standard::function<void(T1,T2,T3)> func_type;
  func_type func_;
  void_function_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    func_(t1,t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
baseInvoker* create(standard::function<void (T1,T2,T3)> fun)
{
  typedef void_function_caller3<T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct void_function_caller4:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4)> func_type;
  func_type func_;
  void_function_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    func_(t1,t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(standard::function<void (T1,T2,T3,T4)> fun)
{
  typedef void_function_caller4<T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct void_function_caller5:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5)> func_type;
  func_type func_;
  void_function_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    func_(t1,t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(standard::function<void (T1,T2,T3,T4,T5)> fun)
{
  typedef void_function_caller5<T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct void_function_caller6:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6)> func_type;
  func_type func_;
  void_function_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    func_(t1,t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6)> fun)
{
  typedef void_function_caller6<T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct void_function_caller7:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7)> func_type;
  func_type func_;
  void_function_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    func_(t1,t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7)> fun)
{
  typedef void_function_caller7<T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct void_function_caller8:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7,T8)> func_type;
  func_type func_;
  void_function_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7,T8)> fun)
{
  typedef void_function_caller8<T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct void_function_caller9:baseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7,T8,T9)> func_type;
  func_type func_;
  void_function_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7,T8,T9)> fun)
{
  typedef void_function_caller9<T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename Ret>
struct caller0:baseInvoker{
  typedef Ret (*func_type)() ;
  func_type func_;
  caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    Ret r = func_();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret>
baseInvoker* create(Ret (*fun)())
{
  typedef caller0<Ret> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1>
struct caller1:baseInvoker{
  typedef Ret (*func_type)(T1) ;
  func_type func_;
  caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    Ret r = func_(t1);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
baseInvoker* create(Ret (*fun)(T1))
{
  typedef caller1<Ret,T1> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2>
struct caller2:baseInvoker{
  typedef Ret (*func_type)(T1,T2) ;
  func_type func_;
  caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    Ret r = func_(t1,t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
baseInvoker* create(Ret (*fun)(T1,T2))
{
  typedef caller2<Ret,T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct caller3:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3) ;
  func_type func_;
  caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    Ret r = func_(t1,t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
baseInvoker* create(Ret (*fun)(T1,T2,T3))
{
  typedef caller3<Ret,T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct caller4:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4) ;
  func_type func_;
  caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    Ret r = func_(t1,t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4))
{
  typedef caller4<Ret,T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct caller5:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5) ;
  func_type func_;
  caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    Ret r = func_(t1,t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5))
{
  typedef caller5<Ret,T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct caller6:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6) ;
  func_type func_;
  caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    Ret r = func_(t1,t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6))
{
  typedef caller6<Ret,T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct caller7:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7))
{
  typedef caller7<Ret,T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct caller8:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7,T8))
{
  typedef caller8<Ret,T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct caller9:baseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef caller9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
struct void_caller0:baseInvoker{
  typedef void (*func_type)() ;
  func_type func_;
  void_caller0(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    func_();
    return 0;
  }
};
baseInvoker* create(void (*fun)())
{
  typedef void_caller0 caller_type;
  return new caller_type(fun);
}
template<typename T1>
struct void_caller1:baseInvoker{
  typedef void (*func_type)(T1) ;
  func_type func_;
  void_caller1(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    func_(t1);
    return 0;
  }
};
template<typename T1>
baseInvoker* create(void (*fun)(T1))
{
  typedef void_caller1<T1> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2>
struct void_caller2:baseInvoker{
  typedef void (*func_type)(T1,T2) ;
  func_type func_;
  void_caller2(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    func_(t1,t2);
    return 0;
  }
};
template<typename T1,typename T2>
baseInvoker* create(void (*fun)(T1,T2))
{
  typedef void_caller2<T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3>
struct void_caller3:baseInvoker{
  typedef void (*func_type)(T1,T2,T3) ;
  func_type func_;
  void_caller3(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    func_(t1,t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
baseInvoker* create(void (*fun)(T1,T2,T3))
{
  typedef void_caller3<T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct void_caller4:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4) ;
  func_type func_;
  void_caller4(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    func_(t1,t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(void (*fun)(T1,T2,T3,T4))
{
  typedef void_caller4<T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct void_caller5:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5) ;
  func_type func_;
  void_caller5(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    func_(t1,t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(void (*fun)(T1,T2,T3,T4,T5))
{
  typedef void_caller5<T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct void_caller6:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6) ;
  func_type func_;
  void_caller6(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    func_(t1,t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6))
{
  typedef void_caller6<T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct void_caller7:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  void_caller7(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    func_(t1,t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7))
{
  typedef void_caller7<T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct void_caller8:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  void_caller8(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7,T8))
{
  typedef void_caller8<T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct void_caller9:baseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  void_caller9(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef void_caller9<T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1>
struct mem_caller0:baseInvoker{
  typedef Ret (T1::*func_type)() ;
  func_type func_;
  mem_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
baseInvoker* create(Ret (T1::*fun)())
{
  typedef mem_caller0<Ret,T1> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1>
struct constmem_caller0:baseInvoker{
  typedef Ret (T1::*func_type)() const;
  func_type func_;
  constmem_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
baseInvoker* create(Ret (T1::*fun)()const)
{
  typedef constmem_caller0<Ret,T1> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2>
struct mem_caller1:baseInvoker{
  typedef Ret (T1::*func_type)(T2) ;
  func_type func_;
  mem_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
baseInvoker* create(Ret (T1::*fun)(T2))
{
  typedef mem_caller1<Ret,T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2>
struct constmem_caller1:baseInvoker{
  typedef Ret (T1::*func_type)(T2) const;
  func_type func_;
  constmem_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
baseInvoker* create(Ret (T1::*fun)(T2)const)
{
  typedef constmem_caller1<Ret,T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct mem_caller2:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3) ;
  func_type func_;
  mem_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
baseInvoker* create(Ret (T1::*fun)(T2,T3))
{
  typedef mem_caller2<Ret,T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct constmem_caller2:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3) const;
  func_type func_;
  constmem_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
baseInvoker* create(Ret (T1::*fun)(T2,T3)const)
{
  typedef constmem_caller2<Ret,T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct mem_caller3:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4) ;
  func_type func_;
  mem_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4))
{
  typedef mem_caller3<Ret,T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct constmem_caller3:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4) const;
  func_type func_;
  constmem_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4)const)
{
  typedef constmem_caller3<Ret,T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct mem_caller4:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5) ;
  func_type func_;
  mem_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5))
{
  typedef mem_caller4<Ret,T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct constmem_caller4:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5) const;
  func_type func_;
  constmem_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5)const)
{
  typedef constmem_caller4<Ret,T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct mem_caller5:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6) ;
  func_type func_;
  mem_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6))
{
  typedef mem_caller5<Ret,T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct constmem_caller5:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6) const;
  func_type func_;
  constmem_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6)const)
{
  typedef constmem_caller5<Ret,T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct mem_caller6:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  mem_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7))
{
  typedef mem_caller6<Ret,T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct constmem_caller6:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7) const;
  func_type func_;
  constmem_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7)const)
{
  typedef constmem_caller6<Ret,T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct mem_caller7:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  mem_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8))
{
  typedef mem_caller7<Ret,T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct constmem_caller7:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) const;
  func_type func_;
  constmem_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8)const)
{
  typedef constmem_caller7<Ret,T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct mem_caller8:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  mem_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef mem_caller8<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct constmem_caller8:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) const;
  func_type func_;
  constmem_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9)const)
{
  typedef constmem_caller8<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct mem_caller9:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) ;
  func_type func_;
  mem_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())|| !types::strict_check_type(state,10,types::type_tag<T10>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())|| !types::check_type(state,10,types::type_tag<T10>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    T10 t10 = types::get(state,10,types::type_tag<T10>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10))
{
  typedef mem_caller9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> caller_type;
  return new caller_type(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct constmem_caller9:baseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) const;
  func_type func_;
  constmem_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())|| !types::strict_check_type(state,10,types::type_tag<T10>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())|| !types::check_type(state,10,types::type_tag<T10>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    T10 t10 = types::get(state,10,types::type_tag<T10>());
  T1* ptr = getPtr(state);
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
baseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10)const)
{
  typedef constmem_caller9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> caller_type;
  return new caller_type(fun);
}
template<typename T1>
struct void_mem_caller0:baseInvoker{
  typedef void (T1::*func_type)() ;
  func_type func_;
  void_mem_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = getPtr(state);
    (ptr->*func_)();
    return 0;
  }
};
template<typename T1>
baseInvoker* create(void (T1::*fun)())
{
  typedef void_mem_caller0<T1> caller_type;
  return new caller_type(fun);
}
template<typename T1>
struct constvoid_mem_caller0:baseInvoker{
  typedef void (T1::*func_type)() const;
  func_type func_;
  constvoid_mem_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = getPtr(state);
    (ptr->*func_)();
    return 0;
  }
};
template<typename T1>
baseInvoker* create(void (T1::*fun)()const)
{
  typedef constvoid_mem_caller0<T1> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2>
struct void_mem_caller1:baseInvoker{
  typedef void (T1::*func_type)(T2) ;
  func_type func_;
  void_mem_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2);
    return 0;
  }
};
template<typename T1,typename T2>
baseInvoker* create(void (T1::*fun)(T2))
{
  typedef void_mem_caller1<T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2>
struct constvoid_mem_caller1:baseInvoker{
  typedef void (T1::*func_type)(T2) const;
  func_type func_;
  constvoid_mem_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2);
    return 0;
  }
};
template<typename T1,typename T2>
baseInvoker* create(void (T1::*fun)(T2)const)
{
  typedef constvoid_mem_caller1<T1,T2> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3>
struct void_mem_caller2:baseInvoker{
  typedef void (T1::*func_type)(T2,T3) ;
  func_type func_;
  void_mem_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
baseInvoker* create(void (T1::*fun)(T2,T3))
{
  typedef void_mem_caller2<T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3>
struct constvoid_mem_caller2:baseInvoker{
  typedef void (T1::*func_type)(T2,T3) const;
  func_type func_;
  constvoid_mem_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
baseInvoker* create(void (T1::*fun)(T2,T3)const)
{
  typedef constvoid_mem_caller2<T1,T2,T3> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct void_mem_caller3:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4) ;
  func_type func_;
  void_mem_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(void (T1::*fun)(T2,T3,T4))
{
  typedef void_mem_caller3<T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct constvoid_mem_caller3:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4) const;
  func_type func_;
  constvoid_mem_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
baseInvoker* create(void (T1::*fun)(T2,T3,T4)const)
{
  typedef constvoid_mem_caller3<T1,T2,T3,T4> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct void_mem_caller4:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5) ;
  func_type func_;
  void_mem_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5))
{
  typedef void_mem_caller4<T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct constvoid_mem_caller4:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5) const;
  func_type func_;
  constvoid_mem_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5)const)
{
  typedef constvoid_mem_caller4<T1,T2,T3,T4,T5> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct void_mem_caller5:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6) ;
  func_type func_;
  void_mem_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6))
{
  typedef void_mem_caller5<T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct constvoid_mem_caller5:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6) const;
  func_type func_;
  constvoid_mem_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6)const)
{
  typedef constvoid_mem_caller5<T1,T2,T3,T4,T5,T6> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct void_mem_caller6:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  void_mem_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7))
{
  typedef void_mem_caller6<T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct constvoid_mem_caller6:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7) const;
  func_type func_;
  constvoid_mem_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7)const)
{
  typedef constvoid_mem_caller6<T1,T2,T3,T4,T5,T6,T7> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct void_mem_caller7:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  void_mem_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8))
{
  typedef void_mem_caller7<T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct constvoid_mem_caller7:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) const;
  func_type func_;
  constvoid_mem_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8)const)
{
  typedef constvoid_mem_caller7<T1,T2,T3,T4,T5,T6,T7,T8> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct void_mem_caller8:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  void_mem_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef void_mem_caller8<T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct constvoid_mem_caller8:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) const;
  func_type func_;
  constvoid_mem_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9)const)
{
  typedef constvoid_mem_caller8<T1,T2,T3,T4,T5,T6,T7,T8,T9> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct void_mem_caller9:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) ;
  func_type func_;
  void_mem_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())|| !types::strict_check_type(state,10,types::type_tag<T10>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())|| !types::check_type(state,10,types::type_tag<T10>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    T10 t10 = types::get(state,10,types::type_tag<T10>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10))
{
  typedef void_mem_caller9<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> caller_type;
  return new caller_type(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct constvoid_mem_caller9:baseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) const;
  func_type func_;
  constvoid_mem_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(getPtr(state) == 0){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())|| !types::strict_check_type(state,10,types::type_tag<T10>())){return false;}
  }else{
      if(false|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())|| !types::check_type(state,10,types::type_tag<T10>())){return false;}
  }
    return true;
  }
  T1* getPtr(lua_State *state) {
    if (types::check_type(state, 1, types::type_tag<T1*>()))
    {
      return types::get(state, 1, types::type_tag<T1*>());
   }
   if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<T1>*>()))
    {
      standard::shared_ptr<T1>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<T1>*>());
      if (shared_ptr) {
        return shared_ptr->get();
      }
    }
    return 0;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
    T10 t10 = types::get(state,10,types::type_tag<T10>());
  T1* ptr = getPtr(state);
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
baseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10)const)
{
  typedef constvoid_mem_caller9<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> caller_type;
  return new caller_type(fun);
}
template<typename CLASS>
struct constructor_caller0:baseInvoker{
  constructor_caller0(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    if(strictcheck){
      if(false){return false;}
  }else{
      if(false){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1>
struct constructor_caller1:baseInvoker{
  constructor_caller1(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2>
struct constructor_caller2:baseInvoker{
  constructor_caller2(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3>
struct constructor_caller3:baseInvoker{
  constructor_caller3(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4>
struct constructor_caller4:baseInvoker{
  constructor_caller4(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5>
struct constructor_caller5:baseInvoker{
  constructor_caller5(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4,t5);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct constructor_caller6:baseInvoker{
  constructor_caller6(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct constructor_caller7:baseInvoker{
  constructor_caller7(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct constructor_caller8:baseInvoker{
  constructor_caller8(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7,t8);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct constructor_caller9:baseInvoker{
  constructor_caller9(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(false|| !types::strict_check_type(state,1,types::type_tag<T1>())|| !types::strict_check_type(state,2,types::type_tag<T2>())|| !types::strict_check_type(state,3,types::type_tag<T3>())|| !types::strict_check_type(state,4,types::type_tag<T4>())|| !types::strict_check_type(state,5,types::type_tag<T5>())|| !types::strict_check_type(state,6,types::type_tag<T6>())|| !types::strict_check_type(state,7,types::type_tag<T7>())|| !types::strict_check_type(state,8,types::type_tag<T8>())|| !types::strict_check_type(state,9,types::type_tag<T9>())){return false;}
  }else{
      if(false|| !types::check_type(state,1,types::type_tag<T1>())|| !types::check_type(state,2,types::type_tag<T2>())|| !types::check_type(state,3,types::type_tag<T3>())|| !types::check_type(state,4,types::type_tag<T4>())|| !types::check_type(state,5,types::type_tag<T5>())|| !types::check_type(state,6,types::type_tag<T6>())|| !types::check_type(state,7,types::type_tag<T7>())|| !types::check_type(state,8,types::type_tag<T8>())|| !types::check_type(state,9,types::type_tag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::type_tag<T1>());
    T2 t2 = types::get(state,2,types::type_tag<T2>());
    T3 t3 = types::get(state,3,types::type_tag<T3>());
    T4 t4 = types::get(state,4,types::type_tag<T4>());
    T5 t5 = types::get(state,5,types::type_tag<T5>());
    T6 t6 = types::get(state,6,types::type_tag<T6>());
    T7 t7 = types::get(state,7,types::type_tag<T7>());
    T8 t8 = types::get(state,8,types::type_tag<T8>());
    T9 t9 = types::get(state,9,types::type_tag<T9>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    CLASS* ptr = types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7,t8,t9);
    luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
    return 1;
  }
};
