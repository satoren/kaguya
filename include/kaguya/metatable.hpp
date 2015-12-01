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
		struct ValueType
		{
			ValueType() :ivalue(0), dvalue(0), type(int_value) {}
			ValueType(int v) :ivalue(v), dvalue(0), type(int_value) {}
			ValueType(long long v) :ivalue(v), dvalue(0), type(int_value) {}
			ValueType(float v) :ivalue(0), dvalue(v), type(double_value) {}
			ValueType(double v) :ivalue(0), dvalue(v), type(double_value) {}
			ValueType(const std::string& v) :strvalue(v), ivalue(0), dvalue(0), type(str_value) {}
			ValueType(const char* v) :strvalue(v), ivalue(0), dvalue(0), type(str_value) {}
			std::string strvalue;
			long long ivalue;
			double dvalue;
			enum { str_value, int_value, double_value } type;
		};

		typedef nativefunction::FunctorType FunctorType;
		typedef std::vector<FunctorType> FuncArrayType;
		typedef std::map<std::string, FuncArrayType> FuncMapType;

		typedef std::map<std::string, ValueType> ValueMapType;



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

		Metatable& addField(const char* name, const std::string& str)
		{
			return addField(name, str.c_str());
		}
		Metatable& addField(const char* name, const char* str)
		{
			value_map_[name] = ValueType(str);
			return *this;
		}
		Metatable& addField(const char* name, double v)
		{
			value_map_[name] = ValueType(v);
			return *this;
		}
		Metatable& addField(const char* name, long long v)
		{
			value_map_[name] = ValueType(v);
			return *this;
		}
		Metatable& addField(const char* name, int v)
		{
			value_map_[name] = ValueType(v);
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
		void registerFunction(lua_State* state, const char* name, const FuncArrayType& func_array)const
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
		void registerField(lua_State* state, const char* name, const ValueType& value)const
		{
			if (value.type == ValueType::str_value)
			{
				types::push(state, value.strvalue);
			}
			else if (value.type == ValueType::int_value)
			{
				types::push(state, value.ivalue);
			}
			else if (value.type == ValueType::double_value)
			{
				types::push(state, value.dvalue);
			}
			else
			{
				assert(false);
			}
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
			for (ValueMapType::const_iterator it = value_map_.begin(); it != value_map_.end(); ++it)
			{
				registerField(state, it->first.c_str(), it->second);
			}
		}
		FuncMapType function_map_;
		FuncMapType member_function_map_;
		ValueMapType value_map_;
		std::string metatableName_;
		std::string parent_metatableName_;
	};

	template<typename class_type>
	struct ClassMetatable :Metatable
	{
		ClassMetatable(std::string parent_metatable = "") :Metatable(types::metatableName<class_type>(), parent_metatable)
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

#if defined(_MSC_VER) && _MSC_VER <= 1800
		//can not write  Ret class_type::* f on MSC++2013
		template<typename Fun>
		ClassMetatable& addMember(const char* name, Fun f)
		{
			addFunction(name, f);
			return *this;
		}
#else
		template<typename Ret>
		ClassMetatable& addMember(const char* name, Ret class_type::* f)
		{
			addFunction(name, f);
			return *this;
		}
#endif

		template<typename Fun>
		ClassMetatable& addStaticMember(const char* name, Fun f)
		{
			addFunction(name, f);
			return *this;
		}

		template<typename Data>
		ClassMetatable& addStaticField(const char* name, Data f)
		{
			addField(name, f);
			return *this;
		}
	};
};
