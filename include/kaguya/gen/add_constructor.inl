//generated header by add_constructor_generator.py
ClassMetatable& addConstructor()
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type>());
return *this;
}
template<typename T1>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1>());
return *this;
}
template<typename T1,typename T2>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2>());
return *this;
}
template<typename T1,typename T2,typename T3>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0,types::typetag<T5>* t5= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4,T5>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0,types::typetag<T5>* t5= 0,types::typetag<T6>* t6= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4,T5,T6>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0,types::typetag<T5>* t5= 0,types::typetag<T6>* t6= 0,types::typetag<T7>* t7= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4,T5,T6,T7>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0,types::typetag<T5>* t5= 0,types::typetag<T6>* t6= 0,types::typetag<T7>* t7= 0,types::typetag<T8>* t8= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4,T5,T6,T7,T8>());
return *this;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
ClassMetatable& addConstructor(types::typetag<T1>* t1= 0,types::typetag<T2>* t2= 0,types::typetag<T3>* t3= 0,types::typetag<T4>* t4= 0,types::typetag<T5>* t5= 0,types::typetag<T6>* t6= 0,types::typetag<T7>* t7= 0,types::typetag<T8>* t8= 0,types::typetag<T9>* t9= 0)
{
  function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type,T1,T2,T3,T4,T5,T6,T7,T8,T9>());
return *this;
}
