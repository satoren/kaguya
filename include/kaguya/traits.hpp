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
	}

};
