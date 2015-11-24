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
		struct baseInvoker
		{
			virtual bool checktype(lua_State *state,bool strictcheck) = 0;
			virtual int invoke(lua_State *state) = 0;
			virtual ~baseInvoker() {}
		};

		typedef standard::shared_ptr<nativefunction::baseInvoker> functor_type;


		inline int functor_dispatcher(lua_State *l)
		{
			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));
			
			for (int i = 0; i < overloadnum; ++i)
			{
				functor_type& fun = *(static_cast<functor_type*>(lua_touserdata(l, lua_upvalueindex(i + 2))));
				if (!fun->checktype(l,true))
				{
					continue;
				}
				try {
					return fun->invoke(l);
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

			for (int i = 0; i < overloadnum; ++i)
			{
				functor_type& fun = *(static_cast<functor_type*>(lua_touserdata(l, lua_upvalueindex(i + 2))));

				if (!fun->checktype(l, false))
				{
					continue;
				}
				try {
					return fun->invoke(l);
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
			std::string message = "argment not matched:" + utils::argment_types(l);
			lua_pushstring(l, message.c_str());
			return lua_error(l);
		}

		inline int functor_destructor(lua_State *state)
		{
			functor_type* f = (functor_type*)luaL_checkudata(state, 1, KAGUYA_FUNCTOR_METATABLE);
			f->~functor_type();
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
		struct mem_data_invoker :baseInvoker {
			typedef MemType ClassType::*data_type;
			data_type data_;
			mem_data_invoker(data_type data) :data_(data) {}
			virtual bool checktype(lua_State *state, bool strictcheck) {
				if (lua_gettop(state) != 1 && lua_gettop(state) != 2) { return false; }
				if (getPtr(state) == 0) { return false; }
				return true;
			}
			ClassType* getPtr(lua_State *state) {
				if (types::check_type(state, 1, types::type_tag<ClassType*>()))
				{
					return types::get(state, 1, types::type_tag<ClassType*>());
				}

				if (types::check_type(state, 1, types::type_tag<standard::shared_ptr<ClassType>*>()))
				{
					standard::shared_ptr<ClassType>* shared_ptr = types::get(state, 1, types::type_tag<standard::shared_ptr<ClassType>*>());
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
					ptr->*data_ = types::get(state, 2, types::type_tag<MemType>());
				}

				return types::push(state, standard::forward<MemType>(ptr->*data_));
			}
		};
		template< class ClassType,class MemType>
		baseInvoker* create(MemType ClassType::*data)
		{
			typedef mem_data_invoker<ClassType,MemType> invoker_type;
			return new invoker_type(data);
		}

	}


	typedef nativefunction::functor_type function_t;
	template<typename T>
	inline function_t function(T f)
	{
		return function_t(nativefunction::create(standard::forward<T>(f)));
	}

	namespace types
	{
		inline nativefunction::functor_type& get(lua_State* l, int index, type_tag<nativefunction::functor_type> tag = type_tag<nativefunction::functor_type>())
		{
			return *(static_cast<nativefunction::functor_type*>(luaL_checkudata(l, index, KAGUYA_FUNCTOR_METATABLE)));
		}
		inline int push(lua_State* l, const nativefunction::functor_type& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::functor_type));
			new(storage) nativefunction::functor_type(f);
			luaL_setmetatable(l, KAGUYA_FUNCTOR_METATABLE);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#if defined(_HAS_RVALUE_REFERENCES) || defined(__cpp_rvalue_references)
		inline int push(lua_State* l, nativefunction::functor_type&& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::functor_type));
			new(storage) nativefunction::functor_type(standard::forward<nativefunction::functor_type>(f));
			luaL_setmetatable(l, KAGUYA_FUNCTOR_METATABLE);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#endif
	}
};

#include "kaguya/variadic_function.hpp"