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
		static const int MAX_OVERLOAD_SCORE = 255;
		template<typename Fn>uint8_t compute_function_matching_score(lua_State* state, const Fn& fn)
		{
			int argcount = lua_gettop(state);

			if (strictCheckArgTypes(state, fn))
			{
				int fnargcount = argCount(fn);
				if (argcount == fnargcount)
				{
					return MAX_OVERLOAD_SCORE;
				}
				else
				{
					return std::max(100 - std::abs(argcount - fnargcount), 51);
				}
			}
			else if (checkArgTypes(state, fn))
			{
				int fnargcount = argCount(fn);
				if (argcount == fnargcount)
				{
					return 200;
				}
				else
				{
					return std::max(50 - std::abs(argcount - fnargcount), 1);
				}
			}
			else
			{
				return 0;
			}
		}

		struct BaseInvoker
		{
			virtual int argsCount()const = 0;
			virtual bool checktype(lua_State *state, bool strictcheck) = 0;
			virtual int invoke(lua_State *state) = 0;
			virtual std::string argumentTypeNames() = 0;
			virtual uint8_t argsMatchingScore(lua_State* state) = 0;
			virtual ~BaseInvoker() {}
		};

		//! deperecated
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

					return count;

				}
				virtual std::string argumentTypeNames() {
					return argTypesName(func_);
				}
				virtual uint8_t argsMatchingScore(lua_State* state) {
					return compute_function_matching_score(state, func_);
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
			FunctorType* match_function = 0;
			uint8_t match_score = 0;
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if (!fun || !(*fun))
				{
					continue;
				}
				uint8_t score = (*fun)->argsMatchingScore(l);
				if (match_score < score)
				{
					match_function = fun;
					match_score = score;
					if (match_score == MAX_OVERLOAD_SCORE)
					{
						return match_function;
					}
				}
			}
			return match_function;
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
					util::traceBack(l, (std::string("maybe...") + build_arg_error_message(l)).c_str());
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
		template<typename Fn> int invoke_index(lua_State* state, int index, int current_index, const Fn& fn)
		{
			if (index == current_index)
			{
				return nativefunction::call(state, fn);
			}
			else
			{
				return -1;
			}
		}
		template<typename Fn, typename... Functions> int invoke_index(lua_State* state, int index, int current_index, const Fn& fn, const Functions&... fns)
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

		template<typename Fun> int best_match_invoke(lua_State* state, const Fun& fn)
		{
			return nativefunction::call(state, fn);
		}

		template<typename Fun, typename... Functions> int best_match_invoke(lua_State* state, const Fun& fn, const Functions&... fns)
		{
			int index = best_function_index(state, fn, fns...);
			if (index >= 0)
			{
				return invoke_index(state, index, 0, fn, fns...);
			}
			else
			{
				throw LuaTypeMismatch("type mismatch!!");
			}
			return 0;
		}

		template<typename TupleType, std::size_t ...S> int invoke_tuple_impl(lua_State* state, TupleType&& tuple, nativefunction::cpp11impl::index_tuple<S...>)
		{
			return best_match_invoke(state, std::get<S>(tuple)...);
		}
		template<typename TupleType> int invoke_tuple(lua_State* state, TupleType&& tuple)
		{
			typedef typename std::decay<TupleType>::type ttype;

			typedef typename nativefunction::cpp11impl::index_range<0, std::tuple_size<ttype>::value>::type indexrange;

			return invoke_tuple_impl(state, tuple, indexrange());
		}

		template<typename Fun> std::string arg_typename(const Fun& fn)
		{
			return nativefunction::argTypesName(fn);
		}

		template<typename Fun, typename... Functions> std::string arg_typename(const Fun& fn, const Functions&... fns)
		{
			return "\t\t" + nativefunction::argTypesName(fn) + "\n" + arg_typename(fns...);
		}
		template<typename TupleType, std::size_t ...S> std::string arg_typename_tuple_impl(TupleType&& tuple, nativefunction::cpp11impl::index_tuple<S...>)
		{
			return arg_typename(std::get<S>(tuple)...);
		}
		template<typename TupleType> std::string arg_typename_tuple(TupleType&& tuple)
		{
			typedef typename std::decay<TupleType>::type ttype;
			typedef typename nativefunction::cpp11impl::index_range<0, std::tuple_size<ttype>::value>::type indexrange;

			return arg_typename_tuple_impl(tuple, indexrange());
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


#define KAGUYA_ARG_TYPENAMES(N) + "\t\t" + nativefunction::argTypesName(standard::get<N-1>(tuple)) + "\n"
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
		std::string arg_typename_tuple(standard::tuple<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)>& tuple)\
		{\
			return std::string() KAGUYA_PP_REPEAT(N, KAGUYA_ARG_TYPENAMES);\
		}

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

		template<typename TupleType> std::string arg_typename_tuple(TupleType& tuple)
		{
			return "";
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
		KAGUYA_STATIC_ASSERT(nativefunction::is_callable<typename standard::decay<T>::type>::value, "argument need callable");
		return FunctionInvokerType<standard::tuple<T> >(standard::tuple<T>(f));
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


	template<typename FunctionTuple> struct lua_type_traits<FunctionInvokerType<FunctionTuple> >
	{
		typedef FunctionInvokerType<FunctionTuple> userdatatype;
		typedef const FunctionInvokerType<FunctionTuple>& push_type;


		static std::string build_arg_error_message(lua_State *state, FunctionTuple* tuple)
		{
			return  "Argument mismatch:" + nativefunction::argmentTypes(state) + "\t candidate is:\n"
				+ detail::arg_typename_tuple(*tuple);
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
					util::traceBack(state, (std::string("maybe...") + build_arg_error_message(state, t)).c_str());
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


	//!deperecated
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
		, typename traits::enable_if<traits::is_function<typename traits::remove_pointer<T>::type>::value>::type >
	{
		static int push(lua_State* l, T f)
		{
			return util::one_push(l, function(f));
		}
	};



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
			if (lua_type(l, index) != LUA_TFUNCTION) {
				return get_type();
			}
			lua_pushvalue(l, index);
			return get_type(LuaFunction(l, StackTop()));
		}

		static int push(lua_State* l, push_type v)
		{
			return util::one_push(l, function(v));
		}
	};
}
