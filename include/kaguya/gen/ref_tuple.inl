//generated header by ref_tuple_generator.py
template<class T0>
struct ref_tuple1
{
  ref_tuple1(T0& va0):v0(va0){}
  T0& v0;
  static const int size=1;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
  }
};

template<class T0,class T1>
struct ref_tuple2
{
  ref_tuple2(T0& va0,T1& va1):v0(va0),v1(va1){}
  T0& v0;  T1& v1;
  static const int size=2;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
  }
};

template<class T0,class T1,class T2>
struct ref_tuple3
{
  ref_tuple3(T0& va0,T1& va1,T2& va2):v0(va0),v1(va1),v2(va2){}
  T0& v0;  T1& v1;  T2& v2;
  static const int size=3;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
  }
};

template<class T0,class T1,class T2,class T3>
struct ref_tuple4
{
  ref_tuple4(T0& va0,T1& va1,T2& va2,T3& va3):v0(va0),v1(va1),v2(va2),v3(va3){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;
  static const int size=4;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
  }
};

template<class T0,class T1,class T2,class T3,class T4>
struct ref_tuple5
{
  ref_tuple5(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4):v0(va0),v1(va1),v2(va2),v3(va3),v4(va4){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;  T4& v4;
  static const int size=5;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
    if(4 < res.size())v4 = res[4].get<T4>();
  }
};

template<class T0,class T1,class T2,class T3,class T4,class T5>
struct ref_tuple6
{
  ref_tuple6(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5):v0(va0),v1(va1),v2(va2),v3(va3),v4(va4),v5(va5){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;  T4& v4;  T5& v5;
  static const int size=6;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
    if(4 < res.size())v4 = res[4].get<T4>();
    if(5 < res.size())v5 = res[5].get<T5>();
  }
};

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6>
struct ref_tuple7
{
  ref_tuple7(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6):v0(va0),v1(va1),v2(va2),v3(va3),v4(va4),v5(va5),v6(va6){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;  T4& v4;  T5& v5;  T6& v6;
  static const int size=7;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
    if(4 < res.size())v4 = res[4].get<T4>();
    if(5 < res.size())v5 = res[5].get<T5>();
    if(6 < res.size())v6 = res[6].get<T6>();
  }
};

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7>
struct ref_tuple8
{
  ref_tuple8(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6,T7& va7):v0(va0),v1(va1),v2(va2),v3(va3),v4(va4),v5(va5),v6(va6),v7(va7){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;  T4& v4;  T5& v5;  T6& v6;  T7& v7;
  static const int size=8;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
    if(4 < res.size())v4 = res[4].get<T4>();
    if(5 < res.size())v5 = res[5].get<T5>();
    if(6 < res.size())v6 = res[6].get<T6>();
    if(7 < res.size())v7 = res[7].get<T7>();
  }
};

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
struct ref_tuple9
{
  ref_tuple9(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6,T7& va7,T8& va8):v0(va0),v1(va1),v2(va2),v3(va3),v4(va4),v5(va5),v6(va6),v7(va7),v8(va8){}
  T0& v0;  T1& v1;  T2& v2;  T3& v3;  T4& v4;  T5& v5;  T6& v6;  T7& v7;  T8& v8;
  static const int size=9;
  template<class T>
  void operator=(const T& eval)
  {
    const std::vector<LuaRef>& res = eval.get_result(size);
    if(0 < res.size())v0 = res[0].get<T0>();
    if(1 < res.size())v1 = res[1].get<T1>();
    if(2 < res.size())v2 = res[2].get<T2>();
    if(3 < res.size())v3 = res[3].get<T3>();
    if(4 < res.size())v4 = res[4].get<T4>();
    if(5 < res.size())v5 = res[5].get<T5>();
    if(6 < res.size())v6 = res[6].get<T6>();
    if(7 < res.size())v7 = res[7].get<T7>();
    if(8 < res.size())v8 = res[8].get<T8>();
  }
};

template<class T0>
ref_tuple1<T0> tie(T0& va0)
{
  return ref_tuple1<T0>(va0);
}

template<class T0,class T1>
ref_tuple2<T0,T1> tie(T0& va0,T1& va1)
{
  return ref_tuple2<T0,T1>(va0,va1);
}

template<class T0,class T1,class T2>
ref_tuple3<T0,T1,T2> tie(T0& va0,T1& va1,T2& va2)
{
  return ref_tuple3<T0,T1,T2>(va0,va1,va2);
}

template<class T0,class T1,class T2,class T3>
ref_tuple4<T0,T1,T2,T3> tie(T0& va0,T1& va1,T2& va2,T3& va3)
{
  return ref_tuple4<T0,T1,T2,T3>(va0,va1,va2,va3);
}

template<class T0,class T1,class T2,class T3,class T4>
ref_tuple5<T0,T1,T2,T3,T4> tie(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4)
{
  return ref_tuple5<T0,T1,T2,T3,T4>(va0,va1,va2,va3,va4);
}

template<class T0,class T1,class T2,class T3,class T4,class T5>
ref_tuple6<T0,T1,T2,T3,T4,T5> tie(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5)
{
  return ref_tuple6<T0,T1,T2,T3,T4,T5>(va0,va1,va2,va3,va4,va5);
}

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6>
ref_tuple7<T0,T1,T2,T3,T4,T5,T6> tie(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6)
{
  return ref_tuple7<T0,T1,T2,T3,T4,T5,T6>(va0,va1,va2,va3,va4,va5,va6);
}

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7>
ref_tuple8<T0,T1,T2,T3,T4,T5,T6,T7> tie(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6,T7& va7)
{
  return ref_tuple8<T0,T1,T2,T3,T4,T5,T6,T7>(va0,va1,va2,va3,va4,va5,va6,va7);
}

template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
ref_tuple9<T0,T1,T2,T3,T4,T5,T6,T7,T8> tie(T0& va0,T1& va1,T2& va2,T3& va3,T4& va4,T5& va5,T6& va6,T7& va7,T8& va8)
{
  return ref_tuple9<T0,T1,T2,T3,T4,T5,T6,T7,T8>(va0,va1,va2,va3,va4,va5,va6,va7,va8);
}

