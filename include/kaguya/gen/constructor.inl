template<typename CLASS>
CLASS* constructor(void* pointer)
{
  CLASS* ptr = new(pointer) CLASS();
  return ptr;
}
template<typename CLASS,typename T1>
CLASS* constructor(void* pointer,T1 t1)
{
  CLASS* ptr = new(pointer) CLASS(t1);
  return ptr;
}
template<typename CLASS,typename T1,typename T2>
CLASS* constructor(void* pointer,T1 t1,T2 t2)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4,t5);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4,t5,t6);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4,t5,t6,t7);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4,t5,t6,t7,t8);
  return ptr;
}
template<typename CLASS,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
CLASS* constructor(void* pointer,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9)
{
  CLASS* ptr = new(pointer) CLASS(t1,t2,t3,t4,t5,t6,t7,t8,t9);
  return ptr;
}
