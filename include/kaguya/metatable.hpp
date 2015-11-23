#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <iostream>

#include "kaguya/config.hpp"
#include "kaguya/native_function.hpp"

namespace kaguya
{

	struct Metatable
	{
		struct value_type
		{
			value_type(int v) :ivalue(v), type(int_value){}
			value_type(int64_t v) :ivalue(v), type(int_value) {}
			value_type(float v) :dvalue(v), type(double_value) {}
			value_type(double v) :dvalue(v), type(double_value) {}
			value_type(const std::string& v) :strvalue(v), type(str_value) {}
			value_type(const char* v) :strvalue(v), type(str_value) {}
			enum {str_value,int_value,double_value} type;
			std::string strvalue;
			int64_t ivalue;
			double dvalue;
		};

		typedef nativefunction::functor_type functor_type;
		typedef std::vector<functor_type> func_array_type;
		typedef std::map<std::string, func_array_type> func_map_type;

		typedef std::map<std::string, value_type> value_map;



		Metatable(std::string name) :metatable_name_(name)
		{
		}
		Metatable(std::string name, std::string parent_name) :metatable_name_(name), parent_metatable_name_(parent_name)
		{
		}

		const std::string name()const { return metatable_name_; }


		template<typename Fun>
		Metatable& addFunction(const char* name, Fun f)
		{
			functor_type fun(kaguya::nativefunction::create(f));
			member_function_map_[name].push_back(fun);
			return *this;
		}

		void registerTable(lua_State* state)const
		{
			luaL_newmetatable(state, metatable_name_.c_str());
			if (!parent_metatable_name_.empty())
			{
				luaL_setmetatable(state, parent_metatable_name_.c_str());
			}
			registerFunctions(state);
			lua_pushvalue(state, -1);
			lua_setfield(state, -1, "__index");
		}
	protected:
		void registerFunction(lua_State* state,const char* name, const func_array_type& func_array)const
		{
			if (func_array.empty()) { return; }
			lua_pushnumber(state, func_array.size());
			for (func_array_type::const_iterator f = func_array.begin(); f != func_array.end(); ++f)
			{
				void *storage = lua_newuserdata(state, sizeof(functor_type));
				new(storage) functor_type(*f);
				luaL_setmetatable(state, KAGUYA_FUNCTOR_METATABLE);
			}
			lua_pushcclosure(state, &nativefunction::functor_dispatcher, func_array.size() + 1);
			lua_setfield(state, -2, name);
		}
		void registerFunctions(lua_State* state)const
		{
			for (func_map_type::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
			for (func_map_type::const_iterator it = member_function_map_.begin(); it != member_function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
		}
		func_map_type function_map_;
		func_map_type member_function_map_;
		std::string metatable_name_;
		std::string parent_metatable_name_;
	};

	template<typename class_type>
	struct ClassMetatable :Metatable
	{
		ClassMetatable() :Metatable(types::metatable_name<class_type>())
		{
			functor_type dtor(kaguya::nativefunction::create(&types::destructor<class_type>));
			function_map_["__gc"].push_back(dtor);
		}

#include "gen/add_constructor.inl"

		//variadic argment constructor(receive const std::vector<LuaRef>&)
//		template<>ClassMetatable& addConstructor(types::type_tag<variadic_arg_type>* )
		ClassMetatable& addConstructorVariadicArg()
		{
			functor_type fun(new kaguya::nativefunction::variadic_constructor_caller<class_type>());
			function_map_["new"].push_back(fun);
			return *this;
		}

		template<typename Fun>
		ClassMetatable& addMember(const char* name, Fun f)
		{
			functor_type fun(kaguya::nativefunction::create(f));
			member_function_map_[name].push_back(fun);
			return *this;
		}

		void setMembers(const func_map_type& funcs)
		{
			member_function_map_ = funcs;
		}
		func_map_type members()const {
			return member_function_map_;
		}
	};
};
