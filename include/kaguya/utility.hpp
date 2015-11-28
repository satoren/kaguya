#pragma once

#include "kaguya/config.hpp"

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
		inline void traceBack(lua_State* state,const char* message,int level=0)
		{
			luaL_traceback(state, state, message, level);
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
						printf("userdata:%s",lua_tostring(L, -1));
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
	}
}
