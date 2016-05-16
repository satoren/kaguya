
#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"

namespace kaguya
{
	template<>
	struct lua_type_traits<standard::tuple<> >
	{
		static int push(lua_State* l, const standard::tuple<>& v)
		{
			int count = 0;
			return count;
		}
	};
	template<typename T1>
	struct lua_type_traits<standard::tuple<T1> >
	{
		static int push(lua_State* l, const standard::tuple<T1>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			return count;
		}
	};
	template<typename T1, typename T2>
	struct lua_type_traits<standard::tuple<T1, T2> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3>
	struct lua_type_traits<standard::tuple<T1, T2, T3> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4, T5> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4, T5>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			count += lua_type_traits<T5>::push(l, standard::get<4>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4, T5, T6> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4, T5, T6>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			count += lua_type_traits<T5>::push(l, standard::get<4>(v));
			count += lua_type_traits<T6>::push(l, standard::get<5>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4, T5, T6, T7> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4, T5, T6, T7>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			count += lua_type_traits<T5>::push(l, standard::get<4>(v));
			count += lua_type_traits<T6>::push(l, standard::get<5>(v));
			count += lua_type_traits<T7>::push(l, standard::get<6>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4, T5, T6, T7, T8> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4, T5, T6, T7, T8>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			count += lua_type_traits<T5>::push(l, standard::get<4>(v));
			count += lua_type_traits<T6>::push(l, standard::get<5>(v));
			count += lua_type_traits<T7>::push(l, standard::get<6>(v));
			count += lua_type_traits<T8>::push(l, standard::get<7>(v));
			return count;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	struct lua_type_traits<standard::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9> >
	{
		static int push(lua_State* l, const standard::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& v)
		{
			int count = 0;
			count += lua_type_traits<T1>::push(l, standard::get<0>(v));
			count += lua_type_traits<T2>::push(l, standard::get<1>(v));
			count += lua_type_traits<T3>::push(l, standard::get<2>(v));
			count += lua_type_traits<T4>::push(l, standard::get<3>(v));
			count += lua_type_traits<T5>::push(l, standard::get<4>(v));
			count += lua_type_traits<T6>::push(l, standard::get<5>(v));
			count += lua_type_traits<T7>::push(l, standard::get<6>(v));
			count += lua_type_traits<T8>::push(l, standard::get<7>(v));
			count += lua_type_traits<T9>::push(l, standard::get<8>(v));
			return count;
		}
	};
}