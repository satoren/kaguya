#pragma once

#include <string>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace traits
	{
		template<bool B, class T = void>struct enable_if {};
		template<class T>struct enable_if<true, T> { typedef T type; };

		template< typename T > struct remove_reference { typedef T type; };
		template< typename T > struct remove_reference<T&> { typedef T type; };
#if KAGUYA_USE_RVALUE_REFERENCE
		template< typename T > struct remove_reference<T&&> { typedef T type; };
#endif

		//    template< typename T > struct remove_reference<T&&> {typedef T type;};

		template< typename T > struct remove_pointer { typedef T type; };
		template< typename T > struct remove_pointer<T*> { typedef T type; };
		template< typename T > struct remove_pointer<T* const> { typedef T type; };
		template< typename T > struct remove_pointer<T* volatile> { typedef T type; };
		template< typename T > struct remove_pointer<T* const volatile> { typedef T type; };


		template< typename T > struct remove_const { typedef T type; };
		template< typename T > struct remove_const<const T> { typedef T type; };

		template< typename T > struct remove_volatile { typedef T type; };
		template< typename T > struct remove_volatile<volatile T> { typedef T type; };


		template< typename T >
		struct remove_cv {
			typedef typename remove_volatile<typename remove_const<T>::type>::type type;
		};
		template<bool B, class T, class F>
		struct conditional { typedef T type; };

		template<class T, class F>
		struct conditional<false, T, F> { typedef F type; };


		template< typename T >
		struct remove_const_reference {
			typedef typename remove_const<typename remove_reference<T>::type>::type type;
		};

		template< typename T >
		struct arg_get_type {
			typedef typename conditional<standard::is_arithmetic<typename remove_reference<T>::type >::value|| standard::is_enum<typename remove_reference<T>::type>::value, typename remove_const_reference<T>::type,T>::type type;
		};
		template< >
		struct arg_get_type<std::string&> {
			typedef std::string type;
		};
		template< >
		struct arg_get_type<const std::string&> {
			typedef std::string type;
		};
	}

};
