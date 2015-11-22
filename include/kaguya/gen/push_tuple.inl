//generated header by push_tuple_generator.py
template<>
int push(lua_State* l,standard::tuple<>& v)
{
  int count =0;
  return count;
}
template<typename T1>
int push(lua_State* l,standard::tuple<T1>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  return count;
}
template<typename T1,typename T2>
int push(lua_State* l,standard::tuple<T1,T2>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  return count;
}
template<typename T1,typename T2,typename T3>
int push(lua_State* l,standard::tuple<T1,T2,T3>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4,T5>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  count += push(l,v.get<4>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4,T5,T6>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  count += push(l,v.get<4>());
  count += push(l,v.get<5>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4,T5,T6,T7>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  count += push(l,v.get<4>());
  count += push(l,v.get<5>());
  count += push(l,v.get<6>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  count += push(l,v.get<4>());
  count += push(l,v.get<5>());
  count += push(l,v.get<6>());
  count += push(l,v.get<7>());
  return count;
}
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
int push(lua_State* l,standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8,T9>& v)
{
  int count =0;
  count += push(l,v.get<0>());
  count += push(l,v.get<1>());
  count += push(l,v.get<2>());
  count += push(l,v.get<3>());
  count += push(l,v.get<4>());
  count += push(l,v.get<5>());
  count += push(l,v.get<6>());
  count += push(l,v.get<7>());
  count += push(l,v.get<8>());
  return count;
}
