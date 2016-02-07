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
		namespace cpp11impl
		{
			template<class ThisType, class Res, class... FArgs, class... Args>
			Res invoke(Res(ThisType::*f)(FArgs...), ThisType* this_, Args&&... args)
			{
				return (this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class... FArgs, class... Args>
			void invoke(void (ThisType::*f)(FArgs...), ThisType* this_, Args&&... args)
			{
				(this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class Res, class... FArgs, class... Args>
			Res invoke(Res(ThisType::*f)(FArgs...)const, const ThisType* this_, Args&&... args)
			{
				return (this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class... FArgs, class... Args>
			void invoke(void (ThisType::*f)(FArgs...)const, const ThisType* this_, Args&&... args)
			{
				(this_->*f)(std::forward<Args>(args)...);
			}
			template<class F, class... Args>
			typename traits::enable_if<!traits::is_same<typename standard::result_of<F(Args...)>::type, void>::value, typename standard::result_of<F(Args...)>::type>::type
				invoke(const F& f, Args&&... args)
			{
				return f(std::forward<Args>(args)...);
			}
			template<class F, class... Args>
			typename traits::enable_if<traits::is_same<typename standard::result_of<F(Args...)>::type, void>::value, void>::type
				invoke(const F& f, Args&&... args)
			{
				f(std::forward<Args>(args)...);
			}

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

			template<class Ret, class... Args>
			struct invoke_signature_type {};

			template<class F>
			struct arg_count;
			template<class R, class... Args>
			struct arg_count<invoke_signature_type<R, Args...> > : traits::integral_constant<size_t, sizeof...(Args)>
			{};

			template<class F>
			struct arg_index_range;

			template<class R, class... Args>
			struct arg_index_range<invoke_signature_type<R, Args...> > : index_range<1, sizeof...(Args)+1>
			{};


			template <typename T>
			struct functor_f_signature : public functor_f_signature<decltype(&T::operator())> {};

			template <typename T, typename Ret, typename... Args>
			struct functor_f_signature<Ret(T::*)(Args...) const> {
				typedef invoke_signature_type<Ret, Args...> type;
			};


			template <typename T>
			struct f_signature : public functor_f_signature<decltype(&T::operator())> {};



			template <typename T, typename Ret, typename... Args>
			struct f_signature<Ret(T::*)(Args...)> {
				typedef invoke_signature_type<Ret, T*, Args...> type;
			};
			template <typename T, typename Ret, typename... Args>
			struct f_signature<Ret(T::*)(Args...) const> {
				typedef invoke_signature_type<Ret, const T*, Args...> type;
			};
			template<class Ret, class... Args>
			struct f_signature<Ret(*)(Args...)> {
				typedef invoke_signature_type<Ret, Args...> type;
			};

			template<class F, class Ret, class... Args, size_t... Indexes>
			int _call_apply(lua_State* state, const F& f, index_tuple<Indexes...>, invoke_signature_type<Ret, Args...>)
			{
				return lua_type_traits<Ret>::push(state, invoke(f, lua_type_traits<Args>::get(state, Indexes)...));
			}
			template<class F, class... Args, size_t... Indexes>
			int _call_apply(lua_State* state, const F& f, index_tuple<Indexes...>, invoke_signature_type<void, Args...>)
			{
				invoke(f, lua_type_traits<Args>::get(state, Indexes)...);
				return 0;
			}

			inline bool all_true()
			{
				return true;
			}
			template<class...Args>bool all_true(bool b, Args... args)
			{
				return b && all_true(args...);
			}

			inline std::string join(const char* delim)
			{
				return "";
			}
			template<class...Args>std::string join(const char* delim, const std::string& str, Args... args)
			{
				return str + delim + join(delim, args...);
			}


			template<class R, class... Args, size_t... Indexes>
			bool _ctype_apply(lua_State* state, index_tuple<Indexes...>, invoke_signature_type<R, Args...>)
			{
				return all_true(lua_type_traits<Args>::checkType(state, Indexes)...);
			}
			template<class R, class... Args, size_t... Indexes>
			bool _sctype_apply(lua_State* state, index_tuple<Indexes...>, invoke_signature_type<R, Args...>)
			{
				return all_true(lua_type_traits<Args>::strictCheckType(state, Indexes)...);
			}
			template<class R, class... Args>
			std::string _type_name_apply(invoke_signature_type<R, Args...>)
			{
				return join(",", typeid(Args).name()...);
			}

			///! for data member
			//@{
			template <typename T, typename MemType>
			struct f_signature<MemType T::*> {
				typedef MemType T::*type;
			};
			template<class MemType, class T>
			struct arg_count<MemType T::*> : traits::integral_constant<size_t, 1>
			{};
			template <typename T, typename MemType>
			struct arg_index_range<MemType T::*> : index_range<1, 2> {};

			template<class MemType, class T, class unusedindex>
			int _call_apply(lua_State* state, MemType T::* m, unusedindex, MemType T::*)
			{
				T* this_ = lua_type_traits<T*>::get(state, 1);
				if (!this_)
				{
					return -1;
				}
				if (lua_gettop(state) == 1)
				{
					return lua_type_traits<MemType>::push(state, this_->*m);
				}
				else
				{
					this_->*m = lua_type_traits<MemType>::get(state, 2);
					return 0;
				}
			}
			template<class MemType, class T, class unusedindex>
			bool _ctype_apply(lua_State* state, unusedindex, MemType T::*)
			{
				bool thistypecheck = lua_type_traits<T>::checkType(state, 1);
				if (thistypecheck && lua_gettop(state) == 2)
				{
					return lua_type_traits<MemType>::checkType(state, 2);
				}
				return thistypecheck;
			}
			template<class MemType, class T, class unusedindex>
			bool _sctype_apply(lua_State* state, unusedindex, MemType T::*)
			{
				bool thistypecheck = lua_type_traits<T>::strictCheckType(state, 1);
				if (thistypecheck && lua_gettop(state) == 2)
				{
					return lua_type_traits<MemType>::strictCheckType(state, 2);
				}
				return thistypecheck;
			}
			template<class MemType, class T>
			std::string _type_name_apply(MemType T::*)
			{
				return std::string(typeid(T*).name()) + ",[OPT] " + typeid(MemType).name();
			}
			//@}

			///! for constructor
			//@{
			template<class ClassType, class... Args>
			struct constructor_signature_type :invoke_signature_type<ClassType, Args...>
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
			struct arg_count<constructor_signature_type<ClassType, Args...> > : traits::integral_constant<size_t, sizeof...(Args)>
			{};

			template <class ClassType, class... Args, size_t... Indexes>
			int _call_apply(lua_State* state, constructor_signature_type<ClassType, Args...>, index_tuple<Indexes...>, constructor_signature_type<ClassType, Args...>)
			{
				typedef ObjectWrapper<ClassType> wrapper_type;
				void *storage = lua_newuserdata(state, sizeof(wrapper_type));
				new(storage) wrapper_type(lua_type_traits<Args>::get(state, Indexes)...);

				class_userdata::setmetatable<ClassType>(state);
				return 1;
			}
			//@}


			template<class F>
			int call(lua_State* state, const F& f)
			{
				typedef typename f_signature<F>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;

				return _call_apply(state, f, index(), fsigtype());
			}
			template<class F>
			bool checkArgTypes(lua_State* state, const F& f)
			{
				typedef typename f_signature<F>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;
				return _ctype_apply(state, index(), fsigtype());
			}
			template<class F>
			bool strictCheckArgTypes(lua_State* state, const F& f)
			{
				typedef typename f_signature<F>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;
				return _sctype_apply(state, index(), fsigtype());
			}

			template<class F>
			std::string argTypesName(const F& f)
			{
				typedef typename f_signature<F>::type fsigtype;
				return _type_name_apply(fsigtype());
			}
			template<class F>
			int argCount(const F& f)
			{
				typedef typename f_signature<F>::type fsigtype;
				return arg_count<fsigtype>::value;
			}
		};
		using cpp11impl::call;
		using cpp11impl::checkArgTypes;
		using cpp11impl::strictCheckArgTypes;
		using cpp11impl::argTypesName;
		using cpp11impl::argCount;
		using cpp11impl::constructor_signature_type;

	}
}
