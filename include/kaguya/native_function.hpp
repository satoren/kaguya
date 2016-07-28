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

	struct FunctionImpl
	{
		virtual int invoke(lua_State *state) = 0;
		virtual std::string argTypeNames()const = 0;
		virtual bool checkArgTypes(lua_State* state)const = 0;
		virtual bool strictCheckArgTypes(lua_State* state)const = 0;
		virtual int minArgCount()const=0;
		virtual int maxArgCount()const=0;
		virtual ~FunctionImpl() {}
	};
	struct PolymorphicInvoker
	{
		typedef standard::shared_ptr<FunctionImpl> holder_type;
		PolymorphicInvoker(const holder_type& fptr) :fnc(fptr) {}
		int invoke(lua_State *state)const { return fnc->invoke(state); }
		std::string argTypeNames()const { return fnc->argTypeNames(); }
		bool checkArgTypes(lua_State* state)const { return fnc->checkArgTypes(state); }
		bool strictCheckArgTypes(lua_State* state)const { return fnc->strictCheckArgTypes(state); }
		int minArgCount()const { return fnc->minArgCount(); }
		int maxArgCount()const { return fnc->maxArgCount(); }
		~PolymorphicInvoker() {}
	private:
		holder_type fnc;
	};
	struct PolymorphicMemberInvoker : PolymorphicInvoker{
		PolymorphicMemberInvoker(const holder_type& fptr) :PolymorphicInvoker(fptr) {}
	};

	namespace nativefunction
	{
		template<size_t INDEX,typename F>
		typename lua_type_traits<typename util::ArgumentType<INDEX, F>::type>::get_type			
			getArgument(lua_State* state)
		{
			return lua_type_traits<typename util::ArgumentType<INDEX, F>::type>::get(state, INDEX + 1);
		}

		/*
		template<class MemType, class T>
		struct PropertyAccessor
		{
			PropertyAccessor(MemType T::* ptr) :dataptr(ptr){}
			MemType T::* dataptr;
		};

		///! for data member
		template<class MemType, class T>
		int call(lua_State* state,const PropertyAccessor<MemType,T>& m)
		{
			T* this_ = lua_type_traits<T*>::get(state, 1);
			if (lua_gettop(state) == 1)
			{
				if (!this_)
				{
					const T& this_ = lua_type_traits<const T&>::get(state, 1);
					if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
					{
						return util::push_args(state, standard::reference_wrapper<const MemType>(this_.*(m.dataptr)));
					}
					else
					{
						return util::push_args(state, this_.*(m.dataptr));
					}
				}
				else
				{
					if (is_usertype<MemType>::value && !traits::is_pointer<MemType>::value)
					{
						return util::push_args(state, standard::reference_wrapper<MemType>(this_->*(m.dataptr)));
					}
					else
					{
						return util::push_args(state, this_->*(m.dataptr));
					}
				}
			}
			else
			{
				if (!this_)
				{
					throw LuaTypeMismatch("type mismatch!!");
				}
				this_->*(m.dataptr) = lua_type_traits<MemType>::get(state, 2);
				return 0;
			}
		}
		template<class MemType, class T>
		int call(lua_State* state, PropertyAccessor<MemType, T>& m)
		{
			return call(state, static_cast<const PropertyAccessor<MemType, T>&>(m));
		}


		template<class MemType, class T>
		bool checkArgTypes(lua_State* state, const PropertyAccessor<MemType, T>& m, int opt_count = 0)
		{
			if (lua_gettop(state) >= 2)
			{
				//setter typecheck
				return lua_type_traits<MemType>::checkType(state, 2) && lua_type_traits<T>::checkType(state, 1);
			}
			//getter typecheck
			return  lua_type_traits<T>::checkType(state, 1);
		}
		template<class MemType, class T>
		bool strictCheckArgTypes(lua_State* state, const PropertyAccessor<MemType, T>& m, int opt_count = 0)
		{
			if (lua_gettop(state) == 2)
			{
				//setter typecheck
				return lua_type_traits<MemType>::strictCheckType(state, 2) && lua_type_traits<T>::strictCheckType(state, 1);
			}
			//getter typecheck
			return  lua_type_traits<T>::strictCheckType(state, 1);
		}
		template<class MemType, class T>
		std::string argTypesName(const PropertyAccessor<MemType, T>&)
		{
			return std::string(typeid(T*).name()) + ",[OPT] " + typeid(MemType).name();
		}
		template<class MemType, class T>
		int minArgCount(const PropertyAccessor<MemType, T>&)
		{
			return 1;
		}
		template<class MemType, class T>
		int maxArgCount(const PropertyAccessor<MemType, T>&)
		{
			return 2;
		}
		template<class MemType, class T>
		struct is_callable<PropertyAccessor<MemType,T> > : traits::integral_constant<bool, true> {};

		*/

		inline int call(lua_State* state, const PolymorphicInvoker& f)
		{
			return f.invoke(state);
		}
		inline int call(lua_State* state, PolymorphicInvoker& f)
		{
			return f.invoke(state);
		}
		inline bool checkArgTypes(lua_State* state, const PolymorphicInvoker& f)
		{
			return f.checkArgTypes(state);
		}
		inline bool strictCheckArgTypes(lua_State* state, const PolymorphicInvoker& f)
		{
			return f.strictCheckArgTypes(state);
		}
		inline std::string argTypesName(const PolymorphicInvoker& f)
		{
			return f.argTypeNames();
		}
		inline int minArgCount(const PolymorphicInvoker& f)
		{
			return f.minArgCount();
		}
		inline int maxArgCount(const PolymorphicInvoker& f)
		{
			return f.maxArgCount();
		}

		template<>
		struct is_callable<PolymorphicInvoker>:traits::integral_constant<bool, true> {};


		inline int call(lua_State* state, const PolymorphicMemberInvoker& f)
		{
			return f.invoke(state);
		}
		inline int call(lua_State* state, PolymorphicMemberInvoker& f)
		{
			return f.invoke(state);
		}
		inline bool checkArgTypes(lua_State* state, const PolymorphicMemberInvoker& f)
		{
			return f.checkArgTypes(state);
		}
		inline bool strictCheckArgTypes(lua_State* state, const PolymorphicMemberInvoker& f)
		{
			return f.strictCheckArgTypes(state);
		}
		inline std::string argTypesName(const PolymorphicMemberInvoker& f)
		{
			return f.argTypeNames();
		}
		inline int minArgCount(const PolymorphicMemberInvoker& f)
		{
			return f.minArgCount();
		}
		inline int maxArgCount(const PolymorphicMemberInvoker& f)
		{
			return f.maxArgCount();
		}

		template<>
		struct is_callable<PolymorphicMemberInvoker> :traits::integral_constant<bool, true> {};
	}

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

		struct reference :public Ref::StackRef, public detail::LuaVariantImpl<reference>
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
				return reference(state_, stack_index_);
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
			/**
			* @name relational operators
			* @brief
			*/
			//@{
			bool operator==(const iterator& other)const
			{
				return state_ == other.state_ && stack_index_ == other.stack_index_;
			}
			bool operator!=(const iterator& other)const
			{
				return !(*this == other);
			}
			//@}
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
	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for VariadicArgType
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
		static const int MAX_OVERLOAD_SCORE = 255;
		template<typename Fn>uint8_t compute_function_matching_score(lua_State* state, const Fn& fn)
		{
			int argcount = lua_gettop(state);


			if (strictCheckArgTypes(state, fn))
			{
				const int minargcount = minArgCount(fn);
				const int maxargcount = maxArgCount(fn);
				if (minargcount <= argcount && maxargcount >= argcount)
				{
					return MAX_OVERLOAD_SCORE;
				}
				else
				{
					int diff = std::min(std::abs(argcount - minargcount), std::abs(argcount - maxargcount));
					return std::max(100 - diff, 51);
				}
			}
			else if (checkArgTypes(state, fn))
			{
				const int minargcount = minArgCount(fn);
				const int maxargcount = maxArgCount(fn);
				if (minargcount <= argcount && maxargcount >= argcount)
				{
					return 200;
				}
				else
				{
					int diff = std::min(std::abs(argcount - minargcount), std::abs(argcount - maxargcount));
					return std::max(50 - diff, 1);
				}
			}
			else
			{
				return 0;
			}
		}
		inline int pushArgmentTypeNames(lua_State *state, int top)
		{
			for (int i = 1; i <= top; i++) {
				if (i != 1)
				{
					lua_pushliteral(state, ",");
				}

				int type = lua_type(state, i);
				if (type == LUA_TUSERDATA)
				{
					int nametype = luaL_getmetafield(state, i, "__name");
					if (nametype != LUA_TSTRING)
					{
						lua_pop(state, 1);
						lua_pushstring(state, lua_typename(state, type));
					}
				}
				else
				{
					lua_pushstring(state, lua_typename(state, type));
				}
			}
			return lua_gettop(state) - top;
		}
	}



