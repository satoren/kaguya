#pragma once

#include <string>
#include <vector>

#include "kaguya/config.hpp"
#include "kaguya/utility.hpp"
#include "kaguya/type.hpp"
#include "kaguya/lua_ref.hpp"


namespace kaguya
{
	typedef const std::vector<LuaRef>& VariadicArgType;
	namespace nativefunction
	{
		//variadic argment
		template<typename Ret>
		struct VariadicArgInvoker :BaseInvoker {
			typedef Ret(*func_type)(VariadicArgType);
			func_type func_;
			VariadicArgInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}
				Ret r = func_(args);
				return types::push(state, standard::forward<Ret>(r));
			}
			virtual std::string argumentTypeNames() {
				return "VariadicArg";
			}
		};
		template<typename Ret>
		FunctorType create(Ret(*fun)(VariadicArgType))
		{
			typedef VariadicArgInvoker<Ret> caller_type;
			return FunctorType(new caller_type(fun));
		}

		struct VariadicArgVoidInvoker :BaseInvoker {
			typedef void(*func_type)(VariadicArgType);
			func_type func_;
			VariadicArgVoidInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}
				func_(args);
				return 0;
			}

			virtual std::string argumentTypeNames() {
				return "VariadicArg";
			}
		};
		FunctorType create(void(*fun)(VariadicArgType))
		{
			typedef VariadicArgVoidInvoker caller_type;
			return FunctorType(new caller_type(fun));
		}

		template <typename Ret, typename T>
		struct VariadicArgMemFunInvoker :BaseInvoker {
			typedef Ret(T::*func_type)(VariadicArgType);
			func_type func_;
			VariadicArgMemFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::typetag<T*>());
				if (!t) { return 0; }

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}

				Ret r = (t->*func_)(args);
				return types::push(state, standard::forward<Ret>(r));
			}


			virtual std::string argumentTypeNames() {
				std::string result;
				result += typeid(T).name();
				result += ",VariadicArg";
				return result;
			}
		};
		template <typename Ret, typename T>
		FunctorType create(Ret(T::*fun)(VariadicArgType))
		{
			typedef VariadicArgMemFunInvoker<Ret, T> caller_type;
			return FunctorType(new caller_type(fun));
		}

		template <typename T>
		struct VariadicArgMemVoidFunInvoker :BaseInvoker {
			typedef void (T::*func_type)(VariadicArgType);
			func_type func_;
			VariadicArgMemVoidFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::typetag<T*>());
				if (!t) { return 0; }

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}

				(t->*func_)(args);
				return 0;
			}
			virtual std::string argumentTypeNames() {
				std::string result;
				result += typeid(T).name();
				result += ",VariadicArg";
				return result;
			}
		};
		template <typename T>
		FunctorType create(void (T::*fun)(VariadicArgType))
		{
			typedef VariadicArgMemVoidFunInvoker<T> caller_type;
			return FunctorType(new caller_type(fun));
		}


		template <typename Ret, typename T>
		struct VariadicArgConstMemFunInvoker :BaseInvoker {
			typedef Ret(T::*func_type)(VariadicArgType)const;
			func_type func_;
			VariadicArgConstMemFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::typetag<T*>());
				if (!t) { return 0; }

				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top-1);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}

				Ret r = (t->*func_)(args);
				return types::push(state, standard::forward <Ret>(r));
			}
			virtual std::string argumentTypeNames() {
				std::string result;
				result += typeid(T).name();
				result += ",VariadicArg";
				return result;
			}
		};
		template <typename Ret, typename T>
		FunctorType create(Ret(T::*fun)(VariadicArgType)const)
		{
			typedef VariadicArgConstMemFunInvoker<Ret, T> caller_type;
			return FunctorType(new caller_type(fun));
		}

		template <typename T>
		struct VariadicArgConstMemVoidFunInvoker :BaseInvoker {
			typedef void (T::*func_type)(VariadicArgType)const;
			func_type func_;
			VariadicArgConstMemVoidFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				T* t = types::get(state, 1, types::typetag<T*>());
				if (!t) { return 0; }
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top-1);
				for (int i = 2; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}

				(t->*func_)(args);
				return 0;
			}
			virtual std::string argumentTypeNames() {
				std::string result;
				result += typeid(T).name();
				result += ",VariadicArg";
				return result;
			}
		};
		template <typename T>
		FunctorType create(void (T::*fun)(VariadicArgType)const)
		{
			typedef VariadicArgConstMemVoidFunInvoker<T> caller_type;
			return FunctorType(new caller_type(fun));
		}


		template<typename Ret>
		struct VariadicArgFunInvoker :BaseInvoker {
			typedef standard::function<Ret(VariadicArgType)> func_type;
			func_type func_;
			VariadicArgFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}
				Ret r = func_(args);
				return types::push(state, standard::forward<Ret>(r));
			}
			virtual std::string argumentTypeNames() {
				return "VariadicArg";
			}
		};
		template<typename Ret>
		FunctorType create(standard::function<Ret(VariadicArgType)> fun)
		{
			typedef VariadicArgFunInvoker<Ret> caller_type;
			return FunctorType(new caller_type(fun));
		}

		struct VariadicArgVoidFunInvoker :BaseInvoker {
			typedef standard::function<void(VariadicArgType)> func_type;
			func_type func_;
			VariadicArgVoidFunInvoker(func_type fun) :func_(fun) {}
			virtual bool checktype(lua_State *state,bool) { return true; }
			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}
				func_(args);
				return 0;
			}
			virtual std::string argumentTypeNames() {
				return "VariadicArg";
			}
		};
		FunctorType create(standard::function<void(VariadicArgType)> fun)
		{
			typedef VariadicArgVoidFunInvoker caller_type;
			return FunctorType(new caller_type(fun));
		}

		template<typename CLASS>
		struct VariadicArgConstructorInvoker :BaseInvoker {
			VariadicArgConstructorInvoker() {}
			virtual bool checktype(lua_State *state, bool) { return true; }

			virtual int invoke(lua_State *state)
			{
				std::vector<LuaRef> args;
				int top = lua_gettop(state);
				args.reserve(top + 1);
				for (int i = 1; i <= top; ++i)
				{
					args.push_back(types::get(state, i, types::typetag<LuaRef>()));
				}
				void *storage = lua_newuserdata(state, sizeof(CLASS));
				types::constructor<CLASS>(storage, args);
				luaL_setmetatable(state, types::metatableName<CLASS>().c_str());
				return 1;
			}
			virtual std::string argumentTypeNames() {
				return "VariadicArg";
			}
		};
	}
};
