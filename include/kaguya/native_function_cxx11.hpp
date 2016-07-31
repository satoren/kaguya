// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <string>

#include "kaguya/config.hpp"
#include "kaguya/utility.hpp"
#include "kaguya/object.hpp"


namespace kaguya
{
	namespace nativefunction
	{
		template<std::size_t... indexes>
		struct index_tuple {};

		template<std::size_t first, std::size_t last, class result = index_tuple<>, bool flag = first >= last>
		struct index_range
		{
			using type = result;
		};

		template<std::size_t step, std::size_t last, std::size_t... indexes>
		struct index_range<step, last, index_tuple<indexes...>, false>
			: index_range<step + 1, last, index_tuple<indexes..., step>>
		{};

		template<class F>
		struct arg_count;
		template<class R, class... Args>
		struct arg_count<util::FunctionSignatureType<R, Args...> > : traits::integral_constant<size_t, sizeof...(Args)>
		{};
		template<class T>
		struct min_arg_count : arg_count<T> {};
		template<class T>
		struct max_arg_count : arg_count<T> {};

		template<class F>
		struct arg_index_range;

		template<class R, class... Args>
		struct arg_index_range<util::FunctionSignatureType<R, Args...> > : index_range<1, sizeof...(Args)+1>
		{};
		
		template <typename T>
		struct f_signature : public util::FunctionSignature<T> {};


		template <typename T, typename Ret, typename... Args>
		struct f_signature<Ret(T::*)(Args...)> {
			typedef util::FunctionSignatureType<Ret, T&, Args...> type;
		};
		template <typename T, typename Ret, typename... Args>
		struct f_signature<Ret(T::*)(Args...) const> {
			typedef util::FunctionSignatureType<Ret, const T&, Args...> type;
		};
		template <typename T, typename Ret, typename... Args>
		struct f_signature<Ret(T::* &)(Args...)> {
			typedef util::FunctionSignatureType<Ret, T&, Args...> type;
		};
		template <typename T, typename Ret, typename... Args>
		struct f_signature<Ret(T::* &)(Args...) const> {
			typedef util::FunctionSignatureType<Ret, const T&, Args...> type;
		};
		template<class Ret, class... Args>
		struct f_signature<Ret(*)(Args...)> {
			typedef util::FunctionSignatureType<Ret, Args...> type;
		};
		template<class Ret, class... Args>
		struct f_signature<Ret(Args...)> {
			typedef util::FunctionSignatureType<Ret, Args...> type;
		};

		template<class F, class Ret, class... Args, size_t... Indexes>
		int _call_apply(lua_State* state, F&& f, index_tuple<Indexes...>, util::FunctionSignatureType<Ret, Args...>)
		{
			return util::push_args(state, util::invoke(f, lua_type_traits<Args>::get(state, Indexes)...));
		}
		template<class F, class... Args, size_t... Indexes>
		int _call_apply(lua_State* state, F&& f, index_tuple<Indexes...>, util::FunctionSignatureType<void, Args...>)
		{
			util::invoke(f, lua_type_traits<Args>::get(state, Indexes)...);
			return 0;
		}

		inline bool all_true()
		{
			return true;
		}
		template<class Arg, class...Args>bool all_true(Arg f, Args... args)
		{	//check from backward and lazy evaluation
			return all_true(args...) && bool(f);
		}

		inline void join(std::string& result, const char* delim)
		{
		}
		template<class Arg, class...Args>void join(std::string& result, const char* delim, const Arg& str, const Args&... args)
		{
			result += str;
			result += delim;
			join(result, delim, args...);
		}

		template<typename T, size_t Index>
		struct _wcheckeval
		{
			_wcheckeval(lua_State* s, bool opt) :state(s), opt_arg(opt) {}
			lua_State* state;
			bool opt_arg;
			operator bool()
			{
				return (opt_arg && lua_isnoneornil(state, Index) )|| lua_type_traits<T>::checkType(state, Index);
			}
		};

		template<typename T, size_t Index>
		struct _scheckeval
		{
			_scheckeval(lua_State* s, bool opt) :state(s), opt_arg(opt) {}
			lua_State* state;
			bool opt_arg;
			operator bool()
			{
				return (opt_arg && lua_isnoneornil(state, Index)) || lua_type_traits<T>::strictCheckType(state, Index);
			}
		};