#if KAGUYA_USE_CPP11

	namespace detail
	{
		template<typename Fn, typename... Functions> void function_match_scoring(lua_State* state, uint8_t* score_array, int current_index, const Fn& fn)
		{
			score_array[current_index] = nativefunction::compute_function_matching_score(state, fn);
		}
		template<typename Fn, typename... Functions> void function_match_scoring(lua_State* state, uint8_t* score_array, int current_index, const Fn& fn, const Functions&... fns)
		{
			score_array[current_index] = nativefunction::compute_function_matching_score(state, fn);
			if (score_array[current_index] < nativefunction::MAX_OVERLOAD_SCORE)
			{
				function_match_scoring(state, score_array, current_index + 1, fns...);
			}
		}
		template<typename... Functions> int best_function_index(lua_State* state, const Functions&... fns)
		{
			static const int fncount = sizeof...(fns);
			uint8_t score[fncount] = {};
			function_match_scoring(state, score, 0, fns...);
			uint8_t best_score = 0;
			int best_score_index = -1;
			for (int i = 0; i < fncount; ++i)
			{
				if (best_score < score[i])
				{
					best_score = score[i];
					best_score_index = i;
					if (best_score == nativefunction::MAX_OVERLOAD_SCORE)
					{
						break;
					}
				}
			}
			return best_score_index;
		}
		template<typename Fn> int invoke_index(lua_State* state, int index, int current_index, Fn&& fn)
		{
			return nativefunction::call(state, fn);
		}
		template<typename Fn, typename... Functions> int invoke_index(lua_State* state, int index, int current_index, Fn&& fn, Functions&&... fns)
		{
			if (index == current_index)
			{
				return nativefunction::call(state, fn);
			}
			else
			{
				return invoke_index(state, index, current_index + 1, fns...);
			}
		}

		template<typename Fun> int best_match_invoke(lua_State* state, Fun&& fn)
		{
			return nativefunction::call(state, fn);
		}

		template<typename Fun, typename... Functions> int best_match_invoke(lua_State* state, Fun&& fn, Functions&&... fns)
		{
			int index = best_function_index(state, fn, fns...);
			if (index >= 0)
			{
				assert(size_t(index) <= sizeof...(fns));
				return invoke_index(state, index, 0, fn, fns...);
			}
			else
			{
				throw LuaTypeMismatch("type mismatch!!");
			}
			return 0;
		}

		template<typename TupleType, std::size_t ...S> int invoke_tuple_impl(lua_State* state, TupleType&& tuple, nativefunction::index_tuple<S...>)
		{
			return best_match_invoke(state, std::get<S>(tuple)...);
		}
		template<typename TupleType> int invoke_tuple(lua_State* state, TupleType&& tuple)
		{
			typedef typename std::decay<TupleType>::type ttype;

			typedef typename nativefunction::index_range<0, std::tuple_size<ttype>::value>::type indexrange;

			return invoke_tuple_impl(state, tuple, indexrange());
		}

		template<typename Fun> void  push_arg_typename(lua_State *state,const Fun& fn)
		{
			lua_pushliteral(state, "\t\t");
			lua_pushstring(state, nativefunction::argTypesName(fn).c_str());
			lua_pushliteral(state, "\n");
		}

		template<typename Fun, typename... Functions> void  push_arg_typename(lua_State *state, const Fun& fn, const Functions&... fns)
		{
			lua_pushliteral(state, "\t\t");
			lua_pushstring(state, nativefunction::argTypesName(fn).c_str());
			lua_pushliteral(state, "\n");
			push_arg_typename(state,fns...);
		}
		template<typename TupleType, std::size_t ...S> void  push_arg_typename_tuple_impl(lua_State *state, TupleType&& tuple, nativefunction::index_tuple<S...>)
		{
			return push_arg_typename(state,std::get<S>(tuple)...);
		}
		template<typename TupleType>void push_arg_typename_tuple(lua_State *state, TupleType&& tuple)
		{
			typedef typename std::decay<TupleType>::type ttype;
			typedef typename nativefunction::index_range<0, std::tuple_size<ttype>::value>::type indexrange;

			return push_arg_typename_tuple_impl(state,tuple, indexrange());
		}
	}


