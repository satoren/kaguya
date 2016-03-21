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


namespace kaguya
{

	template<typename T>
	inline bool pushCountCheck(lua_State* state, int count)
	{
		if (count != 1)
		{
			if (count > 1) { except::typeMismatchError(state, std::string("can not push multiple value:") + typeid(T).name()); }
			if (count == 0) { except::typeMismatchError(state, std::string("can not push ") + typeid(T).name() + " value"); }
			return false;
		}
		return true;
	}

	struct StackTop {};
	namespace Ref
	{
		struct NoMainCheck {};

		class StackRef
		{
			lua_State *state_;
			int stack_index_;
			mutable bool pop_;
		protected:
#if KAGUYA_USE_CPP11
			StackRef(StackRef&& src) :state_(src.state_), stack_index_(src.stack_index_), pop_(src.pop_)
			{
				src.pop_ = false;
			}
#else
			StackRef(const StackRef&src) : state_(src.state_), stack_index_(src.stack_index_), pop_(src.pop_)
			{
				src.pop_ = false;
			}
#endif
			StackRef(lua_State* s, int index) :state_(s), stack_index_(index), pop_(true)
			{
			}
			StackRef(lua_State* s, int index, bool pop) :state_(s), stack_index_(index), pop_(pop)
			{
			}
			~StackRef()
			{
				if (state_ && pop_)
				{
					lua_settop(state_, stack_index_ - 1);
				}
			}
		public:


			bool isNilref()const { return state_ == 0 || lua_type(state_, stack_index_) == LUA_TNIL; }

			int push()const
			{
				lua_pushvalue(state_, stack_index_);
				return 1;
			}
			int push(lua_State* state)const
			{
				lua_pushvalue(state_, stack_index_);
				if (state_ != state)
				{
					lua_pushvalue(state_, stack_index_);
					lua_xmove(state_, state, 1);
				}
				return 1;
			}

			int pushStackIndex(lua_State* state)const
			{
				if (state_ != state)
				{
					lua_pushvalue(state_,stack_index_);
					lua_xmove(state_,state,1);
					return lua_gettop(state);
				}
				else
				{
					return stack_index_;
				}
			}
			lua_State *state()const { return state_; }


		};

		class RegistoryRef
		{
			typedef void (RegistoryRef::*bool_type)() const;
			void this_type_does_not_support_comparisons() const {}


		public:

			RegistoryRef(const RegistoryRef& src) :state_(src.state_)
			{
				if (!src.isNilref())
				{
					src.push(state_);
					ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				}
				else
				{
					ref_ = LUA_REFNIL;
				}
			}
			RegistoryRef& operator =(const RegistoryRef& src)
			{
				unref();
				state_ = src.state_;
				if (!src.isNilref())
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
			RegistoryRef(RegistoryRef&& src)throw() :state_(0), ref_(LUA_REFNIL)
			{
				swap(src);
			}
			RegistoryRef& operator =(RegistoryRef&& src)throw()
			{
				swap(src);
				return *this;
			}
#endif

			RegistoryRef() :state_(0), ref_(LUA_REFNIL) {}
			RegistoryRef(lua_State* state) :state_(state), ref_(LUA_REFNIL) {}


			RegistoryRef(lua_State* state, StackTop, NoMainCheck) :state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}

			RegistoryRef(lua_State* state, StackTop) :state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				state_ = util::toMainThread(state_);
			}

			void swap(RegistoryRef& other)throw()
			{
				std::swap(state_, other.state_);
				std::swap(ref_, other.ref_);
			}

			template<typename T>
			RegistoryRef(lua_State* state, const T& v, NoMainCheck) : state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				util::ScopedSavedStack save(state_);
				int vc = util::push_args(state_, v);
				if (!pushCountCheck<T>(state_, vc)) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			template<typename T>
			RegistoryRef(lua_State* state, const T& v) : state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				util::ScopedSavedStack save(state_);
				int vc = util::push_args(state_, v);
				if (!pushCountCheck<T>(state_, vc)) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				state_ = util::toMainThread(state_);
			}
#if KAGUYA_USE_CPP11
			template<typename T>
			RegistoryRef(lua_State* state, T&& v, NoMainCheck) : state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				util::ScopedSavedStack save(state_);
				int vc = util::push_args(state_, standard::forward<T>(v));
				if (!pushCountCheck<T>(vc)) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
			}
			template<typename T>
			RegistoryRef(lua_State* state, T&& v) : state_(state), ref_(LUA_REFNIL)
			{
				if (!state_) { return; }
				util::ScopedSavedStack save(state_);
				int vc = util::push_args(state_, standard::forward<T>(v));
				if (!pushCountCheck<T>(state_, vc)) { return; }
				ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
				state_ = util::toMainThread(state_);
			}
#endif
			~RegistoryRef()
			{
				unref();
			}

			//push to Lua stack
			int push()const
			{
				return push(state_);
			}
			int push(lua_State* state)const
			{
				if (isNilref())
				{
					lua_pushnil(state);
					return 1;
				}
#if LUA_VERSION_NUM >= 502
				if (state != state_)
				{//state check
					assert(util::toMainThread(state) == util::toMainThread(state_));
				}
#endif
				lua_rawgeti(state, LUA_REGISTRYINDEX, ref_);
				return 1;
			}

			int pushStackIndex(lua_State* state)const
			{
				push(state);
				return lua_gettop(state);
			}
			lua_State *state()const { return state_; }

			bool isNilref()const { return state_ == 0 || ref_ == LUA_REFNIL; }
			
		protected:
			lua_State *state_;
			int ref_;


			void unref()
			{
				if (!isNilref())
				{
					luaL_unref(state_, LUA_REGISTRYINDEX, ref_);
					state_ = 0;
					ref_ = LUA_REFNIL;
				}
			}
		};
		
	}
}
