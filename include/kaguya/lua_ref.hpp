// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <algorithm>
#include <ostream>
#include "kaguya/config.hpp"
#include "kaguya/error_handler.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"

#include "kaguya/detail/lua_ref_impl.hpp"
#include "kaguya/detail/lua_variant_def.hpp"

namespace kaguya
{

	class LuaUserData;
	class LuaTable;
	class LuaFunction;
	class LuaThread;
	template<typename KEY>
	class TableKeyReference;
	class FunctionResults;
	class mem_fun_binder;

	class LuaRef;

	template<>
	struct lua_type_traits<LuaRef>
	{
		typedef LuaRef get_type;
		typedef const LuaRef& push_type;

		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return false;
		}

		static get_type get(lua_State* l, int index);
		static int push(lua_State* l, push_type v);
	};
	template<>	struct lua_type_traits<const LuaRef&> :lua_type_traits<LuaRef> {};


	namespace util
	{
		template<class Result>
		inline Result get_result_impl(lua_State* l, int startindex, types::typetag<Result>)
		{
			return lua_type_traits<Result>::get(l, startindex);
		}
#if KAGUYA_USE_CPP11
		inline standard::tuple<> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}
		template<typename T, typename... TYPES>
		inline standard::tuple<T, TYPES...> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<T, TYPES...> > tag)
		{
			return standard::tuple_cat(standard::tuple<T>(lua_type_traits<T>::get(l, index)),
				get_result_tuple_impl(l, index + 1, types::typetag<standard::tuple<TYPES...> >()));
		}
		template<typename... TYPES>
		inline standard::tuple<TYPES...> get_result_impl(lua_State* l, int startindex, types::typetag<standard::tuple<TYPES...> > tag)
		{
			return get_result_tuple_impl<TYPES...>(l, startindex, tag);
		}
#else

		inline standard::tuple<> get_result_impl(lua_State *l, int startindex, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_TARG(N) KAGUYA_PP_CAT(A,N)
#define KAGUYA_GET_DEF(N) lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(l, N + startindex - 1) 
#define KAGUYA_GET_TUPLE_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> get_result_impl(lua_State *l,int startindex,types::typetag<standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> >)\
		{\
			return standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG) >(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_GET_DEF));\
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_GET_TUPLE_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_TARG
#undef KAGUYA_GET_DEF
#undef KAGUYA_GET_TUPLE_DEF
#endif

			template<class Result>
		inline Result get_result(lua_State* l, int startindex)
		{
			return get_result_impl(l, startindex, types::typetag<Result>());
		}
		template<>
		inline void get_result<void>(lua_State* l, int startindex)
		{
		}
	}


	/**
	* Reference of Lua any type value.
	*/
	class LuaRef :public Ref::RegistoryRef, public LuaVariantImpl<LuaRef>
	{
		friend class LuaUserData;
		friend class LuaTable;
		friend class LuaFunction;
		friend class LuaThread;
	private:
		typedef void (LuaRef::*bool_type)() const;
		void this_type_does_not_support_comparisons() const {}


		static lua_State* toMainThread(lua_State* state)
		{
#if LUA_VERSION_NUM >= 502
			if (state)
			{
				lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
				lua_State* mainthread = lua_tothread(state, -1);
				lua_pop(state, 1);
				if (mainthread)
				{
					return mainthread;
				}
			}
#endif
			return state;
		}
	public:


		LuaRef(const Ref::RegistoryRef& src) :Ref::RegistoryRef(src)
		{
		}
		LuaRef(const LuaRef& src) :Ref::RegistoryRef(src)
		{
		}
		LuaRef& operator =(const LuaRef& src)
		{
			static_cast<RegistoryRef&>(*this) = src;
			return *this;
		}

#if KAGUYA_USE_CPP11

		LuaRef(LuaRef&& src)throw() :Ref::RegistoryRef(std::move(src)) {}

		LuaRef& operator =(LuaRef&& src)throw()
		{
			swap(src);
			return *this;
		}

		LuaRef(RegistoryRef&& src)throw() :Ref::RegistoryRef(std::move(src)) {}
		template<typename T>
		LuaRef(lua_State* state, T&& v, Ref::NoMainCheck) : Ref::RegistoryRef(state, std::move(v), Ref::NoMainCheck()) {}
		template<typename T>
		LuaRef(lua_State* state, T&& v) : Ref::RegistoryRef(state, std::move(v)) {}
#endif

		LuaRef() {}
		LuaRef(lua_State* state) :Ref::RegistoryRef(state) {}

		LuaRef(lua_State* state, StackTop, Ref::NoMainCheck) :Ref::RegistoryRef(state, StackTop(), Ref::NoMainCheck()) {}

		LuaRef(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
		}


		template<typename T>
		LuaRef(lua_State* state, const T& v, Ref::NoMainCheck) : Ref::RegistoryRef(state, v, Ref::NoMainCheck()) {}
		template<typename T>
		LuaRef(lua_State* state, const T& v) : Ref::RegistoryRef(state, v) {}

		bool isNilref()const { return state() == 0 || ref_ == LUA_REFNIL; }



		//push to Lua stack
		int push()const
		{
			return push(state());
		}
		int push(lua_State* s)const
		{
			if (isNilref())
			{
				lua_pushnil(s);
				return 1;
			}
#if LUA_VERSION_NUM >= 502
			if (s != state())
			{//state check
				assert(toMainThread(s) == toMainThread(state()));
			}
#endif
			lua_rawgeti(s, LUA_REGISTRYINDEX, ref_);
			return 1;
		}
		int pushStackIndex(lua_State* state)const
		{
			push(state);
			return lua_gettop(state);
		}

		template<typename T>
		bool typeTest()const
		{
			util::ScopedSavedStack save(state());
			push();//push to stack
			return lua_type_traits<T>::strictCheckType(state(), -1);
		}
		template<typename T>
		bool weakTypeTest()const
		{
			util::ScopedSavedStack save(state());
			push();//push to stack
			return lua_type_traits<T>::checkType(state(), -1);
		}




		const void* native_pointer()const
		{
			util::ScopedSavedStack save(state());
			push(state());
			return lua_topointer(state(), -1);
		}
		//@}
		static void putindent(std::ostream& os, int indent)
		{
			while (indent-- > 0)
			{
				os << "  ";
			}
		}
	};




	inline lua_type_traits<LuaRef>::get_type lua_type_traits<LuaRef>::get(lua_State* l, int index)
	{
		lua_pushvalue(l, index);
		return LuaRef(l, StackTop());
	}
	inline int lua_type_traits<LuaRef>::push(lua_State* l, lua_type_traits<LuaRef>::push_type v)
	{
		v.push(l);
		return 1;
	}
}



#if KAGUYA_USE_CPP11
#else
namespace std
{
	template <> inline void swap(kaguya::LuaRef& a, kaguya::LuaRef& b)
	{
		a.swap(b);
	}
}
#endif
