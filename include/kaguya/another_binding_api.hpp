// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include "kaguya/kaguya.hpp"


/// @addtogroup another_binding_api
/// @brief Boost.python like binding API.(experimental)
/// @{


#if defined(KAGUYA_DYNAMIC_LIB)
#if defined(_WIN32) || defined(_WIN64)
#define KAGUYA_EXPORT extern "C" __declspec(dllexport)
#else
#define KAGUYA_EXPORT extern "C" __attribute__((visibility("default")))
#endif
#else
#define KAGUYA_EXPORT
#endif

/// @brief start define binding 
#define KAGUYA_BINDINGS(MODULE_NAME)\
void KAGUYA_PP_CAT(kaguya_bind_internal_,MODULE_NAME)();\
KAGUYA_EXPORT int KAGUYA_PP_CAT(luaopen_, MODULE_NAME)(lua_State* L) { return kaguya::detail::bind_internal(L,&KAGUYA_PP_CAT(kaguya_bind_internal_, MODULE_NAME)); }\
void KAGUYA_PP_CAT(kaguya_bind_internal_, MODULE_NAME)()

namespace kaguya
{
	namespace detail
	{
		struct scope_stack
		{
			LuaTable current_scope()
			{
				return !stack.empty() ? stack.back() : LuaTable();
			}
			static scope_stack& instance()
			{
				static scope_stack instance_;
				return instance_;
			}
			void push(const LuaTable& table)
			{
				stack.push_back(table);
			}
			void pop()
			{
				stack.pop_back();
			}
		private:
			std::vector<LuaTable> stack;
		};
	}


	/// @ingroup another_binding_api
	/// @brief binding scope
	struct scope
	{
		scope(const std::string& name)
		{
			detail::scope_stack& stack = detail::scope_stack::instance();
			LuaTable current = stack.current_scope();
			if (!current[name])
			{
				current[name] = NewTable();
			}
			stack.push(current[name]);
		}
		scope(const LuaTable& t)
		{
			detail::scope_stack::instance().push(t);
		}
		~scope()
		{
			detail::scope_stack::instance().pop();
		}
	};


	namespace detail
	{
		inline int bind_internal(lua_State* L, void(*bindfn)()) {
			int count = lua_gettop(L);
			kaguya::State state(L);
			LuaTable l = state.newTable();
			l.push();
			scope scope(l);
			bindfn();
			return lua_gettop(L) - count;
		}
	}

	/// @ingroup another_binding_api
	/// @brief define class binding 
	template<typename ClassType, typename BaseType = void>
	struct class_ : private UserdataMetatable<ClassType, BaseType>
	{
		class_(const std::string& name) :name_(name) {
		}
		~class_() {
			LuaTable scope = detail::scope_stack::instance().current_scope();
			if (scope)
			{
				scope[name_].setClass(*this);
			}
		}

#if KAGUYA_USE_CPP11
		template<typename... Args>
		class_& constructor() { this->template setConstructors<ClassType(Args...)>(); return *this; }
#else
#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_ADD_CON_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	class_& constructor() { this->template setConstructors<ClassType(KAGUYA_PP_TEMPLATE_ARG_REPEAT(N))>(); return *this; }

		KAGUYA_ADD_CON_FN_DEF(0)
#undef KAGUYA_TEMPLATE_PARAMETER
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_ADD_CON_FN_DEF)
#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_ADD_CON_FN_DEF
#endif

		/// @brief function binding 
		template<typename F>
		class_& function(const char* name, F f) { this-> addFunction(name, f); return *this; }


		/// @brief property binding 
		template<typename F>
		class_& property(const char* name, F f) { this->addPropaty(name, f); return *this; }

		/// @brief property binding with getter and sette function
		template<typename Getter, typename Setter>
		class_& property(const char* name, Getter getter, Setter setter) { this->addPropaty(name, getter, setter); return *this; }

		/// @brief class function binding
		template<typename F>
		class_& class_function(const char* name, F f) { this->addStaticFunction(name, f); return *this; }

		/// @defgroup boostpythonlikeapi Boost.python like interface
		/// @{

		/// @brief function binding 
		template<typename F>
		class_& def(const char* name, F f) { this->addFunction(name, f); return *this; }

		/// @brief property binding with getter and sette function
		template<typename Getter, typename Setter>
		class_& add_property(const char* name, Getter getter, Setter setter) { property(name, getter, setter); return *this; }

		/// @brief property binding 
		template<typename F>
		class_& add_property(const char* name, F f) { property(name, f); return *this; }


		/// @brief static property binding 
		template<typename Data>
		class_& add_static_property(const char* name, Data data) { this->addStaticField(name, data); return *this; }

		/// @}

	private:
		std::string name_;
	};

	/// @ingroup another_binding_api
	/// @brief function binding 
	template<typename F>
	void function(const char* name, F f) {
		LuaTable scope = detail::scope_stack::instance().current_scope();
		if (scope)
		{
			scope[name] = kaguya::function(f);
		}
	}

	/// @ingroup boostpythonlikeapi
	/// @brief function binding 
	template<typename F>
	void def(const char* name, F f) { kaguya::function(name, f); }
}
/// @}

