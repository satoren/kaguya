#pragma once

#include <string>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace traits
	{
		using namespace kaguya::standard;

		template<bool B, class T = void>struct enable_if {};
		template<class T>struct enable_if<true, T> { typedef T type; };


		template< typename T >
		struct remove_const_reference {
			typedef typename remove_const<typename remove_reference<T>::type>::type type;
		};


		template< class T >
		struct lua_class_type
		{
			typedef typename remove_cv<typename remove_reference<typename remove_pointer<T>::type>::type>::type* type;
		};

		template< class T >
		struct is_convertible_lua_string :integral_constant<bool,
			is_same<typename remove_const_reference<T>::type, std::string>::value ||
			is_same<T, const char*>::value> {};

		template< class T >
		struct is_convertible_lua_number :integral_constant<bool,
			is_floating_point<typename remove_const_reference<T>::type>::value ||
			is_integral<typename remove_const_reference<T>::type>::value ||
			is_enum<typename remove_const_reference<T>::type>::value> {};

#if LUA_VERSION_NUM >= 503
		template< class T >
		struct lua_number_type
		{
			typedef typename conditional<is_floating_point<T>::value, lua_Number, lua_Integer>::type type;
		};
#else
		template< class T >
		struct lua_number_type
		{
			typedef lua_Number type;
		};
#endif

		template< class T >
		struct lua_type
		{
			typedef typename remove_cv<typename remove_reference<typename remove_pointer<T>::type>::type>::type* type;
		};

		template< typename T >
		struct arg_get_type_convertible_lua_number {
			typedef T type;
			// much warning int64_t to int
			//typedef typename conditional<is_enum<T>::value
				//, T
				//, typename conditional<is_same<T, bool>::value,bool, typename lua_number_type<T>::type>::type
			//	>::type type;

		};
		template< typename T >
		struct arg_get_type {
			typedef typename conditional<is_convertible_lua_number<typename remove_reference<T>::type >::value
				, typename arg_get_type_convertible_lua_number<typename remove_const_reference<T>::type >::type
				, T>::type type;
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
