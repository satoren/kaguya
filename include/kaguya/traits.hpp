#pragma once

#include <string>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace nativefunction
	{
		struct FunctorType;
	}
	typedef nativefunction::FunctorType FunctorType;
	namespace traits
	{
		using namespace kaguya::standard;

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
		

		template<class T> struct is_const : integral_constant<bool, false> {};
		template<class T> struct is_const<const T> : integral_constant<bool, true> {};


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


		template< class T >
		struct lua_class_type
		{
			typedef typename remove_cv<typename remove_reference<typename remove_pointer<T>::type>::type>::type* type;
		};

		template< class T >
		struct is_convertible_lua_string :integral_constant<bool,
			is_same<typename remove_const_and_reference<T>::type, std::string>::value ||
			is_same<T, const char*>::value> {};

		template< class T >
		struct is_convertible_lua_number :integral_constant<bool,
			is_floating_point<typename remove_const_and_reference<T>::type>::value ||
			is_integral<typename remove_const_and_reference<T>::type>::value ||
			is_enum<typename remove_const_and_reference<T>::type>::value> {};



		template< class T >
		struct lua_number_type
		{
			typedef typename conditional<is_same<T, bool>::value,
				bool,
#if LUA_VERSION_NUM >= 503
				typename conditional<is_floating_point<T>::value, lua_Number, lua_Integer>::type
#else
				lua_Number
#endif
			>::type type;
		};


		template< class T >
		struct is_convertible_duk_string :integral_constant<bool,
			is_same<typename remove_const_and_reference<T>::type, std::string>::value ||
			is_same<T, const char*>::value> {};

		template< class T >
		struct lua_push_type
		{
			typedef typename conditional<
				is_function<typename remove_pointer<T>::type>::value, nativefunction::FunctorType, typename conditional<is_convertible_lua_number<typename remove_reference<T>::type >::value
				, typename lua_number_type<typename remove_const_and_reference<T>::type >::type
				, T>::type>::type type;
		};
		template< >
		struct lua_push_type<std::string&> {
			typedef const std::string& type;
		};
		template< >
		struct lua_push_type<const std::string&> {
			typedef const std::string& type;
		};
		template<int N>
		struct lua_push_type<const char[N]> {
			typedef const char* type;
		};
		template< >
		struct lua_push_type<const char*> {
			typedef const char* type;
		};

		template<class T> struct is_push_specialized : integral_constant<bool, 
		!standard::is_same<T, typename lua_push_type<T>::type>::value ||
		is_convertible_lua_number<typename remove_const_and_reference<T>::type>::value
		> {};

		template< >	struct is_push_specialized<std::string> : integral_constant<bool, true> {};
		template< >	struct is_push_specialized<const std::string> : integral_constant<bool, true> {};
		template< >	struct is_push_specialized<const char*> : integral_constant<bool, true> {};
		template< >	struct is_push_specialized<FunctorType> : integral_constant<bool, true> {};

		template<class T>	struct is_std_vector : integral_constant<bool, false> {};
		template<class T>	struct is_std_vector<std::vector<T> > : integral_constant<bool, true> {};
		template<class T>	struct is_std_map : integral_constant<bool, false> {};
		template<class K, class V>	struct is_std_map<std::map<K,V> > : integral_constant<bool, true> {};

		template< typename T >
		struct arg_get_type {
			typedef typename conditional<is_convertible_lua_number<typename remove_const_and_reference<T>::type >::value
				, typename remove_const_and_reference<T>::type
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
		template<int N>
		struct arg_get_type<const char[N]> {
			typedef std::string type;
		};

		template< typename T >
		struct enum_dispatch_type {
			typedef typename remove_const_and_reference<T>::type type;
		};

		template< typename T >
		struct arg_get_type_dispatch {
			typedef typename conditional<is_enum<typename remove_const_and_reference<T>::type >::value
				, enum_dispatch_type<T>
				, typename conditional<is_convertible_lua_number<typename remove_const_and_reference<T>::type >::value
				, typename lua_number_type<typename remove_const_and_reference<T>::type >::type
				, typename arg_get_type<T>::type>::type
			>::type type;
		};
	}

	namespace types
	{
		template<typename T>
		struct typetag {};
	}
};
