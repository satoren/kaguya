// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <string>

#include "kaguya/config.hpp"

namespace kaguya
{
	namespace util
	{
		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...), ThisType& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}
		
		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...)const, const ThisType& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}
#if defined(_MSC_VER) && _MSC_VER >= 1900 || defined(__cpp_ref_qualifiers)
		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...) &, ThisType& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}
		
		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...)const &, const ThisType& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}
		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...) &&, ThisType&& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}

		template<class ThisType, class Res, class... FArgs, class... Args>
		Res invoke(Res(ThisType::*f)(FArgs...)const &&, const ThisType& this_, Args&&... args)
		{
			return (this_.*f)(std::forward<Args>(args)...);
		}
#endif

		template<class F, class... Args>
		auto invoke(F&& f, Args&&... args) -> decltype(f(std::forward<Args>(args)...))
		{
			return f(std::forward<Args>(args)...);
		}


		template<class... Args>
		struct TypeTuple {
		};
		template<class Ret, class... Args>
		struct FunctionSignatureType {
			typedef Ret result_type;
			typedef TypeTuple<Args...> argument_type_tuple;
			static const int argument_count = sizeof...(Args);
			typedef Ret (*c_function_type)(Args...);
		};
		template <typename T>
		struct FunctorSignature {};

		template <typename T, typename Ret, typename... Args>
		struct FunctorSignature<Ret(T::*)(Args...) const> {
			typedef FunctionSignatureType<Ret, Args...> type;
		};
		template <typename T, typename Ret, typename... Args>
		struct FunctorSignature<Ret(T::*)(Args...)> {
			typedef FunctionSignatureType<Ret, Args...> type;
		};
		template <typename T>
		struct FunctionSignature : public FunctorSignature<decltype(&T::operator())> {};

		template <typename T, typename Ret, typename... Args>
		struct FunctionSignature<Ret(T::*)(Args...)> {
			typedef FunctionSignatureType<Ret, T&, Args...> type;
		};
		template <typename T, typename Ret, typename... Args>
		struct FunctionSignature<Ret(T::*)(Args...) const> {
			typedef FunctionSignatureType<Ret, const T&, Args...> type;
		};
		template<class Ret, class... Args>
		struct FunctionSignature<Ret(*)(Args...)> {
			typedef FunctionSignatureType<Ret, Args...> type;
		};
		template<class Ret, class... Args>
		struct FunctionSignature<Ret(Args...)> {
			typedef FunctionSignatureType<Ret, Args...> type;
		};

		template<typename F>
		struct FunctionResultType
		{
			typedef typename FunctionSignature<F>::type::result_type type;
		};



		template<std::size_t remain, class Arg, bool flag = remain <= 0>
		struct TypeIndexGet;

		template<std::size_t remain, class Arg,class... Args >
		struct TypeIndexGet<remain, TypeTuple<Arg, Args...>, true>
		{
			typedef Arg type;
		};

		template<std::size_t remain, class Arg, class... Args >
		struct TypeIndexGet<remain, TypeTuple<Arg, Args...>, false>
			: TypeIndexGet<remain - 1, TypeTuple<Args...> >
		{
		};
		template<int N, typename F>
		struct ArgumentType
		{
			typedef typename TypeIndexGet<N, typename FunctionSignature<F>::type::argument_type_tuple>::type type;
		};
	}

}