#else

	namespace detail
	{
#define KAGUYA_FUNCTION_SCOREING(N) if (currentbestscore < nativefunction::MAX_OVERLOAD_SCORE) {\
		int score = nativefunction::compute_function_matching_score(state, standard::get<N - 1>(tuple));\
		if (currentbestscore < score) {\
			currentbestscore = score;\
			currentbestindex = N;\
		}\
		}
#define KAGUYA_FUNCTION_INVOKE(N) if (currentbestindex == N) {\
			 return nativefunction::call(state,  standard::get<N - 1>(tuple));\
		}\


#define KAGUYA_ARG_PUSH_TYPENAMES(N)lua_pushliteral(state, "\t\t"); lua_pushstring(state, nativefunction::argTypesName(standard::get<N-1>(tuple)).c_str());lua_pushliteral(state, "\n");
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>
#define KAGUYA_TUPLE_INVOKE_DEF(N) \
		KAGUYA_TEMPLATE_PARAMETER(N)\
		int invoke_tuple(lua_State* state, standard::tuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>& tuple)\
		{\
			if(N==1){return nativefunction::call(state,  standard::get<0>(tuple));}\
			int32_t currentbestscore = 0;\
			int32_t currentbestindex = -1;\
			KAGUYA_PP_REPEAT(N, KAGUYA_FUNCTION_SCOREING);\
			KAGUYA_PP_REPEAT(N, KAGUYA_FUNCTION_INVOKE);\
			throw LuaTypeMismatch("type mismatch!!"); \
		}\
		KAGUYA_TEMPLATE_PARAMETER(N)\
		void push_arg_typename_tuple(lua_State *state,standard::tuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>& tuple)\
		{\
			KAGUYA_PP_REPEAT(N, KAGUYA_ARG_PUSH_TYPENAMES);\
		}\

		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_TUPLE_INVOKE_DEF)
