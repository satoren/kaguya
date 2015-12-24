#pragma once

#include <string>
#include <vector>

#include "kaguya/config.hpp"
#include "kaguya/utility.hpp"
#include "kaguya/type.hpp"
#include "kaguya/lua_ref.hpp"


namespace kaguya
{
	namespace nativefunction
	{
		struct BaseInvoker
		{
			virtual bool checktype(lua_State *state, bool strictcheck) = 0;
			virtual int invoke(lua_State *state) = 0;
			virtual std::string argumentTypeNames() = 0;
			virtual ~BaseInvoker() {}
		};

		typedef standard::shared_ptr<nativefunction::BaseInvoker> FunctorType;

		inline FunctorType* pick_match_function(lua_State *l)
		{
			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));

			if (overloadnum == 1)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(2)));

				if (!(*fun)->checktype(l, false))
				{
					return 0;
				}
				return fun;
			}
			FunctorType* weak_match = 0;
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if (!fun || !(*fun))
				{
					continue;
				}
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
		std::string build_arg_error_message(lua_State *l)
		{
			std::string message = "argument not matching" + util::argmentTypes(l) + "\t candidated\n";

			int overloadnum = int(lua_tonumber(l, lua_upvalueindex(1)));
			for (int i = 0; i < overloadnum; ++i)
			{
				FunctorType* fun = static_cast<FunctorType*>(lua_touserdata(l, lua_upvalueindex(i + 2)));
				if (!fun || !(*fun))
				{
					continue;
				}
				message += std::string("\t\t") + (*fun)->argumentTypeNames() + "\n";
			}
			return message;
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
					util::traceBack(l, e.what());
				}
				catch (...) {
					util::traceBack(l, "Unknown exception");
				}
			}
			else
			{
				util::traceBack(l, build_arg_error_message(l).c_str());
			}
			return lua_error(l);
		}

		inline int functor_destructor(lua_State *state)
		{
			FunctorType* f = types::class_userdata::test_userdata<FunctorType>(state, 1);
			if (f)
			{
				f->~FunctorType();
			}
			return 0;
		}
		inline void reg_functor_destructor(lua_State* state)
		{
			if (types::class_userdata::newmetatable<FunctorType>(state))
			{
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

			virtual std::string argumentTypeNames() {
				std::string result;
				result += typeid(ClassType).name();
				result += std::string("[opt]") + typeid(MemType).name();
				return result;
			}
		};
		template< class ClassType, class MemType>
		FunctorType create(MemType ClassType::*data)
		{
			typedef MemDataInvoker<ClassType, MemType> invoker_type;
			return standard::shared_ptr<BaseInvoker>(new invoker_type(data));
		}


#if KAGUYA_USE_DECLTYPE
		template <typename T>
		struct lambda_fun : public lambda_fun<decltype(&T::operator())> {};

		template <typename T, typename Ret, typename... Args>
		struct lambda_fun<Ret(T::*)(Args...) const> {
			typedef standard::function<Ret(Args...)> type;
		};

		//for lambda or bind
		template< typename L>
		FunctorType create(L l)
		{
			return create(typename lambda_fun<L>::type(l));
		}
#endif
	}


	typedef nativefunction::FunctorType FunctorType;
	template<typename T>
	inline FunctorType lua_function(T f)
	{
		return FunctorType(nativefunction::create(standard::forward<T>(f)));
	}

	template<typename T>
	inline FunctorType function(T f)
	{
		return lua_function(standard::forward<T>(f));
	}
	namespace types
	{
		template<>
		inline nativefunction::FunctorType get(lua_State* l, int index, typetag<nativefunction::FunctorType> tag)
		{
			nativefunction::FunctorType* ptr = types::class_userdata::test_userdata<nativefunction::FunctorType>(l, index);
			if (ptr)
			{
				return *ptr;
			}
			return nativefunction::FunctorType();
		}
		template<>
		inline int push(lua_State* l, const nativefunction::FunctorType& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::FunctorType));
			new(storage) nativefunction::FunctorType(f);
			types::class_userdata::setmetatable<FunctorType>(l);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#if KAGUYA_USE_RVALUE_REFERENCE
		inline int push(lua_State* l, nativefunction::FunctorType&& f)
		{
			lua_pushnumber(l, 1);//no overload
			void *storage = lua_newuserdata(l, sizeof(nativefunction::FunctorType));
			new(storage) nativefunction::FunctorType(standard::forward<nativefunction::FunctorType>(f));
			types::class_userdata::setmetatable<FunctorType>(l);
			lua_pushcclosure(l, &nativefunction::functor_dispatcher, 2);
			return 1;
		}
#endif
	}
};

#include "kaguya/variadic_function.hpp"