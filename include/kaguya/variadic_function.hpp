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
	typedef const std::vector<LuaRef>& variadic_arg_type;
	namespace nativefunction
	{
		//variadic argment
		template<typename Ret>
		struct variadic_arg_caller :baseInvoker {
			typedef Ret(*func_type)(variadic_arg_type);
			func_type func_;
			variadic_arg_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}
				Ret r = func_(args);
				return types::push(state, standard::forward<Ret>(r));
			}
		};
		template<typename Ret>
		baseInvoker* create(Ret(*fun)(variadic_arg_type))
		{
			typedef variadic_arg_caller<Ret> caller_type;
			return new caller_type(fun);
		}

		struct variadic_arg_void_caller :baseInvoker {
			typedef void(*func_type)(variadic_arg_type);
			func_type func_;
			variadic_arg_void_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}
				func_(args);
				return 0;
			}
		};
		baseInvoker* create(void(*fun)(variadic_arg_type))
		{
			typedef variadic_arg_void_caller caller_type;
			return new caller_type(fun);
		}

		template <typename Ret, typename T>
		struct variadic_arg_mem_function_caller :baseInvoker {
			typedef Ret(T::*func_type)(variadic_arg_type);
			func_type func_;
			variadic_arg_mem_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::type_tag<T*>());

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}

				Ret r = (t->*func_)(args);
				return types::push(state, standard::forward<Ret>(r));
			}
		};
		template <typename Ret, typename T>
		baseInvoker* create(Ret(T::*fun)(variadic_arg_type))
		{
			typedef variadic_arg_mem_function_caller<Ret, T> caller_type;
			return new caller_type(fun);
		}

		template <typename T>
		struct variadic_arg_mem_void_function_caller :baseInvoker {
			typedef void (T::*func_type)(variadic_arg_type);
			func_type func_;
			variadic_arg_mem_void_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::type_tag<T*>());

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}

				(t->*func_)(args);
				return 0;
			}
		};
		template <typename T>
		baseInvoker* create(void (T::*fun)(variadic_arg_type))
		{
			typedef variadic_arg_mem_void_function_caller<T> caller_type;
			return new caller_type(fun);
		}


		template <typename Ret, typename T>
		struct variadic_arg_const_mem_function_caller :baseInvoker {
			typedef Ret(T::*func_type)(variadic_arg_type)const;
			func_type func_;
			variadic_arg_const_mem_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::type_tag<T*>());

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top-1);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}

				Ret r = (t->*func_)(args);
				return types::push(state, standard::forward <Ret>(r));
			}
		};
		template <typename Ret, typename T>
		baseInvoker* create(Ret(T::*fun)(variadic_arg_type)const)
		{
			typedef variadic_arg_const_mem_function_caller<Ret, T> caller_type;
			return new caller_type(fun);
		}

		template <typename T>
		struct variadic_arg_const_mem_void_function_caller :baseInvoker {
			typedef void (T::*func_type)(variadic_arg_type)const;
			func_type func_;
			variadic_arg_const_mem_void_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::type_tag<T*>());

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top-1);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}

				(t->*func_)(args);
				return 0;
			}
		};
		template <typename T>
		baseInvoker* create(void (T::*fun)(variadic_arg_type)const)
		{
			typedef variadic_arg_const_mem_void_function_caller<T> caller_type;
			return new caller_type(fun);
		}


		template<typename Ret>
		struct variadic_arg_function_caller :baseInvoker {
			typedef standard::function<Ret(variadic_arg_type)> func_type;
			func_type func_;
			variadic_arg_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}
				Ret r = func_(args);
				return types::push(state, standard::forward<Ret>(r));
			}
		};
		template<typename Ret>
		baseInvoker* create(standard::function<Ret(variadic_arg_type)> fun)
		{
			typedef variadic_arg_function_caller<Ret> caller_type;
			return new caller_type(fun);
		}

		struct variadic_arg_void_function_caller :baseInvoker {
			typedef standard::function<void(variadic_arg_type)> func_type;
			func_type func_;
			variadic_arg_void_function_caller(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}
				func_(args);
				return 0;
			}
		};
		baseInvoker* create(standard::function<void(variadic_arg_type)> fun)
		{
			typedef variadic_arg_void_function_caller caller_type;
			return new caller_type(fun);
		}

		template<typename CLASS>
		struct variadic_constructor_caller :baseInvoker {
			variadic_constructor_caller() {}
			virtual bool checktype(lua_State *state, bool) { return true; }

			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::type_tag<LuaRef>()));
				}
				void *storage = lua_newuserdata(state, sizeof(CLASS));
				types::constructor<CLASS>(storage, args);
				luaL_setmetatable(state, types::metatable_name<CLASS>().c_str());
				return 1;
			}
		};
	}
};
