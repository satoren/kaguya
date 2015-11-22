//generated header by add_constructor_generator.py
ClassMetatable& addConstructor()
{
  functor_type fun(new kaguya::nativefunction::constructor_caller0<class_type>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller1<class_type,T1>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller2<class_type,T1,T2>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller3<class_type,T1,T2,T3>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller4<class_type,T1,T2,T3,T4>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0,types::type_tag<T5>* t5= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller5<class_type,T1,T2,T3,T4,T5>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0,types::type_tag<T5>* t5= 0,types::type_tag<T6>* t6= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller6<class_type,T1,T2,T3,T4,T5,T6>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0,types::type_tag<T5>* t5= 0,types::type_tag<T6>* t6= 0,types::type_tag<T7>* t7= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller7<class_type,T1,T2,T3,T4,T5,T6,T7>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0,types::type_tag<T5>* t5= 0,types::type_tag<T6>* t6= 0,types::type_tag<T7>* t7= 0,types::type_tag<T8>* t8= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller8<class_type,T1,T2,T3,T4,T5,T6,T7,T8>());
  function_map_["new"].push_back(fun);
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
ClassMetatable& addConstructor(types::type_tag<T1>* t1= 0,types::type_tag<T2>* t2= 0,types::type_tag<T3>* t3= 0,types::type_tag<T4>* t4= 0,types::type_tag<T5>* t5= 0,types::type_tag<T6>* t6= 0,types::type_tag<T7>* t7= 0,types::type_tag<T8>* t8= 0,types::type_tag<T9>* t9= 0)
{
  functor_type fun(new kaguya::nativefunction::constructor_caller9<class_type,T1,T2,T3,T4,T5,T6,T7,T8,T9>());
  function_map_["new"].push_back(fun);
return *this;
}
