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
		typedef std::map<std::string, std::string> CodeChunkMapType;



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
			function_map_[name].push_back(fun);
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

		Metatable& addCodeChunkResult(const char* name, const std::string& lua_code_chunk)
		{
			code_chunk_map_[name] = lua_code_chunk;
			return *this;
		}


		void registerTable(lua_State* state)const
		{
			if (luaL_newmetatable(state, metatableName_.c_str()))
			{
				registerMember(state);
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
		bool has_key(const std::string& key, bool exclude_function = false)
		{
			if (!exclude_function && function_map_.find(key) != function_map_.end())
			{
				return true;
			}
			if (value_map_.find(key) != value_map_.end())
			{
				return true;
			}
			return false;
		}
		void registerFunction(lua_State* state, const char* name, const FuncArrayType& func_array)const
		{
			int funcnum = int(func_array.size());
			if (funcnum == 0) { return; }
			types::push(state, funcnum);
			for (FuncArrayType::const_iterator f = func_array.begin(); f != func_array.end(); ++f)
			{
				void *storage = lua_newuserdata(state, sizeof(FunctorType));
				new(storage) FunctorType(*f);
				luaL_setmetatable(state, KAGUYA_FUNCTOR_METATABLE);
			}
			lua_pushcclosure(state, &nativefunction::functor_dispatcher, funcnum + 1);
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
		void registerCodeChunk(lua_State* state, const char* name, std::string value)const
		{
			util::ScopedSavedStack save(state);
			int status = luaL_loadstring(state, value.c_str());
			if (!except::checkErrorAndThrow(status, state)) { return; }
			status = lua_pcall(state, 0, 1, 0);
			if (!except::checkErrorAndThrow(status, state)) { return; }
			lua_setfield(state, -2, name);
		}
		void registerMember(lua_State* state)const
		{
			for (FuncMapType::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
			for (ValueMapType::const_iterator it = value_map_.begin(); it != value_map_.end(); ++it)
			{
				registerField(state, it->first.c_str(), it->second);
			}
			for (CodeChunkMapType::const_iterator it = code_chunk_map_.begin(); it != code_chunk_map_.end(); ++it)
			{
				registerCodeChunk(state, it->first.c_str(), it->second);
			}
		}
		FuncMapType function_map_;
		ValueMapType value_map_;
		CodeChunkMapType code_chunk_map_;
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
		//variadic arguments constructor(receive const std::vector<LuaRef>&)
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
			if (has_key(name, true))
			{
				//already registerd
				return *this;
			}
			addFunction(name, f);
			return *this;
		}
#else
		template<typename Ret>
		ClassMetatable& addMember(const char* name, Ret class_type::* f)
		{
			if (has_key(name, true))
			{
				//already registerd
				return *this;
			}
			addFunction(name, f);
			return *this;
		}
#endif

		template<typename Fun>
		ClassMetatable& addStaticMember(const char* name, Fun f)
		{
			if (has_key(name, true))
			{
				//already registerd
				return *this;
			}
			addFunction(name, f);
			return *this;
		}

		//add field to 
		ClassMetatable& addCodeChunkResult(const char* name, const std::string& lua_code_chunk)
		{
			Metatable::addCodeChunkResult(name, lua_code_chunk);
			return *this;
		}


		template<typename Data>
		ClassMetatable& addStaticField(const char* name, Data f)
		{
			if (has_key(name))
			{
				//already registerd
				return *this;
			}
			addField(name, f);
			return *this;
		}
	};
};