#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_TUPLE_INVOKE_DEF
#undef KAGUYA_ARG_TYPENAMES
#undef KAGUYA_FUNCTION_INVOKE
#undef KAGUYA_FUNCTION_SCOREING

			template<typename TupleType> int invoke_tuple(lua_State* state, TupleType& tuple)
		{
			return 0;
		}
	}
#endif



	template<typename FunctionTuple>
	struct FunctionInvokerType
	{
		FunctionTuple functions;
		FunctionInvokerType(const FunctionTuple& t) :functions(t) {}
	};
	template<typename T>
	inline FunctionInvokerType<standard::tuple<T> > function(T f)
	{
		KAGUYA_STATIC_ASSERT(nativefunction::is_callable<typename traits::decay<T>::type>::value, "argument need callable");
		return FunctionInvokerType<standard::tuple<T> >(standard::tuple<T>(f));
	}
	//specialize for lua_CFunction
	inline lua_CFunction function(lua_CFunction f)
	{
		return f;
	}


	template<typename FTYPE, typename T>
	inline FunctionInvokerType<standard::tuple<standard::function<FTYPE> > > function(T f)
	{
		return FunctionInvokerType<standard::tuple<standard::function<FTYPE> > >(standard::tuple<standard::function<FTYPE> >(standard::function<FTYPE>(f)));
	}
#if KAGUYA_USE_CPP11


	template<typename... Functions>
	FunctionInvokerType<standard::tuple<Functions...> > overload(Functions... fns)
	{
		return FunctionInvokerType<standard::tuple<Functions...> >(standard::tuple<Functions...>(fns...));
	}
#else
#define KAGUYA_DEF_TEMPLATE(N) KAGUYA_PP_CAT(typename F,N)
#define KAGUYA_TEMPLATE_ARG_DEF(N) KAGUYA_PP_CAT(F,N)
#define KAGUYA_TUPLE_ARG_DEF(N) KAGUYA_PP_CAT(f,N)
#define KAGUYA_ARG_DEF(N) KAGUYA_PP_CAT(F,N) KAGUYA_PP_CAT(f,N)
#define KAGUYA_FOVERLOAD_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_DEF_TEMPLATE)>\
		FunctionInvokerType<standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_TEMPLATE_ARG_DEF)> > overload(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_ARG_DEF))\
		{\
			typedef typename standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_TEMPLATE_ARG_DEF)> ttype;\
			return FunctionInvokerType<ttype>(ttype(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_TUPLE_ARG_DEF)));\
		}
	KAGUYA_PP_REPEAT_DEF(9, KAGUYA_FOVERLOAD_DEF)
