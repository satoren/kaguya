#pragma once

#include "kaguya/config.hpp"
#include "kaguya/type.hpp"
#include "kaguya/preprocess.hpp"

namespace kaguya
{
	namespace util
	{
		class ScopedSavedStack {
			lua_State * state_;
			int saved_top_index_;

		public:
			explicit ScopedSavedStack(lua_State * state)
				: state_(state),
				saved_top_index_(lua_gettop(state_))
			{}
			explicit ScopedSavedStack(lua_State * state,int count)
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

#if KAGUYA_USE_CPP11
		inline void push_args(lua_State *l)
		{
		}
		template<class Arg, class...Args>
		inline void push_args(lua_State *l, Arg&& arg, Args&&... args)
		{
			lua_type_traits<Arg>::push(l, standard::forward<Arg>(arg));
			push_args(l, standard::forward<Args>(args)...);
		}
		template<class Arg, class...Args>
		inline void push_args(lua_State *l, const Arg& arg,Args&&... args)
		{
			lua_type_traits<Arg>::push(l, arg);
			push_args(l, standard::forward<Args>(args)...);
		}
#else
		inline void push_args(lua_State *l)
		{
		}

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) KAGUYA_PP_CAT(A,N) KAGUYA_PP_CAT(a,N)
#define KAGUYA_PUSH_DEF(N) lua_type_traits<KAGUYA_PP_CAT(A,N) >::push(l, standard::forward<KAGUYA_PP_CAT(A,N)>(KAGUYA_PP_CAT(a,N))); 
#define KAGUYA_PUSH_ARG_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline void push_args(lua_State *l,KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			using standard::forward;\
			KAGUYA_PP_REPEAT(N,KAGUYA_PUSH_DEF)\
		}
		KAGUYA_PP_REPEAT_DEF(9,KAGUYA_PUSH_ARG_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_PUSH_DEF
#undef KAGUYA_PUSH_ARG_DEF
#endif
	}
}
