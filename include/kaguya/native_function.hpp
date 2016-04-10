// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <vector>

#include "kaguya/config.hpp"
#include "kaguya/utility.hpp"
#include "kaguya/type.hpp"
#include "kaguya/lua_ref.hpp"

#if KAGUYA_USE_CPP11
#include "kaguya/native_function_cxx11.hpp"
#else
#include "kaguya/preprocess.hpp"
#include "kaguya/native_function_cxx03.hpp"
#endif
namespace kaguya
{
	class VariadicArgType
	{
	public:
		VariadicArgType(lua_State* state, int startIndex) :state_(state), startIndex_(startIndex), endIndex_(lua_gettop(state) + 1)
		{
			if (startIndex_ > endIndex_)
			{
				endIndex_ = startIndex_;
			}
		}

		template<typename T>
		operator std::vector<T>()const
		{
			if (startIndex_ >= endIndex_)
			{
				return std::vector<T>();
			}
			std::vector<T> result;
			result.reserve(endIndex_ - startIndex_);
			for (int index = startIndex_; index < endIndex_; ++index)
			{
				result.push_back(lua_type_traits<T>::get(state_, index));
			}
			return result;
		}

		struct reference :public Ref::StackRef, public LuaVariantImpl<reference>
		{
			reference(lua_State* s, int index) :Ref::StackRef(s, index, false)
			{
			}

			const reference* operator->()const
			{
				return this;
			}
		};
		struct iterator
		{
			iterator(lua_State* state, int index) :state_(state), stack_index_(index)
			{
			}
			reference operator*()const
			{
				return reference(state_,stack_index_);
			}
			reference operator->()const
			{
				return reference(state_, stack_index_);
			}
			const iterator& operator++()
			{
				stack_index_++;
				return *this;
			}
			iterator operator++(int)
			{
				return iterator(state_, stack_index_++);
			}

			iterator operator+=(int n)
			{
				stack_index_ += n;
				return iterator(state_, stack_index_);
			}
			bool operator==(const iterator& other)const
			{
				return state_ == other.state_ && stack_index_ == other.stack_index_;
			}
			bool operator!=(const iterator& other)const
			{
				return !(*this == other);
			}
		private:
			lua_State* state_;
			int stack_index_;
		};
		typedef iterator const_iterator;
		typedef reference const_reference;

		iterator begin()
		{
			return iterator(state_, startIndex_);
		}
		iterator end()
		{
			return iterator(state_, endIndex_);
		}
		const_iterator cbegin()
		{
			return const_iterator(state_, startIndex_);
		}
		const_iterator cend()
		{
			return const_iterator(state_, endIndex_);
		}


		template<typename T>
		typename lua_type_traits<T>::get_type at(size_t index)const
		{
			if (index >= size())
			{
				throw std::out_of_range("variadic arguments out of range");
			}
			return lua_type_traits<T>::get(state_, startIndex_ + static_cast<int>(index));
		}

		reference at(size_t index)const
		{
			if (index >= size())
			{
				throw std::out_of_range("variadic arguments out of range");
			}
			return reference(state_, startIndex_ + static_cast<int>(index));
		}