#undef KAGUYA_DEF_TEMPLATE
#undef KAGUYA_TEMPLATE_ARG_DEF
#undef KAGUYA_TUPLE_ARG_DEF
#undef KAGUYA_ARG_DEF
#undef KAGUYA_FOVERLOAD_DEF
#endif


	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for FunctionInvokerType
	template<typename FunctionTuple> struct lua_type_traits<FunctionInvokerType<FunctionTuple> >
	{
		typedef FunctionInvokerType<FunctionTuple> userdatatype;
		typedef const FunctionInvokerType<FunctionTuple>& push_type;

		static const char* build_arg_error_message(lua_State *state,const char* msg, FunctionTuple* tuple)
		{
			int stack_top = lua_gettop(state);
			if (msg) { lua_pushstring(state, msg); }
			lua_pushliteral(state, "Argument mismatch:");
			nativefunction::pushArgmentTypeNames(state, stack_top);

			lua_pushliteral(state, "\t candidate is:\n");
			detail::push_arg_typename_tuple(state, *tuple);

			lua_concat(state, lua_gettop(state) - stack_top);
			return lua_tostring(state,-1);
		}

		static int invoke(lua_State *state)
		{
			FunctionTuple* t = static_cast<FunctionTuple*>(lua_touserdata(state, lua_upvalueindex(1)));

			if (t)
			{
				try {
					return detail::invoke_tuple(state, *t);
				}
				catch (LuaTypeMismatch &) {
					util::traceBack(state, build_arg_error_message(state, "maybe...", t));
				}
				catch (std::exception & e) {
					util::traceBack(state, e.what());
				}
				catch (...) {
					util::traceBack(state, "Unknown exception");
				}
			}
			return lua_error(state);
		}

		inline static int tuple_destructor(lua_State *state)
		{
			FunctionTuple* f = static_cast<FunctionTuple*>(lua_touserdata(state, 1));
			if (f)
			{
				f->~FunctionTuple();
			}
			return 0;
		}

		static int push(lua_State* state, push_type fns)
		{
			void* ptr = lua_newuserdata(state, sizeof(FunctionTuple));
			new(ptr) FunctionTuple(fns.functions);
			lua_createtable(state, 0, 2);
			lua_pushcclosure(state, &tuple_destructor, 0);
			lua_setfield(state, -2, "__gc");
			lua_pushvalue(state, -1);
			lua_setfield(state, -1, "__index");
			lua_setmetatable(state, -2);
			lua_pushcclosure(state, &invoke, 1);

			return 1;
		}
	};


	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for c function
	template<typename T> struct lua_type_traits < T
		, typename traits::enable_if<traits::is_function<typename traits::remove_pointer<T>::type>::value>::type >
	{
		static int push(lua_State* l, T f)
		{
			return util::one_push(l, kaguya::function(f));
		}
	};
	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for lua_CFunction
	template<> struct lua_type_traits < lua_CFunction >
	{
		typedef lua_CFunction push_type;
		typedef lua_CFunction get_type;
		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_iscfunction(l, index) != 0;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_iscfunction(l, index) != 0;
		}
		static get_type get(lua_State* l, int index)
		{
			return lua_tocfunction(l,index);
		}
		static int push(lua_State* l, lua_CFunction f)
		{
			lua_pushcfunction(l, f);
			return 1;
		}
	};



	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for std::function or boost::function
	template<typename T> struct lua_type_traits<standard::function<T> > {
		typedef const standard::function<T>& push_type;
		typedef standard::function<T> get_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TFUNCTION;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TFUNCTION;
		}
		static get_type get(lua_State* l, int index)
		{
			if (!l || lua_type(l, index) != LUA_TFUNCTION) {
				return get_type();
			}
			lua_pushvalue(l, index);
			return get_type(LuaFunction(l, StackTop()));
		}

		static int push(lua_State* l, push_type v)
		{
			return util::one_push(l, kaguya::function(v));
		}
	};

	
	template<typename F, typename Ret = typename util::FunctionSignature<F>::type::result_type>
	struct OverloadFunctionImpl : kaguya::FunctionImpl
	{
		typedef Ret result_type;
		typedef typename util::FunctionSignature<F>::type::c_function_type c_function_type;

		virtual result_type invoke_type(lua_State *state) = 0;

		virtual int invoke(lua_State *state)
		{
			return util::push_args(state, invoke_type(state));
		}
		virtual std::string argTypeNames()const { return nativefunction::argTypesName(c_function_type(0)); }
		virtual bool checkArgTypes(lua_State* state)const { return kaguya::nativefunction::checkArgTypes(state, c_function_type(0), maxArgCount() - minArgCount()); }
		virtual bool strictCheckArgTypes(lua_State* state)const { return kaguya::nativefunction::strictCheckArgTypes(state, c_function_type(0), maxArgCount() - minArgCount()); }
	};

	template<typename F>
	struct OverloadFunctionImpl<F, void> : kaguya::FunctionImpl
	{
		typedef void result_type;
		typedef typename util::FunctionSignature<F>::type::c_function_type c_function_type;

		virtual result_type invoke_type(lua_State *state) = 0;

		virtual int invoke(lua_State *state)
		{
			invoke_type(state);
			return 0;
		}
		virtual std::string argTypeNames()const { return nativefunction::argTypesName(c_function_type(0)); }
		virtual bool checkArgTypes(lua_State* state)const { return kaguya::nativefunction::checkArgTypes(state, c_function_type(0), maxArgCount() - minArgCount()); }
		virtual bool strictCheckArgTypes(lua_State* state)const { return kaguya::nativefunction::strictCheckArgTypes(state, c_function_type(0), maxArgCount() - minArgCount()); }
	};
}