		template<class R, class... Args, size_t... Indexes>
		bool _ctype_apply(lua_State* state, index_tuple<Indexes...>, util::FunctionSignatureType<R, Args...>, int opt_count)
		{
			return all_true(_wcheckeval<Args, Indexes>(state, sizeof...(Indexes)-opt_count < Indexes)...);
		}
		template<class R, class... Args, size_t... Indexes>
		bool _sctype_apply(lua_State* state, index_tuple<Indexes...>, util::FunctionSignatureType<R, Args...>, int opt_count)
		{

			return all_true(_scheckeval<Args, Indexes>(state, sizeof...(Indexes)-opt_count < Indexes)...);
		}
		template<class R, class... Args>
		std::string _type_name_apply(util::FunctionSignatureType<R, Args...>)
		{
			std::string result;
			join(result, ",", typeid(Args).name()...);
			return result;
		}

		///! for constructor
		template<class ClassType, class... Args>
		struct constructor_signature_type :util::FunctionSignatureType<ClassType, Args...>
		{
		};
		template <class ClassType, class... Args>
		struct f_signature<constructor_signature_type<ClassType, Args...> > {
			typedef constructor_signature_type<ClassType, Args...> type;
		};
		template<class ClassType, class... Args>
		struct arg_index_range<constructor_signature_type<ClassType, Args...> > : index_range<1, sizeof...(Args)+1>
		{};
		template<class ClassType, class... Args>
		struct min_arg_count<constructor_signature_type<ClassType, Args...> > : traits::integral_constant<size_t, sizeof...(Args)>
		{};
		template<class ClassType, class... Args>
		struct max_arg_count<constructor_signature_type<ClassType, Args...> > : traits::integral_constant<size_t, sizeof...(Args)>
		{};

		template <class ClassType, class... Args, size_t... Indexes>
		int _call_apply(lua_State* state, constructor_signature_type<ClassType, Args...>, index_tuple<Indexes...>, constructor_signature_type<ClassType, Args...>)
		{
			typedef ObjectWrapper<ClassType> wrapper_type;
			void *storage = lua_newuserdata(state, sizeof(wrapper_type));
			try { new(storage) wrapper_type(lua_type_traits<Args>::get(state, Indexes)...); }
			catch (...) { lua_pop(state, 1); throw; }

			class_userdata::setmetatable<ClassType>(state);
			return 1;
		}


		template<class F>
		int call(lua_State* state, F&& f)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			typedef typename arg_index_range<fsigtype>::type index;
			return _call_apply(state, f, index(), fsigtype());
		}
		template<class F>
		bool checkArgTypes(lua_State* state, const F& f, int opt_count = 0)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			typedef typename arg_index_range<fsigtype>::type index;
			return _ctype_apply(state, index(), fsigtype(), opt_count);
		}
		template<class F>
		bool strictCheckArgTypes(lua_State* state, const F& f, int opt_count = 0)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			typedef typename arg_index_range<fsigtype>::type index;
			return _sctype_apply(state, index(), fsigtype(), opt_count);
		}

		template<class F>
		std::string argTypesName(const F& f)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			return _type_name_apply(fsigtype());
		}
		template<class F>
		int minArgCount(const F& f)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			return min_arg_count<fsigtype>::value;
		}
		template<class F>
		int maxArgCount(const F& f)
		{
			typedef typename traits::decay<F>::type ftype;
			typedef typename f_signature<ftype>::type fsigtype;
			return max_arg_count<fsigtype>::value;
		}

		template<typename ClassType, typename... Args> struct ConstructorFunction;
		template<typename ClassType, typename... Args> struct ConstructorFunction<ClassType(Args...) >
		{
			typedef constructor_signature_type<ClassType, Args...> type;
		};
		template<typename ClassType, typename... Args> struct ConstructorFunction<ClassType, ClassType(Args...) >//class type check version
		{
			typedef constructor_signature_type<ClassType, Args...> type;
		};
	}
	using nativefunction::ConstructorFunction;
}