		reference operator[](size_t index)const
		{
			return reference(state_, startIndex_ + static_cast<int>(index));
		}
		size_t size()const
		{
			return endIndex_ - startIndex_;
		}
	private:
		lua_State* state_;
		int startIndex_;
		int endIndex_;
	};
	template<> struct lua_type_traits<VariadicArgType>
	{
		typedef VariadicArgType get_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return true;
		}
		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static get_type get(lua_State* l, int index)
		{
			return VariadicArgType(l, index);
		}
	};

	namespace nativefunction
	{
		struct BaseInvoker
		{
			virtual int argsCount()const = 0;
			virtual bool checktype(lua_State *state, bool strictcheck) = 0;
			virtual int invoke(lua_State *state) = 0;
			virtual std::string argumentTypeNames() = 0;
			virtual ~BaseInvoker() {}
		};

		struct FunctorType :standard::shared_ptr<BaseInvoker>
		{
			typedef standard::shared_ptr<BaseInvoker> base_ptr_;
			FunctorType() {}
			FunctorType(const standard::shared_ptr<BaseInvoker>& ptr) :base_ptr_(ptr) {}

			template<typename T>FunctorType(T f) : standard::shared_ptr<BaseInvoker>(create(f))
			{
			}
		private:
			template<typename F>
			struct FunInvoker :BaseInvoker {
				typedef F func_type;
				func_type func_;
				FunInvoker(func_type fun) :func_(fun) {}
				virtual int argsCount()const {
					return argCount(func_);
				}
				virtual bool checktype(lua_State *state, bool strictcheck) {
					if (strictcheck)
					{
						return strictCheckArgTypes(state, func_);
					}
					else
					{
						return checkArgTypes(state, func_);
					}
				}
				virtual int invoke(lua_State *state)
				{
					int count = call(state, func_);
					
					// If return pointer,return value retain first argment object
					// example: a = value.pointer_member; value = nil;
					// a has value reference
					// fixme not good implement this
					/*
					int top = lua_gettop(state);
					if (top != count)
					{
						for (int i = top - count + 1; i <= top; ++i)
						{
							ObjectWrapperBase* wrapper = object_wrapper(state, i);
							if (wrapper)
							{
								for (int arg = 1; arg < top - count + 1; ++arg)
								{
									if (lua_type(state, arg) == LUA_TUSERDATA)
									{
										//return value retain arguments
										wrapper->addRef(state, arg);
									}

								}
							}
						}
					}*/

					return count;

				}
				virtual std::string argumentTypeNames() {
					return argTypesName(func_);
				}
			};
			template<typename F>
			static base_ptr_ create(F fun)
			{
				KAGUYA_STATIC_ASSERT(nativefunction::is_callable<typename standard::decay<F>::type>::value, "argument need callable");

				typedef FunInvoker<F> InvokerType;
				return base_ptr_(new InvokerType(fun));
			}
		};


		inline FunctorType* pick_match_function(lua_State *l)
		{
			int argcount = lua_gettop(l);
			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));

			if (overloadnum == 1)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(2)));

				//skip type check for performance
//				if (!fun || !(*fun) || !(*fun)->checktype(l, false))
//				{
//					return 0;
//				}
				return fun;
			}
			FunctorType* weak_match = 0;
			FunctorType* argcount_unmatch = 0;
			int minArgDiv = 0;
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if (!fun || !(*fun))
				{
					continue;
				}
				int fnarg = (*fun)->argsCount();
				int argDiv = argcount - fnarg;
				if (!argDiv && (*fun)->checktype(l, true))
				{
					return fun;
				}
				else if (!weak_match && (!argDiv || !argcount_unmatch || argDiv < minArgDiv) && (*fun)->checktype(l, false))
				{
					if (!argDiv)
					{
						weak_match = fun;
					}
					else
					{
						argcount_unmatch = fun;
                        minArgDiv = argDiv;
					}
				}
			}
			return weak_match ? weak_match : argcount_unmatch;
		}



		inline std::string argmentTypes(lua_State *state)
		{
			int top = lua_gettop(state);

			std::string result = "";

			for (int i = 1; i <= top; i++) {
				if (i != 1)
				{
					result += ",";
				}


				ObjectWrapperBase* object = object_wrapper(state, i);
				if (object)
				{
					result += object->type().name();
				}
				else
				{
					result += lua_typename(state, lua_type(state, i));
				}
			}
			return result;
		}

		inline std::string build_arg_error_message(lua_State *l)
		{
			std::string message = "Argument mismatch:" + argmentTypes(l) + "\t candidate is:\n";

			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if (!fun || !(*fun))
				{
					continue;
				}
				message += std::string("\t\t") + (*fun)->argumentTypeNames() + "\n";
			}
			return message;
		}
		inline int functor_dispatcher(lua_State *l)
		{
			FunctorType* fun = pick_match_function(l);
			if (fun && (*fun))
			{
				try {
					return (*fun)->invoke(l);
				}
				catch (LuaTypeMismatch &) {
					util::traceBack(l,(std::string("maybe...") + build_arg_error_message(l)).c_str());
				}
				catch (std::exception & e) {
					util::traceBack(l, e.what());
				}
				catch (...) {
					util::traceBack(l, "Unknown exception");
				}
			}
			else
			{
				util::traceBack(l, build_arg_error_message(l).c_str());
			}
			return lua_error(l);
		}

		inline int functor_destructor(lua_State *state)
		{
			FunctorType* f = class_userdata::test_userdata<FunctorType>(state, 1);
			if (f)
			{
				f->~FunctorType();
			}
			return 0;
		}
		inline void reg_functor_destructor(lua_State* state)
		{
			if (class_userdata::newmetatable<FunctorType>(state))
			{
				lua_pushcclosure(state, &functor_destructor, 0);
				lua_setfield(state, -2, "__gc");
				lua_setfield(state, -1, "__index");
			}
		}
	}

	typedef nativefunction::FunctorType FunctorType;


	typedef std::vector<FunctorType> FunctorOverloadType;

	//deprecate
	template<typename T>
	inline FunctorType lua_function(T f)
	{
		return FunctorType(f);
	}

	template<typename T>
	inline FunctorType function(const T& f)
	{
		return FunctorType(f);
	}

	template<typename FTYPE, typename T>
	inline FunctorType function(const T&  f)
	{
		return FunctorType(standard::function<FTYPE>(f));
	}