#define KAGUYA_INTERNAL_OVERLOAD_FUNCTION_GET_REP(N) getArgument<N-1,F>(state)
#define KAGUYA_INTERNAL_OVERLOAD_FUNCTION_GET_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_INTERNAL_OVERLOAD_FUNCTION_GET_REP)
#define KAGUYA_INTERNAL_OVERLOAD_FUNCTION_INVOKE(N,FNAME,MINARG, MAXARG) if (argcount == KAGUYA_PP_ADD(MINARG,KAGUYA_PP_DEC(N))) { return FNAME(KAGUYA_INTERNAL_OVERLOAD_FUNCTION_GET_REPEAT(KAGUYA_PP_ADD(MINARG,KAGUYA_PP_DEC(N)))); }

#define KAGUYA_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,FNAME, MINARG, MAXARG,CREATE_FN)\
struct GENERATE_NAME\
{\
	template<typename F>\
	struct Function : kaguya::OverloadFunctionImpl<F>\
	{\
		typedef typename kaguya::OverloadFunctionImpl<F>::result_type result_type;\
		virtual result_type invoke_type(lua_State *state)\
		{\
			using namespace kaguya::nativefunction;\
			int argcount = lua_gettop(state);\
			KAGUYA_PP_REPEAT_DEF_VA_ARG(KAGUYA_PP_INC(KAGUYA_PP_SUB(MAXARG, MINARG)), KAGUYA_INTERNAL_OVERLOAD_FUNCTION_INVOKE, FNAME, MINARG, MAXARG)\
			throw kaguya::LuaTypeMismatch("argument count mismatch");\
		}\
		virtual int minArgCount()const { return MINARG;	}\
		virtual int maxArgCount()const { return MAXARG; }\
	};\
	template<typename F> kaguya::PolymorphicInvoker::holder_type create(F f)\
	{\
		kaguya::OverloadFunctionImpl<F>* ptr = new Function<F>();\
		return kaguya::PolymorphicInvoker::holder_type(ptr);\
	}\
	template<typename F> kaguya::PolymorphicInvoker::holder_type create()\
	{\
		kaguya::OverloadFunctionImpl<F>* ptr = new Function<F>();\
		return kaguya::PolymorphicInvoker::holder_type(ptr);\
	}\
	kaguya::PolymorphicInvoker operator()() { return CREATE_FN; }\
}GENERATE_NAME;


#define KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_GET_REP(N) getArgument<N,F>(state)
#define KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_GET_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_GET_REP)
#define KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_INVOKE(N,FNAME,MINARG, MAXARG) if (argcount == 1 + KAGUYA_PP_ADD(MINARG,KAGUYA_PP_DEC(N))) { return (getArgument<0,F>(state)).FNAME(KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_GET_REPEAT(KAGUYA_PP_ADD(MINARG,KAGUYA_PP_DEC(N)))); }

