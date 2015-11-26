//generated header by function_generator.py
template<typename Ret>
struct function_caller0:BaseInvoker{
  typedef standard::function<Ret()> func_type;
  func_type func_;
  function_caller0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    Ret r = func_();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret>
BaseInvoker* create(standard::function<Ret ()> fun)
{
  typedef function_caller0<Ret> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1>
struct function_caller1:BaseInvoker{
  typedef standard::function<Ret(T1)> func_type;
  func_type func_;
  function_caller1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    Ret r = func_(t1);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
BaseInvoker* create(standard::function<Ret (T1)> fun)
{
  typedef function_caller1<Ret,T1> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2>
struct function_caller2:BaseInvoker{
  typedef standard::function<Ret(T1,T2)> func_type;
  func_type func_;
  function_caller2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    Ret r = func_(t1,t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
BaseInvoker* create(standard::function<Ret (T1,T2)> fun)
{
  typedef function_caller2<Ret,T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct function_caller3:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3)> func_type;
  func_type func_;
  function_caller3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    Ret r = func_(t1,t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
BaseInvoker* create(standard::function<Ret (T1,T2,T3)> fun)
{
  typedef function_caller3<Ret,T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct function_caller4:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4)> func_type;
  func_type func_;
  function_caller4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    Ret r = func_(t1,t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4)> fun)
{
  typedef function_caller4<Ret,T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct function_caller5:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5)> func_type;
  func_type func_;
  function_caller5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    Ret r = func_(t1,t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5)> fun)
{
  typedef function_caller5<Ret,T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct function_caller6:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6)> func_type;
  func_type func_;
  function_caller6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    Ret r = func_(t1,t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6)> fun)
{
  typedef function_caller6<Ret,T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct function_caller7:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7)> func_type;
  func_type func_;
  function_caller7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7)> fun)
{
  typedef function_caller7<Ret,T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct function_caller8:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7,T8)> func_type;
  func_type func_;
  function_caller8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7,T8)> fun)
{
  typedef function_caller8<Ret,T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct function_caller9:BaseInvoker{
  typedef standard::function<Ret(T1,T2,T3,T4,T5,T6,T7,T8,T9)> func_type;
  func_type func_;
  function_caller9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(standard::function<Ret (T1,T2,T3,T4,T5,T6,T7,T8,T9)> fun)
{
  typedef function_caller9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
struct VoidFunctionInvoker0:BaseInvoker{
  typedef standard::function<void()> func_type;
  func_type func_;
  VoidFunctionInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    func_();
    return 0;
  }
};
BaseInvoker* create(standard::function<void ()> fun)
{
  typedef VoidFunctionInvoker0 InvokerType;
  return new InvokerType(fun);
}
template<typename T1>
struct VoidFunctionInvoker1:BaseInvoker{
  typedef standard::function<void(T1)> func_type;
  func_type func_;
  VoidFunctionInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    func_(t1);
    return 0;
  }
};
template<typename T1>
BaseInvoker* create(standard::function<void (T1)> fun)
{
  typedef VoidFunctionInvoker1<T1> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2>
struct VoidFunctionInvoker2:BaseInvoker{
  typedef standard::function<void(T1,T2)> func_type;
  func_type func_;
  VoidFunctionInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    func_(t1,t2);
    return 0;
  }
};
template<typename T1,typename T2>
BaseInvoker* create(standard::function<void (T1,T2)> fun)
{
  typedef VoidFunctionInvoker2<T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3>
struct VoidFunctionInvoker3:BaseInvoker{
  typedef standard::function<void(T1,T2,T3)> func_type;
  func_type func_;
  VoidFunctionInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    func_(t1,t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
BaseInvoker* create(standard::function<void (T1,T2,T3)> fun)
{
  typedef VoidFunctionInvoker3<T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct VoidFunctionInvoker4:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4)> func_type;
  func_type func_;
  VoidFunctionInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    func_(t1,t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4)> fun)
{
  typedef VoidFunctionInvoker4<T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct VoidFunctionInvoker5:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5)> func_type;
  func_type func_;
  VoidFunctionInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    func_(t1,t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4,T5)> fun)
{
  typedef VoidFunctionInvoker5<T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct VoidFunctionInvoker6:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6)> func_type;
  func_type func_;
  VoidFunctionInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    func_(t1,t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6)> fun)
{
  typedef VoidFunctionInvoker6<T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct VoidFunctionInvoker7:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7)> func_type;
  func_type func_;
  VoidFunctionInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    func_(t1,t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7)> fun)
{
  typedef VoidFunctionInvoker7<T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct VoidFunctionInvoker8:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7,T8)> func_type;
  func_type func_;
  VoidFunctionInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7,T8)> fun)
{
  typedef VoidFunctionInvoker8<T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct VoidFunctionInvoker9:BaseInvoker{
  typedef standard::function<void(T1,T2,T3,T4,T5,T6,T7,T8,T9)> func_type;
  func_type func_;
  VoidFunctionInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(standard::function<void (T1,T2,T3,T4,T5,T6,T7,T8,T9)> fun)
{
  typedef VoidFunctionInvoker9<T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret>
struct FunInvoker0:BaseInvoker{
  typedef Ret (*func_type)() ;
  func_type func_;
  FunInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    Ret r = func_();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret>
BaseInvoker* create(Ret (*fun)())
{
  typedef FunInvoker0<Ret> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1>
struct FunInvoker1:BaseInvoker{
  typedef Ret (*func_type)(T1) ;
  func_type func_;
  FunInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    Ret r = func_(t1);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
BaseInvoker* create(Ret (*fun)(T1))
{
  typedef FunInvoker1<Ret,T1> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2>
struct FunInvoker2:BaseInvoker{
  typedef Ret (*func_type)(T1,T2) ;
  func_type func_;
  FunInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    Ret r = func_(t1,t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
BaseInvoker* create(Ret (*fun)(T1,T2))
{
  typedef FunInvoker2<Ret,T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct FunInvoker3:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3) ;
  func_type func_;
  FunInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    Ret r = func_(t1,t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
BaseInvoker* create(Ret (*fun)(T1,T2,T3))
{
  typedef FunInvoker3<Ret,T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct FunInvoker4:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4) ;
  func_type func_;
  FunInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    Ret r = func_(t1,t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4))
{
  typedef FunInvoker4<Ret,T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct FunInvoker5:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5) ;
  func_type func_;
  FunInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    Ret r = func_(t1,t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5))
{
  typedef FunInvoker5<Ret,T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct FunInvoker6:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6) ;
  func_type func_;
  FunInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    Ret r = func_(t1,t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6))
{
  typedef FunInvoker6<Ret,T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct FunInvoker7:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  FunInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7))
{
  typedef FunInvoker7<Ret,T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct FunInvoker8:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  FunInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7,T8))
{
  typedef FunInvoker8<Ret,T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct FunInvoker9:BaseInvoker{
  typedef Ret (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  FunInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    Ret r = func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(Ret (*fun)(T1,T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef FunInvoker9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
struct VoidFunInvoker0:BaseInvoker{
  typedef void (*func_type)() ;
  func_type func_;
  VoidFunInvoker0(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    func_();
    return 0;
  }
};
BaseInvoker* create(void (*fun)())
{
  typedef VoidFunInvoker0 InvokerType;
  return new InvokerType(fun);
}
template<typename T1>
struct VoidFunInvoker1:BaseInvoker{
  typedef void (*func_type)(T1) ;
  func_type func_;
  VoidFunInvoker1(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    func_(t1);
    return 0;
  }
};
template<typename T1>
BaseInvoker* create(void (*fun)(T1))
{
  typedef VoidFunInvoker1<T1> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2>
struct VoidFunInvoker2:BaseInvoker{
  typedef void (*func_type)(T1,T2) ;
  func_type func_;
  VoidFunInvoker2(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    func_(t1,t2);
    return 0;
  }
};
template<typename T1,typename T2>
BaseInvoker* create(void (*fun)(T1,T2))
{
  typedef VoidFunInvoker2<T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3>
struct VoidFunInvoker3:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3) ;
  func_type func_;
  VoidFunInvoker3(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    func_(t1,t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
BaseInvoker* create(void (*fun)(T1,T2,T3))
{
  typedef VoidFunInvoker3<T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct VoidFunInvoker4:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4) ;
  func_type func_;
  VoidFunInvoker4(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    func_(t1,t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4))
{
  typedef VoidFunInvoker4<T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct VoidFunInvoker5:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5) ;
  func_type func_;
  VoidFunInvoker5(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    func_(t1,t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4,T5))
{
  typedef VoidFunInvoker5<T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct VoidFunInvoker6:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6) ;
  func_type func_;
  VoidFunInvoker6(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    func_(t1,t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6))
{
  typedef VoidFunInvoker6<T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct VoidFunInvoker7:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  VoidFunInvoker7(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    func_(t1,t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7))
{
  typedef VoidFunInvoker7<T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct VoidFunInvoker8:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  VoidFunInvoker8(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7,T8))
{
  typedef VoidFunInvoker8<T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct VoidFunInvoker9:BaseInvoker{
  typedef void (*func_type)(T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  VoidFunInvoker9(func_type fun):func_(fun){}  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    func_(t1,t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(void (*fun)(T1,T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef VoidFunInvoker9<T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1>
struct MemFunInvoker0:BaseInvoker{
  typedef Ret (T1::*func_type)() ;
  func_type func_;
  MemFunInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
BaseInvoker* create(Ret (T1::*fun)())
{
  typedef MemFunInvoker0<Ret,T1> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1>
struct constMemFunInvoker0:BaseInvoker{
  typedef Ret (T1::*func_type)() const;
  func_type func_;
  constMemFunInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)();
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1>
BaseInvoker* create(Ret (T1::*fun)()const)
{
  typedef constMemFunInvoker0<Ret,T1> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2>
struct MemFunInvoker1:BaseInvoker{
  typedef Ret (T1::*func_type)(T2) ;
  func_type func_;
  MemFunInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
BaseInvoker* create(Ret (T1::*fun)(T2))
{
  typedef MemFunInvoker1<Ret,T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2>
struct constMemFunInvoker1:BaseInvoker{
  typedef Ret (T1::*func_type)(T2) const;
  func_type func_;
  constMemFunInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2>
BaseInvoker* create(Ret (T1::*fun)(T2)const)
{
  typedef constMemFunInvoker1<Ret,T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct MemFunInvoker2:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3) ;
  func_type func_;
  MemFunInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
BaseInvoker* create(Ret (T1::*fun)(T2,T3))
{
  typedef MemFunInvoker2<Ret,T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3>
struct constMemFunInvoker2:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3) const;
  func_type func_;
  constMemFunInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3>
BaseInvoker* create(Ret (T1::*fun)(T2,T3)const)
{
  typedef constMemFunInvoker2<Ret,T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct MemFunInvoker3:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4) ;
  func_type func_;
  MemFunInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4))
{
  typedef MemFunInvoker3<Ret,T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
struct constMemFunInvoker3:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4) const;
  func_type func_;
  constMemFunInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4)const)
{
  typedef constMemFunInvoker3<Ret,T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct MemFunInvoker4:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5) ;
  func_type func_;
  MemFunInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5))
{
  typedef MemFunInvoker4<Ret,T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
struct constMemFunInvoker4:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5) const;
  func_type func_;
  constMemFunInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5)const)
{
  typedef constMemFunInvoker4<Ret,T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct MemFunInvoker5:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6) ;
  func_type func_;
  MemFunInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6))
{
  typedef MemFunInvoker5<Ret,T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct constMemFunInvoker5:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6) const;
  func_type func_;
  constMemFunInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6)const)
{
  typedef constMemFunInvoker5<Ret,T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct MemFunInvoker6:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  MemFunInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7))
{
  typedef MemFunInvoker6<Ret,T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct constMemFunInvoker6:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7) const;
  func_type func_;
  constMemFunInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7)const)
{
  typedef constMemFunInvoker6<Ret,T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct MemFunInvoker7:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  MemFunInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8))
{
  typedef MemFunInvoker7<Ret,T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct constMemFunInvoker7:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) const;
  func_type func_;
  constMemFunInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8)const)
{
  typedef constMemFunInvoker7<Ret,T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct MemFunInvoker8:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  MemFunInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef MemFunInvoker8<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct constMemFunInvoker8:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) const;
  func_type func_;
  constMemFunInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9)const)
{
  typedef constMemFunInvoker8<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct MemFunInvoker9:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) ;
  func_type func_;
  MemFunInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())||!types::strictCheckType(state,10,types::typetag<T10>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())||!types::checkType(state,10,types::typetag<T10>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    T10 t10 = types::get(state,10,types::typetag<T10>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10))
{
  typedef MemFunInvoker9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> InvokerType;
  return new InvokerType(fun);
}
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct constMemFunInvoker9:BaseInvoker{
  typedef Ret (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) const;
  func_type func_;
  constMemFunInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())||!types::strictCheckType(state,10,types::typetag<T10>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())||!types::checkType(state,10,types::typetag<T10>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    T10 t10 = types::get(state,10,types::typetag<T10>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    Ret r = (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return types::push(state,standard::forward<Ret>(r));
  }
};
template<typename Ret,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
BaseInvoker* create(Ret (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10)const)
{
  typedef constMemFunInvoker9<Ret,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> InvokerType;
  return new InvokerType(fun);
}
template<typename T1>
struct VoidMemFunInvoker0:BaseInvoker{
  typedef void (T1::*func_type)() ;
  func_type func_;
  VoidMemFunInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)();
    return 0;
  }
};
template<typename T1>
BaseInvoker* create(void (T1::*fun)())
{
  typedef VoidMemFunInvoker0<T1> InvokerType;
  return new InvokerType(fun);
}
template<typename T1>
struct constVoidMemFunInvoker0:BaseInvoker{
  typedef void (T1::*func_type)() const;
  func_type func_;
  constVoidMemFunInvoker0(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)();
    return 0;
  }
};
template<typename T1>
BaseInvoker* create(void (T1::*fun)()const)
{
  typedef constVoidMemFunInvoker0<T1> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2>
struct VoidMemFunInvoker1:BaseInvoker{
  typedef void (T1::*func_type)(T2) ;
  func_type func_;
  VoidMemFunInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2);
    return 0;
  }
};
template<typename T1,typename T2>
BaseInvoker* create(void (T1::*fun)(T2))
{
  typedef VoidMemFunInvoker1<T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2>
struct constVoidMemFunInvoker1:BaseInvoker{
  typedef void (T1::*func_type)(T2) const;
  func_type func_;
  constVoidMemFunInvoker1(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2);
    return 0;
  }
};
template<typename T1,typename T2>
BaseInvoker* create(void (T1::*fun)(T2)const)
{
  typedef constVoidMemFunInvoker1<T1,T2> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3>
struct VoidMemFunInvoker2:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3) ;
  func_type func_;
  VoidMemFunInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
BaseInvoker* create(void (T1::*fun)(T2,T3))
{
  typedef VoidMemFunInvoker2<T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3>
struct constVoidMemFunInvoker2:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3) const;
  func_type func_;
  constVoidMemFunInvoker2(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3);
    return 0;
  }
};
template<typename T1,typename T2,typename T3>
BaseInvoker* create(void (T1::*fun)(T2,T3)const)
{
  typedef constVoidMemFunInvoker2<T1,T2,T3> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct VoidMemFunInvoker3:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4) ;
  func_type func_;
  VoidMemFunInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4))
{
  typedef VoidMemFunInvoker3<T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4>
struct constVoidMemFunInvoker3:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4) const;
  func_type func_;
  constVoidMemFunInvoker3(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4)const)
{
  typedef constVoidMemFunInvoker3<T1,T2,T3,T4> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct VoidMemFunInvoker4:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5) ;
  func_type func_;
  VoidMemFunInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5))
{
  typedef VoidMemFunInvoker4<T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct constVoidMemFunInvoker4:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5) const;
  func_type func_;
  constVoidMemFunInvoker4(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5)const)
{
  typedef constVoidMemFunInvoker4<T1,T2,T3,T4,T5> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct VoidMemFunInvoker5:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6) ;
  func_type func_;
  VoidMemFunInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6))
{
  typedef VoidMemFunInvoker5<T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct constVoidMemFunInvoker5:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6) const;
  func_type func_;
  constVoidMemFunInvoker5(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6)const)
{
  typedef constVoidMemFunInvoker5<T1,T2,T3,T4,T5,T6> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct VoidMemFunInvoker6:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7) ;
  func_type func_;
  VoidMemFunInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7))
{
  typedef VoidMemFunInvoker6<T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct constVoidMemFunInvoker6:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7) const;
  func_type func_;
  constVoidMemFunInvoker6(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7)const)
{
  typedef constVoidMemFunInvoker6<T1,T2,T3,T4,T5,T6,T7> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct VoidMemFunInvoker7:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) ;
  func_type func_;
  VoidMemFunInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8))
{
  typedef VoidMemFunInvoker7<T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct constVoidMemFunInvoker7:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8) const;
  func_type func_;
  constVoidMemFunInvoker7(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8)const)
{
  typedef constVoidMemFunInvoker7<T1,T2,T3,T4,T5,T6,T7,T8> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct VoidMemFunInvoker8:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) ;
  func_type func_;
  VoidMemFunInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9))
{
  typedef VoidMemFunInvoker8<T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct constVoidMemFunInvoker8:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9) const;
  func_type func_;
  constVoidMemFunInvoker8(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9)const)
{
  typedef constVoidMemFunInvoker8<T1,T2,T3,T4,T5,T6,T7,T8,T9> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct VoidMemFunInvoker9:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) ;
  func_type func_;
  VoidMemFunInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())||!types::strictCheckType(state,10,types::typetag<T10>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())||!types::checkType(state,10,types::typetag<T10>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    T10 t10 = types::get(state,10,types::typetag<T10>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10))
{
  typedef VoidMemFunInvoker9<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> InvokerType;
  return new InvokerType(fun);
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
struct constVoidMemFunInvoker9:BaseInvoker{
  typedef void (T1::*func_type)(T2,T3,T4,T5,T6,T7,T8,T9,T10) const;
  func_type func_;
  constVoidMemFunInvoker9(func_type fun):func_(fun){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 10){return false;}
if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())||!types::strictCheckType(state,10,types::typetag<T10>())){return false;}
  }else{
      if(!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())||!types::checkType(state,10,types::typetag<T10>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
    T10 t10 = types::get(state,10,types::typetag<T10>());
  T1* ptr = types::get(state, 1, types::typetag<T1*>());
    (ptr->*func_)(t2,t3,t4,t5,t6,t7,t8,t9,t10);
    return 0;
  }
};
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
BaseInvoker* create(void (T1::*fun)(T2,T3,T4,T5,T6,T7,T8,T9,T10)const)
{
  typedef constVoidMemFunInvoker9<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> InvokerType;
  return new InvokerType(fun);
}
template<typename CLASS>
struct ConstructorInvoker0:BaseInvoker{
  ConstructorInvoker0(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 0){return false;}
    return true;
  }
  virtual int invoke(lua_State *state)
  {
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1>
struct ConstructorInvoker1:BaseInvoker{
  ConstructorInvoker1(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 1){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2>
struct ConstructorInvoker2:BaseInvoker{
  ConstructorInvoker2(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 2){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3>
struct ConstructorInvoker3:BaseInvoker{
  ConstructorInvoker3(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 3){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4>
struct ConstructorInvoker4:BaseInvoker{
  ConstructorInvoker4(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 4){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5>
struct ConstructorInvoker5:BaseInvoker{
  ConstructorInvoker5(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 5){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4,t5);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct ConstructorInvoker6:BaseInvoker{
  ConstructorInvoker6(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 6){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct ConstructorInvoker7:BaseInvoker{
  ConstructorInvoker7(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 7){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct ConstructorInvoker8:BaseInvoker{
  ConstructorInvoker8(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 8){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7,t8);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct ConstructorInvoker9:BaseInvoker{
  ConstructorInvoker9(){}
  virtual bool checktype(lua_State *state,bool strictcheck){
    if(lua_gettop(state) != 9){return false;}
    if(strictcheck){
      if(!types::strictCheckType(state,1,types::typetag<T1>())||!types::strictCheckType(state,2,types::typetag<T2>())||!types::strictCheckType(state,3,types::typetag<T3>())||!types::strictCheckType(state,4,types::typetag<T4>())||!types::strictCheckType(state,5,types::typetag<T5>())||!types::strictCheckType(state,6,types::typetag<T6>())||!types::strictCheckType(state,7,types::typetag<T7>())||!types::strictCheckType(state,8,types::typetag<T8>())||!types::strictCheckType(state,9,types::typetag<T9>())){return false;}
  }else{
      if(!types::checkType(state,1,types::typetag<T1>())||!types::checkType(state,2,types::typetag<T2>())||!types::checkType(state,3,types::typetag<T3>())||!types::checkType(state,4,types::typetag<T4>())||!types::checkType(state,5,types::typetag<T5>())||!types::checkType(state,6,types::typetag<T6>())||!types::checkType(state,7,types::typetag<T7>())||!types::checkType(state,8,types::typetag<T8>())||!types::checkType(state,9,types::typetag<T9>())){return false;}
  }
    return true;
  }
  virtual int invoke(lua_State *state)
  {
    T1 t1 = types::get(state,1,types::typetag<T1>());
    T2 t2 = types::get(state,2,types::typetag<T2>());
    T3 t3 = types::get(state,3,types::typetag<T3>());
    T4 t4 = types::get(state,4,types::typetag<T4>());
    T5 t5 = types::get(state,5,types::typetag<T5>());
    T6 t6 = types::get(state,6,types::typetag<T6>());
    T7 t7 = types::get(state,7,types::typetag<T7>());
    T8 t8 = types::get(state,8,types::typetag<T8>());
    T9 t9 = types::get(state,9,types::typetag<T9>());
  void *storage = lua_newuserdata(state, sizeof(CLASS));
    types::constructor<CLASS>(storage,t1,t2,t3,t4,t5,t6,t7,t8,t9);
    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
    return 1;
  }
};