#if KAGUYA_USE_CPP11
	template<typename T>
	inline FunctorType function(T&& f)
	{
		return FunctorType(std::forward<T>(f));
	}

	template<typename FTYPE, typename T>
	inline FunctorType function(T&& f)
	{
		return FunctorType(standard::function<FTYPE>(std::forward<T>(f)));
	}
#endif

#if KAGUYA_USE_CPP11
	namespace detail
	{
		inline void push_back_r(FunctorOverloadType& v)
		{
		}
		template<typename F, typename... Functions>
		void push_back_r(FunctorOverloadType& v, const F& f, const Functions&... fns)
		{
			v.reserve(sizeof...(fns));
			v.push_back(f);
			push_back_r(v, fns...);
		}
		template<typename F, typename... Functions>
		void push_back_r(FunctorOverloadType& v, F&& f, const Functions&... fns)
		{
			v.reserve(sizeof...(fns));
			v.push_back(std::forward<F>(f));
			push_back_r(v, fns...);
		}
	}
	template<typename... Functions>
	inline FunctorOverloadType overload(const Functions&... fns)
	{
		FunctorOverloadType v;
		detail::push_back_r(v, fns...);
		return v;
	}
#else

#define KAGUYA_DEF_TEMPLATE(N) KAGUYA_PP_CAT(typename F,N)
#define KAGUYA_PUSH_DEF(N) v.push_back(KAGUYA_PP_CAT(f,N));
#define KAGUYA_ARG_DEF(N) KAGUYA_PP_CAT(F,N) KAGUYA_PP_CAT(f,N)
#define KAGUYA_FOVERLOAD_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_DEF_TEMPLATE)>\
		FunctorOverloadType overload(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_ARG_DEF))\
		{\
			FunctorOverloadType v;\
			v.reserve(N);\
			KAGUYA_PP_REPEAT(N,KAGUYA_PUSH_DEF);\
			return v;\
		}
	KAGUYA_PP_REPEAT_DEF(9, KAGUYA_FOVERLOAD_DEF)
#undef KAGUYA_DEF_TEMPLATE
#undef KAGUYA_PUSH_DEF
#undef KAGUYA_FOVERLOAD_DEF
#endif

	template<>	struct lua_type_traits<FunctorType> {
		typedef FunctorType get_type;
		typedef FunctorType push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return 0 != class_userdata::test_userdata<FunctorType>(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return 0 != class_userdata::test_userdata<FunctorType>(l, index);
		}
		static FunctorType get(lua_State* l, int index)
		{
			FunctorType* ptr = class_userdata::test_userdata<FunctorType>(l, index);
			if (ptr)
			{
				return *ptr;
			}
			return FunctorType();
		}
		static int push(lua_State* l, const FunctorType& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(FunctorType));
			new(storage) FunctorType(f);
			class_userdata::setmetatable<FunctorType>(l);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#if KAGUYA_USE_RVALUE_REFERENCE
		static int push(lua_State* l, FunctorType&& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(FunctorType));
			new(storage) FunctorType(std::forward<FunctorType>(f));
			class_userdata::setmetatable<FunctorType>(l);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#endif
	};

	template<>	struct lua_type_traits<const FunctorType&> :lua_type_traits<FunctorType> {};
	
	//specialize for c function
	template<typename T> struct lua_type_traits < T
		, typename traits::enable_if<traits::is_function<typename traits::remove_pointer<T>::type>::value>::type > :lua_type_traits<FunctorType> {};



	template<>	struct lua_type_traits<FunctorOverloadType> {
		typedef const FunctorOverloadType& push_type;

		static int push(lua_State* l, push_type fns)
		{
			lua_pushnumber(l, static_cast<lua_Number>(fns.size()));
			for (FunctorOverloadType::const_iterator f = fns.begin(); f != fns.end(); ++f)
			{
				void *storage = lua_newuserdata(l, sizeof(FunctorType));
				new(storage) FunctorType(*f);
				class_userdata::setmetatable<FunctorType>(l);
			}
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, static_cast<int>(fns.size() + 1));
			return 1;
		}
	};

}