#define KAGUYA_MEMBER_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,CLASS,FNAME, MINARG, MAXARG,CREATE_FN)\
struct GENERATE_NAME\
{\
	template<typename F>\
	struct Function : kaguya::OverloadFunctionImpl<F>\
	{\
		typedef typename kaguya::OverloadFunctionImpl<F>::result_type result_type;\
		virtual result_type invoke_type(lua_State *state)\
		{\
			using namespace kaguya::nativefunction;\
			int argcount = lua_gettop(state);\
			KAGUYA_PP_REPEAT_DEF_VA_ARG(KAGUYA_PP_INC(KAGUYA_PP_SUB(MAXARG, MINARG)), KAGUYA_INTERNAL_OVERLOAD_MEMBER_FUNCTION_INVOKE, FNAME, MINARG, MAXARG)\
			throw kaguya::LuaTypeMismatch("argument count mismatch");\
		}\
		virtual int minArgCount()const { return MINARG + 1;	}\
		virtual int maxArgCount()const { return MAXARG + 1; }\
	};\
	template<typename F> kaguya::PolymorphicMemberInvoker::holder_type create(F f)\
	{\
		kaguya::OverloadFunctionImpl<F>* ptr = new Function<F>();\
		return kaguya::PolymorphicMemberInvoker::holder_type(ptr);\
	}\
	template<typename F> kaguya::PolymorphicMemberInvoker::holder_type create()\
	{\
		kaguya::OverloadFunctionImpl<F>* ptr = new Function<F>();\
		return kaguya::PolymorphicMemberInvoker::holder_type(ptr);\
	}\
	kaguya::PolymorphicMemberInvoker operator()()\
	{\
		return CREATE_FN;\
	}\
}GENERATE_NAME;


/// @brief Generate wrapper function object for count based overloads with nonvoid return function. Include default arguments parameter function
/// @param GENERATE_NAME generate function object name
/// @param FNAME target function name
/// @param MINARG minimum arguments count
/// @param MAXARG maximum arguments count
#define KAGUYA_FUNCTION_OVERLOADS(GENERATE_NAME,FNAME, MINARG, MAXARG) KAGUYA_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,FNAME, MINARG, MAXARG,create(FNAME))


/// @brief Generate wrapper function object for count based overloads with nonvoid return function. Include default arguments parameter function
/// @param GENERATE_NAME generate function object name
/// @param FNAME target function name
/// @param MINARG minimum arguments count
/// @param MAXARG maximum arguments count
/// @param SIGNATURE function signature. e,g, int(int)
#define KAGUYA_FUNCTION_OVERLOADS_WITH_SIGNATURE(GENERATE_NAME,FNAME, MINARG, MAXARG,SIGNATURE) KAGUYA_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,FNAME, MINARG, MAXARG,create<SIGNATURE>())


/// @brief Generate wrapper function object for count based overloads with nonvoid return function. Include default arguments parameter function
/// @param GENERATE_NAME generate function object name
/// @param CLASS target class name
/// @param FNAME target function name
/// @param MINARG minimum arguments count
/// @param MAXARG maximum arguments count
#define KAGUYA_MEMBER_FUNCTION_OVERLOADS(GENERATE_NAME,CLASS,FNAME, MINARG, MAXARG) KAGUYA_MEMBER_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,CLASS,FNAME, MINARG, MAXARG,create(&CLASS::FNAME))

/// @brief Generate wrapper function object for count based overloads with nonvoid return function. Include default arguments parameter function
/// @param GENERATE_NAME generate function object name
/// @param CLASS target class name
/// @param FNAME target function name
/// @param MINARG minimum arguments count
/// @param MAXARG maximum arguments count
/// @param SIGNATURE function signature. e,g, int(Test::*)(int)
#define KAGUYA_MEMBER_FUNCTION_OVERLOADS_WITH_SIGNATURE(GENERATE_NAME,CLASS,FNAME, MINARG, MAXARG,SIGNATURE) KAGUYA_MEMBER_FUNCTION_OVERLOADS_INTERNAL(GENERATE_NAME,CLASS,FNAME, MINARG, MAXARG,create<SIGNATURE>())
