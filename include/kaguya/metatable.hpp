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
		/*
		struct value_type
		{
			value_type(int v) :ivalue(v), type(int_value){}
			value_type(int64_t v) :ivalue(v), type(int_value) {}
			value_type(float v) :dvalue(v), type(double_value) {}
			value_type(double v) :dvalue(v), type(double_value) {}
			value_type(const std::string& v) :strvalue(v), type(str_value) {}
			value_type(const char* v) :strvalue(v), type(str_value) {}
			std::string strvalue;
			int64_t ivalue;
			double dvalue;
			enum {str_value,int_value,double_value} type;
		};*/

		typedef nativefunction::FunctorType FunctorType;
		typedef std::vector<FunctorType> FuncArrayType;
		typedef std::map<std::string, FuncArrayType> FuncMapType;

//		typedef std::map<std::string, value_type> value_map;



		Metatable(std::string name) :metatableName_(name)
		{
		}
		Metatable(std::string name, std::string parent_name) :metatableName_(name), parent_metatableName_(parent_name)
		{
		}

		const std::string name()const { return metatableName_; }


		template<typename Fun>
		Metatable& addFunction(const char* name, Fun f)
		{
			FunctorType fun(kaguya::nativefunction::create(f));
			member_function_map_[name].push_back(fun);
			return *this;
		}

		void registerTable(lua_State* state)const
		{
			if (luaL_newmetatable(state, metatableName_.c_str()))
			{
				registerFunctions(state);
				lua_pushvalue(state, -1);
				lua_setfield(state, -1, "__index");
				
				if (!parent_metatableName_.empty())
				{
					luaL_setmetatable(state, parent_metatableName_.c_str());
				}
			}
			else
			{
				except::OtherError(state, metatableName_ + " is already registered");
			}
		}
	protected:
		void registerFunction(lua_State* state,const char* name, const FuncArrayType& func_array)const
		{
			if (func_array.empty()) { return; }
			lua_pushnumber(state, func_array.size());
			for (FuncArrayType::const_iterator f = func_array.begin(); f != func_array.end(); ++f)
			{
				void *storage = lua_newuserdata(state, sizeof(FunctorType));
				new(storage) FunctorType(*f);
				luaL_setmetatable(state, KAGUYA_FUNCTOR_METATABLE);
			}
			lua_pushcclosure(state, &nativefunction::functor_dispatcher, func_array.size() + 1);
			lua_setfield(state, -2, name);
		}
		void registerFunctions(lua_State* state)const
		{
			for (FuncMapType::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
			for (FuncMapType::const_iterator it = member_function_map_.begin(); it != member_function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
		}
		FuncMapType function_map_;
		FuncMapType member_function_map_;
		std::string metatableName_;
		std::string parent_metatableName_;
	};

	template<typename class_type>
	struct ClassMetatable :Metatable
	{
		ClassMetatable(std::string parent_metatable="") :Metatable(types::metatableName<class_type>(), parent_metatable)
		{
			FunctorType dtor(kaguya::nativefunction::create(&types::destructor<class_type>));
			function_map_["__gc"].push_back(dtor);
		}

#include "gen/add_constructor.inl"

		//variadic argment constructor(receive const std::vector<LuaRef>&)
//		template<>ClassMetatable& addConstructor(types::typetag<VariadicArgType>* )
		ClassMetatable& addConstructorVariadicArg()
		{
			FunctorType fun(new kaguya::nativefunction::VariadicArgConstructorInvoker<class_type>());
			function_map_["new"].push_back(fun);
			return *this;
		}

		template<typename Fun>
		ClassMetatable& addMember(const char* name, Fun f)
		{
			FunctorType fun(kaguya::nativefunction::create(f));
			member_function_map_[name].push_back(fun);
			return *this;
		}

		void setMembers(const FuncMapType& funcs)
		{
			member_function_map_ = funcs;
		}
		FuncMapType members()const {
			return member_function_map_;
		}
	};
};
