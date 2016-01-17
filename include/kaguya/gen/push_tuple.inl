//generated header by push_tuple_generator.py
namespace types{
template<>
inline int push(lua_State* l,const standard::tuple<>& v)
{
  int count =0;
  return count;
}
}
namespace traits{
template<>
struct is_push_specialized<standard::tuple<> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1>
inline int push(lua_State* l,const standard::tuple<T1>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  return count;
}
}
namespace traits{
template<typename T1>
struct is_push_specialized<standard::tuple<T1> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2>
inline int push(lua_State* l,const standard::tuple<T1,T2>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2>
struct is_push_specialized<standard::tuple<T1,T2> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3>
struct is_push_specialized<standard::tuple<T1,T2,T3> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4,typename T5>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4,T5>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  count += push(l,standard::get<4>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4,T5> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4,T5,T6>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  count += push(l,standard::get<4>(v));
  count += push(l,standard::get<5>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4,T5,T6> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4,T5,T6,T7>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  count += push(l,standard::get<4>(v));
  count += push(l,standard::get<5>(v));
  count += push(l,standard::get<6>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4,T5,T6,T7> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  count += push(l,standard::get<4>(v));
  count += push(l,standard::get<5>(v));
  count += push(l,standard::get<6>(v));
  count += push(l,standard::get<7>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8> > : integral_constant<bool, true> {};}
namespace types{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
inline int push(lua_State* l,const standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8,T9>& v)
{
  int count =0;
  count += push(l,standard::get<0>(v));
  count += push(l,standard::get<1>(v));
  count += push(l,standard::get<2>(v));
  count += push(l,standard::get<3>(v));
  count += push(l,standard::get<4>(v));
  count += push(l,standard::get<5>(v));
  count += push(l,standard::get<6>(v));
  count += push(l,standard::get<7>(v));
  count += push(l,standard::get<8>(v));
  return count;
}
}
namespace traits{
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
struct is_push_specialized<standard::tuple<T1,T2,T3,T4,T5,T6,T7,T8,T9> > : integral_constant<bool, true> {};}
