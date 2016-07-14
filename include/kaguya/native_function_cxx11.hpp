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
			Res invoke_fn(Res(ThisType::*f)(FArgs...), ThisType* this_, Args&&... args)
			{
				if (!this_)
				{
					throw LuaTypeMismatch("type mismatch!!");
				}
				return (this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class... FArgs, class... Args>
			void invoke_fn(void (ThisType::*f)(FArgs...), ThisType* this_, Args&&... args)
			{
				if (!this_)
				{
					throw LuaTypeMismatch("type mismatch!!");
				}
				(this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class Res, class... FArgs, class... Args>
			Res invoke_fn(Res(ThisType::*f)(FArgs...)const, const ThisType* this_, Args&&... args)
			{
				if (!this_)
				{
					throw LuaTypeMismatch("type mismatch!!");
				}
				return (this_->*f)(std::forward<Args>(args)...);
			}

			template<class ThisType, class... FArgs, class... Args>
			void invoke_fn(void (ThisType::*f)(FArgs...)const, const ThisType* this_, Args&&... args)
			{
				(this_->*f)(std::forward<Args>(args)...);
			}
			template<class F, class... Args>
			//MSVC 2013 std::result_of is broken.
			auto invoke_fn(F&& f, Args&&... args) -> decltype(f(std::forward<Args>(args)...))
			{
				return f(std::forward<Args>(args)...);
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
			struct functor_f_signature {};

			template <typename T, typename Ret, typename... Args>
			struct functor_f_signature<Ret(T::*)(Args...) const> {
				typedef invoke_signature_type<Ret, Args...> type;
			};
			template <typename T, typename Ret, typename... Args>
			struct functor_f_signature<Ret(T::*)(Args...)> {
				typedef invoke_signature_type<Ret, Args...> type;
			};

#if _MSC_VER
			template <typename T>
			struct f_signature : public functor_f_signature<decltype(&T::operator())> {};
#else

			template <typename T, typename Enable=void>
			struct f_signature {};

			template < typename T, typename = void>
			struct has_operator_fn :std::false_type {};
			template < typename T >
			struct has_operator_fn<T, typename std::enable_if<!std::is_same<void, decltype(&T::operator())>::value>::type> :std::true_type {};

			template <typename T>
			struct f_signature<T,typename std::enable_if<has_operator_fn<T>::value>::type>
				: public functor_f_signature<decltype(&T::operator())> {};
#endif


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
			int _call_apply(lua_State* state, F&& f, index_tuple<Indexes...>, invoke_signature_type<Ret, Args...>)
			{
				return util::push_args(state, invoke_fn(f, lua_type_traits<Args>::get(state, Indexes)...));
			}
			template<class F, class... Args, size_t... Indexes>
			int _call_apply(lua_State* state, F&& f, index_tuple<Indexes...>, invoke_signature_type<void, Args...>)
			{
				invoke_fn(f, lua_type_traits<Args>::get(state, Indexes)...);
				return 0;
			}

			inline bool all_true()
			{
				return true;
			}
			template<class Arg,class...Args>bool all_true(Arg f, Args... args)
			{	//check from backward and lazy evaluation
				return all_true(args...) && bool(f);
			}

			inline void join(std::string& result, const char* delim)
			{
			}
			template<class Arg,class...Args>void join(std::string& result,const char* delim, const Arg& str,const Args&... args)
			{
				result += str;
				result += delim;
				join(result,delim, args...);
			}

			template<typename T, size_t Index>
			struct _wcheckeval
			{
				_wcheckeval(lua_State* s) :state(s) {}
				lua_State* state;
				operator bool()
				{
					return lua_type_traits<T>::checkType(state, Index);
				}
			};

			template<typename T,size_t Index>
			struct _scheckeval
			{
				_scheckeval(lua_State* s) :state(s) {}
				lua_State* state;
				operator bool()
				{
					return lua_type_traits<T>::strictCheckType(state, Index);
				}
			};

			template<class R, class... Args, size_t... Indexes>
			bool _ctype_apply(lua_State* state, index_tuple<Indexes...>, invoke_signature_type<R, Args...>)
			{
				return all_true(_wcheckeval<Args, Indexes>(state)...);
			}
			template<class R, class... Args, size_t... Indexes>
			bool _sctype_apply(lua_State* state, index_tuple<Indexes...>, invoke_signature_type<R, Args...>)
			{
				return all_true(_scheckeval<Args, Indexes>(state)...);
			}
			template<class R, class... Args>
			std::string _type_name_apply(invoke_signature_type<R, Args...>)
			{
				std::string result;
				join(result,",", typeid(Args).name()...);
				return result;
			}

			///! for data member
			//@{
			template <typename T, typename MemType>
			struct f_signature<MemType T::*> {
				typedef MemType T::*type;
			};
			template<class MemType, class T>
			struct arg_count<MemType T::*> : traits::integral_constant<size_t, 2>
			{};
			template <typename T, typename MemType>
			struct arg_index_range<MemType T::*> : index_range<1, 2> {};

			template<class MemType, class T, class unusedindex>
			int _call_apply(lua_State* state, MemType T::* m, unusedindex, MemType T::*)
			{
				T* this_ = lua_type_traits<T*>::get(state, 1);
				if (lua_gettop(state) == 1)
				{
					if (!this_)
					{
						const T* this_ = lua_type_traits<const T*>::get(state, 1);
						if (!this_)
						{
							throw LuaTypeMismatch("type mismatch!!");
						}
						if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
						{
							return util::push_args(state, standard::reference_wrapper<const MemType>(this_->*m));
						}
						else
						{
							return util::push_args(state, this_->*m);
						}
					}
					else
					{
						if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
						{
							return util::push_args(state, standard::reference_wrapper<MemType>(this_->*m));
						}
						else
						{
							return util::push_args(state, this_->*m);
						}
					}
				}
				else
				{
					if (!this_)
					{
						throw LuaTypeMismatch("type mismatch!!");
					}
					this_->*m = lua_type_traits<MemType>::get(state, 2);
					return 0;
				}
			}
			template<class MemType, class T, class unusedindex>
			bool _ctype_apply(lua_State* state, unusedindex, MemType T::*)
			{
				if (lua_gettop(state) >= 2)
				{
					//setter typecheck
					return lua_type_traits<MemType>::checkType(state, 2) && lua_type_traits<T>::checkType(state, 1);
				}
				//getter typecheck
				return  lua_type_traits<T>::checkType(state, 1);
			}
			template<class MemType, class T, class unusedindex>
			bool _sctype_apply(lua_State* state, unusedindex, MemType T::*)
			{
				if (lua_gettop(state) >= 2)
				{
					//setter typecheck
					return lua_type_traits<MemType>::strictCheckType(state, 2) && lua_type_traits<T>::strictCheckType(state, 1);
				}
				//getter typecheck
				return lua_type_traits<T>::strictCheckType(state, 1);
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
				try { new(storage) wrapper_type(lua_type_traits<Args>::get(state, Indexes)...); }catch (...) { lua_pop(state, 1); throw; }

				class_userdata::setmetatable<ClassType>(state);
				return 1;
			}
			//@}


			template<class F>
			int call(lua_State* state, F&& f)
			{
				typedef typename traits::decay<F>::type ftype;
				typedef typename f_signature<ftype>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;

				return _call_apply(state, f, index(), fsigtype());
			}
			template<class F>
			bool checkArgTypes(lua_State* state, const F& f)
			{
				typedef typename traits::decay<F>::type ftype;
				typedef typename f_signature<ftype>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;
				return _ctype_apply(state, index(), fsigtype());
			}
			template<class F>
			bool strictCheckArgTypes(lua_State* state, const F& f)
			{
				typedef typename traits::decay<F>::type ftype;
				typedef typename f_signature<ftype>::type fsigtype;
				typedef typename arg_index_range<fsigtype>::type index;
				return _sctype_apply(state, index(), fsigtype());
			}

			template<class F>
			std::string argTypesName(const F& f)
			{
				typedef typename traits::decay<F>::type ftype;
				typedef typename f_signature<ftype>::type fsigtype;
				return _type_name_apply(fsigtype());
			}
			template<class F>
			int argCount(const F& f)
			{
				typedef typename traits::decay<F>::type ftype;
				typedef typename f_signature<ftype>::type fsigtype;
				return arg_count<fsigtype>::value;
			}

			template< typename T, typename Enable = void>
			struct is_callable : traits::integral_constant<bool, false> {};
			template< typename T>
			struct is_callable<T, typename traits::enable_if<
				!traits::is_same<void, typename f_signature<T>::type>::value
				, void>::type > : traits::integral_constant<bool, true> {};

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
		using cpp11impl::call;
		using cpp11impl::checkArgTypes;
		using cpp11impl::strictCheckArgTypes;
		using cpp11impl::argTypesName;
		using cpp11impl::argCount;
		using cpp11impl::constructor_signature_type;
		using cpp11impl::ConstructorFunction;
		using cpp11impl::is_callable;
	}
	using nativefunction::ConstructorFunction;
}
