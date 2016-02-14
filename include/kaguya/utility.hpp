// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include "kaguya/config.hpp"
#include "kaguya/type.hpp"

namespace kaguya
{
	namespace util
	{
		class Ref
		{
		public:
			Ref(const Ref& src) :state_(src.state_)
			{
				if (src.state_ != 0)
				{
					src.push(state_);
					ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				}
				else
				{
					ref_ = LUA_REFNIL;
				}
			}
			Ref& operator =(const Ref& src)
			{
				unref();
				state_ = src.state_;
				if (src.state_ != 0)
				{
					src.push(state_);
					ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				}
				else
				{
					ref_ = LUA_REFNIL;
				}
				return *this;
			}
#if KAGUYA_USE_RVALUE_REFERENCE
			Ref(Ref&& src)throw() :state_(0), ref_(LUA_REFNIL)
			{
				swap(src);
			}
			Ref& operator =(Ref&& src)throw()
			{
				swap(src);
				return *this;
			}
#endif

			Ref() :state_(0), ref_(LUA_REFNIL) {}
			Ref(lua_State* state) :state_(state), ref_(LUA_REFNIL) {}

			Ref(lua_State* state, int index) :state_(state), ref_(LUA_REFNIL)
			{
				lua_pushvalue(state_,index);
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			void swap(Ref& other)throw()
			{
				std::swap(state_, other.state_);
				std::swap(ref_, other.ref_);
			}
			~Ref()
			{
				unref();
			}
			void ref()
			{
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			void unref()
			{
				luaL_unref(state_, LUA_REGISTRYINDEX, ref_);
				state_ = 0;
				ref_ = LUA_REFNIL;
			}
			void push()const
			{
				push(state_);
			}
			void push(lua_State* state)const
			{
				lua_rawgeti(state, LUA_REGISTRYINDEX, ref_);
			}
		private:
			lua_State *state_;
			int ref_;
		};

		class ScopedSavedStack {
			lua_State * state_;
			int saved_top_index_;

		public:
			explicit ScopedSavedStack(lua_State * state)
				: state_(state),
				saved_top_index_(lua_gettop(state_))
			{}
			explicit ScopedSavedStack(lua_State * state, int count)
				: state_(state),
				saved_top_index_(count)
			{}

			~ScopedSavedStack() {
				if (state_) {
					lua_settop(state_, saved_top_index_);
				}
			}

		private:
			ScopedSavedStack(ScopedSavedStack const &);
			ScopedSavedStack & operator=(ScopedSavedStack const &);
		};

		inline std::string argmentTypes(lua_State *state)
		{
			int top = lua_gettop(state);

			std::string result = "";

			for (int i = 1; i <= top; i++) {
				if (i != 1)
				{
					result += ",";
				}
				if (lua_type(state, i) == LUA_TUSERDATA && luaL_getmetafield(state, i, "__name") == LUA_TSTRING)
				{
					result += lua_tostring(state, -1);
					lua_pop(state, 1);
				}
				else
				{
					result += lua_typename(state, lua_type(state, i));
				}
			}
			return result;
		}
		inline void traceBack(lua_State* state, const char* message, int level = 0)
		{
#if LUA_VERSION_NUM >= 502
			luaL_traceback(state, state, message, level);
#else
			lua_pushstring(state, message);
#endif
	}

		inline void stackDump(lua_State *L) {
			int i;
			int top = lua_gettop(L);
			for (i = 1; i <= top; i++) {  /* repeat for each level */
				int t = lua_type(L, i);
				switch (t) {

				case LUA_TSTRING:  /* strings */
					printf("`%s'", lua_tostring(L, i));
					break;

				case LUA_TBOOLEAN:  /* booleans */
					printf(lua_toboolean(L, i) ? "true" : "false");
					break;

				case LUA_TNUMBER:  /* numbers */
					printf("%g", lua_tonumber(L, i));
					break;
				case LUA_TUSERDATA:
					if (luaL_getmetafield(L, i, "__name") == LUA_TSTRING)
					{
						printf("userdata:%s", lua_tostring(L, -1));
						lua_pop(L, 1);
						break;
					}
				default:  /* other values */
					printf("%s", lua_typename(L, t));
					break;

				}
				printf("  ");  /* put a separator */
			}
			printf("\n");  /* end the listing */
		}

		inline int lua_resume_compat(lua_State *L, int nargs)
		{
			if (nargs < 0) { nargs = 0; }
#if LUA_VERSION_NUM >= 502
			return lua_resume(L, 0, nargs);
#else
			return lua_resume(L, nargs);
#endif
		}



		inline lua_State* toMainThread(lua_State* state)
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

#if KAGUYA_USE_CPP11
		inline int push_args(lua_State *l)
		{
			return 0;
		}
		template<class Arg, class...Args>
		inline int push_args(lua_State *l, Arg&& arg, Args&&... args)
		{
			int c = lua_type_traits<typename traits::remove_reference<Arg>::type>::push(l, std::forward<Arg>(arg));
			return c + push_args(l, std::forward<Args>(args)...);
		}
		template<class Arg, class...Args>
		inline int push_args(lua_State *l, const Arg& arg, Args&&... args)
		{
			int c = lua_type_traits<Arg>::push(l, arg);
			return c + push_args(l, std::forward<Args>(args)...);
		}
#else
		inline void push_args(lua_State *l)
		{
		}

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) const KAGUYA_PP_CAT(A,N)& KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_DEF(N) c+=lua_type_traits<KAGUYA_PP_CAT(A,N) >::push(l, KAGUYA_PP_CAT(a,N)); 
#define KAGUYA_PUSH_ARG_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline int push_args(lua_State *l,KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			int c =0;\
			KAGUYA_PP_REPEAT(N,KAGUYA_PUSH_DEF)\
			return c;\
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_PUSH_ARG_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_PUSH_DEF
#undef KAGUYA_PUSH_ARG_DEF
#endif
		}
}
