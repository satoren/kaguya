#pragma once

#include <string>
#include <vector>

#include "kaguya/config.hpp"
#include "kaguya/utils.hpp"
#include "kaguya/type.hpp"
#include "kaguya/lua_ref.hpp"


#define KAGUYA_FUNCTOR_METATABLE "functor_kaguya_metatype"
namespace kaguya
{
	namespace nativefunction
	{
		struct BaseInvoker
		{
			virtual bool checktype(lua_State *state, bool strictcheck) = 0;
			virtual int invoke(lua_State *state) = 0;
			virtual ~BaseInvoker() {}
		};

		typedef standard::shared_ptr<nativefunction::BaseInvoker> FunctorType;

		inline FunctorType* pick_match_function(lua_State *l)
		{
			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));

			if (overloadnum == 1)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(2)));
				if ((*fun)->checktype(l, false))
				{
					return fun;
				}
				else
				{
					return 0;
				}
			}
			FunctorType* weak_match = 0;
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if ((*fun)->checktype(l, true))
				{
					return fun;
				}
				else if (weak_match == 0 && (*fun)->checktype(l, false))
				{
					weak_match = fun;
				}
			}
			return weak_match;
		}

		inline int functor_dispatcher(lua_State *l)
		{
			FunctorType* fun = pick_match_function(l);
			if (fun && (*fun))
			{
				try {
					return (*fun)->invoke(l);
				}
				catch (std::exception & e) {
					lua_pushstring(l, e.what());
					return lua_error(l);
				}
				catch (...) {
					lua_pushliteral(l, "Unknown exception");
					return lua_error(l);
				}
			}
			std::string message = "argment not matched:" + utils::argmentTypes(l);
			lua_pushstring(l, message.c_str());
			return lua_error(l);
		}

		inline int functor_destructor(lua_State *state)
		{
			FunctorType* f = (FunctorType*)luaL_checkudata(state, 1, KAGUYA_FUNCTOR_METATABLE);
			f->~FunctorType();
			return 0;
		}
		inline void reg_functor_destructor(lua_State* state)
		{
			luaL_getmetatable(state, KAGUYA_FUNCTOR_METATABLE);

			int result = lua_type(state, -1);
			lua_pop(state, 1);
			if (result == LUA_TNIL)
			{
				luaL_newmetatable(state, KAGUYA_FUNCTOR_METATABLE);
				lua_pushcclosure(state, &functor_destructor, 0);
				lua_setfield(state, -2, "__gc");
				lua_setfield(state, -1, "__index");
			}
		}

#include "kaguya/gen/native_function.inl"

		template<class ClassType, class MemType>
		struct MemDataInvoker :BaseInvoker {
			typedef MemType ClassType::*data_type;
			data_type data_;
			MemDataInvoker(data_type data) :data_(data) {}
			virtual bool checktype(lua_State *state, bool strictcheck) {
				if (lua_gettop(state) != 1 && lua_gettop(state) != 2) { return false; }
				if (getPtr(state) == 0) { return false; }
				return true;
			}
			ClassType* getPtr(lua_State *state) {
				if (types::checkType(state, 1, types::typetag<ClassType*>()))
				{
					return types::get(state, 1, types::typetag<ClassType*>());
				}

				if (types::checkType(state, 1, types::typetag<standard::shared_ptr<ClassType>*>()))
				{
					standard::shared_ptr<ClassType>* shared_ptr = types::get(state, 1, types::typetag<standard::shared_ptr<ClassType>*>());
					if (shared_ptr) {
						return shared_ptr->get();
					}
				}
				return 0;
			}
			virtual int invoke(lua_State *state)
			{
				ClassType* ptr = getPtr(state);
				if (lua_gettop(state) == 2)
				{
					ptr->*data_ = types::get(state, 2, types::typetag<MemType>());
				}

				return types::push(state, standard::forward<MemType>(ptr->*data_));
			}
		};
		template< class ClassType, class MemType>
		BaseInvoker* create(MemType ClassType::*data)
		{
			typedef MemDataInvoker<ClassType, MemType> invoker_type;
			return new invoker_type(data);
		}

	}


	typedef nativefunction::FunctorType FunctorType;
	template<typename T>
	inline FunctorType function(T f)
	{
		return FunctorType(nativefunction::create(standard::forward<T>(f)));
	}

	namespace types
	{
		template<>
		inline nativefunction::FunctorType get(lua_State* l, int index, typetag<nativefunction::FunctorType> tag)
		{
			return *(static_cast<nativefunction::FunctorType*>(luaL_checkudata(l, index, KAGUYA_FUNCTOR_METATABLE)));
		}
		template<>
		inline int push(lua_State* l, const nativefunction::FunctorType& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::FunctorType));
			new(storage) nativefunction::FunctorType(f);
			luaL_setmetatable(l, KAGUYA_FUNCTOR_METATABLE);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
		template<>
		inline int push(lua_State* l, nativefunction::FunctorType& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::FunctorType));
			new(storage) nativefunction::FunctorType(f);
			luaL_setmetatable(l, KAGUYA_FUNCTOR_METATABLE);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#if KAGUYA_USE_RVALUE_REFERENCE
		inline int push(lua_State* l, nativefunction::FunctorType&& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::FunctorType));
			new(storage) nativefunction::FunctorType(standard::forward<nativefunction::FunctorType>(f));
			luaL_setmetatable(l, KAGUYA_FUNCTOR_METATABLE);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#endif
	}
};

#include "kaguya/variadic_function.hpp"