// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <vector>
#include <map>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace traits
	{
		using standard::integral_constant;
		using standard::is_lvalue_reference;
		using standard::remove_reference;
		using standard::remove_pointer;
		using standard::remove_const;
		using standard::is_pointer;
		using standard::is_function;
		using standard::is_floating_point;
		using standard::is_integral;
		using standard::is_enum;
		using standard::is_convertible;
		using standard::is_same;

		template<bool B, class T = void>struct enable_if {};
		template<class T>struct enable_if<true, T> { typedef T type; };

		template< typename T >
		struct is_void :integral_constant<bool, false>
		{
		};
		template<>
		struct is_void<void> :integral_constant<bool, true>
		{
		};

		//can not use boost.decay
		template< class T >
		struct decay {
		private:
			typedef typename standard::remove_reference<T>::type U;
		public:
			typedef typename standard::conditional<
				standard::is_array<U>::value,
				typename standard::remove_extent<U>::type*,
				typename standard::conditional<
				is_function<U>::value,
				typename standard::add_pointer<U>::type,
				typename standard::remove_cv<U>::type
				>::type
			>::type type;
		};

		template<class T> struct is_const : integral_constant<bool, false> {};
		template<class T> struct is_const<const T> : integral_constant<bool, true> {};

		template<class T> struct is_const_reference : integral_constant<bool, false> {};
		template<class T> struct is_const_reference<const T&> : integral_constant<bool, true> {};

		template< typename T >
		struct remove_const_and_reference {
			typedef T type;
		};
		template< typename T >
		struct remove_const_and_reference<T&> {
			typedef T type;
		};
		template< typename T >
		struct remove_const_and_reference<const T> {
			typedef T type;
		};
		template< typename T >
		struct remove_const_and_reference<const T&> {
			typedef T type;
		};
		template< typename T >
		struct remove_const_reference {
			typedef T type;
		};
		template< typename T >
		struct remove_const_reference<const T&> {
			typedef T type;
		};

		template<class T> struct is_std_vector : integral_constant<bool, false> {};
		template<class T, class A> struct is_std_vector<std::vector<T, A> > : integral_constant<bool, true> {};
		template<class T> struct is_std_map : integral_constant<bool, false> {};
		template<class K, class V, class C, class A> struct is_std_map<std::map<K, V, C, A> > : integral_constant<bool, true> {};
	}

	template<typename T, typename Enable = void>
	struct lua_type_traits
	{
		typedef void Registerable;

		typedef typename traits::decay<T>::type NCRT;
		typedef const NCRT& get_type;
		typedef const NCRT& push_type;

		static bool checkType(lua_State* l, int index);
		static bool strictCheckType(lua_State* l, int index);

		static get_type get(lua_State* l, int index);
		static int push(lua_State* l, push_type v);
#if KAGUYA_USE_RVALUE_REFERENCE
		static int push(lua_State* l, NCRT&& v);
#endif
	};

	template< typename T, typename Enable = void>
	struct is_registerable : traits::integral_constant<bool, false> {};
	template< typename T>
	struct is_registerable<T, typename lua_type_traits<T>::Registerable> : traits::integral_constant<bool, true> {};


	template< typename T, typename Enable = void>
	struct is_usertype : traits::integral_constant<bool, false> {};
	template< typename T>
	struct is_usertype<T, typename lua_type_traits<T>::Registerable> : traits::integral_constant<bool, true> {};
}
