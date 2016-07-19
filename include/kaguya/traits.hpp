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

		/// @brief same std::enable_if
		template<bool B, class T = void>struct enable_if {};
		/// @brief same std::enable_if
		template<class T>struct enable_if<true, T> { 
			///@	T (defined only if Cond is true)
			typedef T type;
		};

		/// @brief same std::is_void
		template< typename T >
		struct is_void :integral_constant<bool, false>
		{
		};
		/// @brief same std::is_void
		template<>
		struct is_void<void> :integral_constant<bool, true>
		{
		};

		/// @brief same std::decay
		template< class T >
		struct decay {
		private:
			typedef typename standard::remove_reference<T>::type U;
		public:
			///@ If T is a reference type, the type referrered to by T.	Otherwise, T.
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

		/// @brief same std::is_const
		template<class T> struct is_const : integral_constant<bool, false> {};
		template<class T> struct is_const<const T> : integral_constant<bool, true> {};

		/// @brief Trait class that identifies whether T is a const reference type.
		template<class T> struct is_const_reference : integral_constant<bool, false> {};
		template<class T> struct is_const_reference<const T&> : integral_constant<bool, true> {};

		/// @brief Obtains the type T without top-level const and reference.
		template< typename T >
		struct remove_const_and_reference {
			/// @brief If T is const or reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};
		/// @brief Obtains the type T without top-level const and reference.
		template< typename T >
		struct remove_const_and_reference<T&> {
			/// @brief If T is const or reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};
		/// @brief Obtains the type T without top-level const and reference.
		template< typename T >
		struct remove_const_and_reference<const T> {
			/// @brief If T is const or reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};
		/// @brief Obtains the type T without top-level const and reference.
		template< typename T >
		struct remove_const_and_reference<const T&> {
			/// @brief If T is const or reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};

		/// @brief Obtains the type T without top-level const reference.
		template< typename T >
		struct remove_const_reference {
			/// @brief If T is const reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};
		/// @brief Obtains the type T without top-level const reference.
		template< typename T >
		struct remove_const_reference<const T&> {
			/// @brief If T is const reference, the same type as T but with the const reference removed.Otherwise, T
			typedef T type;
		};

		/// @brief Trait class that identifies whether T is a std::vector type.
		template<class T> struct is_std_vector : integral_constant<bool, false> {};
		template<class T, class A> struct is_std_vector<std::vector<T, A> > : integral_constant<bool, true> {};

		/// @brief Trait class that identifies whether T is a std::map type.
		template<class T> struct is_std_map : integral_constant<bool, false> {};
		template<class K, class V, class C, class A> struct is_std_map<std::map<K, V, C, A> > : integral_constant<bool, true> {};
	}


	/// @addtogroup lua_type_traits

	/// @ingroup lua_type_traits
	/// @brief If you want to customize the conversion to type of lua yourself ,
	///  implement specialize of this class
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


	/// @brief Trait class that identifies whether T is a userdata type.
	template< typename T, typename Enable = void>
	struct is_usertype : traits::integral_constant<bool, false> {};
	template< typename T>
	struct is_usertype<T, typename lua_type_traits<T>::Registerable> : traits::integral_constant<bool, true> {};

	/// @brief Trait class that identifies whether T is a registerable by UserdataMetatable.
	template< typename T>
	struct is_registerable : is_usertype<T> {};
}
