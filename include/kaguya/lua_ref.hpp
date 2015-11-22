#pragma once


#include "kaguya/config.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utils.hpp"


namespace kaguya
{
	struct StackTop {};
	class LuaRef
	{
		lua_State *state_;
		int ref_;

	public:
		LuaRef(const LuaRef& src) :state_(src.state_)
		{
			src.push();
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}
		LuaRef& operator =(const LuaRef& src)
		{
			state_ = src.state_;
			src.push();
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			return *this;
		}

		LuaRef(lua_State* state) :state_(state), ref_(LUA_REFNIL) {}


		LuaRef(lua_State* state, StackTop s) :state_(state), ref_(LUA_REFNIL)
		{
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}

		template<typename T>
		LuaRef(lua_State* state, T v) : state_(state)
		{
			types::push(state_, v);
			ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
		}
		~LuaRef()
		{
			luaL_unref(state_, LUA_REGISTRYINDEX, ref_);
		}

		//push to Lua stack
		void push()const
		{
			lua_rawgeti(state_, LUA_REGISTRYINDEX, ref_);
		}


		template<typename T>
		T get()const
		{
			utils::ScopedSavedStack save(state_);
			push();
			return types::get(state_, -1, types::type_tag<T>());
		}

		int type() const
		{
			utils::ScopedSavedStack save(state_);

			if (ref_ == LUA_REFNIL)
			{
				return LUA_TNIL;
			}

			push();
			return lua_type(state_, -1);
		}
		std::string typeName()const
		{
			return lua_typename(state_, type());
		}
	};

	namespace types
	{
		inline LuaRef get(lua_State* l, int index, type_tag<LuaRef> tag = type_tag<LuaRef>())
		{
			lua_pushvalue(l, index);
			return LuaRef(l, StackTop());
		}
		inline int push(lua_State* l, const LuaRef& ref)
		{
			ref.push();
			return 1;
		}
	}
}
