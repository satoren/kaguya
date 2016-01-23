#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <iostream>

#include "kaguya/config.hpp"
#include "kaguya/native_function.hpp"


#include "kaguya/lua_ref_function.hpp"

namespace kaguya
{

	template<typename class_type,typename base_class_type = void>
	struct ClassMetatable
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

		typedef std::vector<FunctorType> FuncArrayType;
		typedef std::map<std::string, FuncArrayType> FuncMapType;

		typedef std::map<std::string, ValueType> ValueMapType;
		typedef std::map<std::string, std::string> CodeChunkMapType;


		ClassMetatable():has_property_(false)
		{
			FunctorType dtor(&class_userdata::destructor<ObjectWrapperBase>);
			function_map_["__gc"].push_back(dtor);

			//type check
			class_type* check = 0;
			base_class_type* ptr = check; (void)(ptr);//unused

#if KAGUYA_USE_CPP11
			//can not register push specialized class
			//std::vector,std::map,std::tuple,
			KAGUYA_STATIC_ASSERT(!traits::is_push_specialized<class_type>::value,
				"Can not register push-specialized class. e.g. std::vector,std::map,std::tuple,std::");
#endif
		}

		LuaRef registerClass(lua_State* state)const
		{
			util::ScopedSavedStack save(state);
			if (class_userdata::newmetatable<class_type>(state))
			{
				LuaRef metatable(state, StackTop());


				if (has_property_)
				{
					LuaRef newindexfn = LuaFunction::loadstring(state, "return function(table, index, value) "
						"if type(table) == 'table' then rawset(table,index,value)"
						" else "
						" table['_prop_'..index](table,value);"
						" end"
						" end")();
					metatable.setField("__newindex", newindexfn);

					LuaRef indextable = createIndexTable(state);

					LuaRef indexfun = kaguya::LuaFunction::loadstring(state, "local arg = {...};local indextable = arg[1];"
						"return function(table, index)"
						" local propfun = indextable['_prop_'..index];"
						" if propfun then return propfun(table) end "
						" return indextable[index]"
						" end")(indextable);

					metatable.setField("__index", indexfun);
				}
				else
				{
					metatable.setField("__index", createIndexTable(state));
				}

				metatable.push();
				registerMetamethods(state);
				if (!traits::is_void<base_class_type>::value)
				{
					class_userdata::setmetatable<base_class_type>(state);
				}

				return metatable;
			}
			else
			{
				except::OtherError(state,typeid(class_type*).name() +  std::string("is already registered"));
			}
			return LuaRef(state);
		}

#if KAGUYA_USE_CPP11
		template<typename... Args>
		ClassMetatable& addConstructor()
		{
			function_map_["new"].push_back(FunctorType::ConstructorInvoker<class_type, Args...>());
			return *this;
		}
#else
#include "gen/add_constructor.inl"
#endif
		//variadic arguments constructor(receive const std::vector<LuaRef>&)
//		template<>ClassMetatable& addConstructor(types::typetag<VariadicArgType>* )
		ClassMetatable& addConstructorVariadicArg()
		{
			function_map_["new"].push_back(FunctorType::VariadicConstructorInvoker<class_type>());
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
		template<typename Ret>
		ClassMetatable& addProperty(const char* name, Ret class_type::* mem)
		{
			has_property_ = true;
			return addFunction((std::string("_prop_") + name).c_str(), mem);
		}

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
			code_chunk_map_[name] = lua_code_chunk;
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

	private:

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
			types::push_dispatch(state, funcnum);
			for (FuncArrayType::const_iterator f = func_array.begin(); f != func_array.end(); ++f)
			{
				void *storage = lua_newuserdata(state, sizeof(FunctorType));
				new(storage) FunctorType(*f);
				class_userdata::setmetatable<FunctorType>(state);
			}
			lua_pushcclosure(state, &nativefunction::functor_dispatcher, funcnum + 1);
			lua_setfield(state, -2, name);
		}
		void registerField(lua_State* state, const char* name, const ValueType& value)const
		{
			if (value.type == ValueType::str_value)
			{
				types::push_dispatch(state, value.strvalue);
			}
			else if (value.type == ValueType::int_value)
			{
				types::push_dispatch(state, value.ivalue);
			}
			else if (value.type == ValueType::double_value)
			{
				types::push_dispatch(state, value.dvalue);
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
		LuaRef createIndexTable(lua_State* state)const
		{
			util::ScopedSavedStack save(state);
			LuaRef indexTable(state, NewTable());
			indexTable.push();
			registerMember(state);

			if (!traits::is_void<base_class_type>::value)
			{
				class_userdata::setmetatable<base_class_type>(state);
			}

			return indexTable;
		}

		void registerMember(lua_State* state)const
		{
			for (typename FuncMapType::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				if (!is_metafield(it->first))
				{
					registerFunction(state, it->first.c_str(), it->second);
				}
			}
			for (typename ValueMapType::const_iterator it = value_map_.begin(); it != value_map_.end(); ++it)
			{
				if (!is_metafield(it->first))
				{
					registerField(state, it->first.c_str(), it->second);
				}
			}
			for (typename CodeChunkMapType::const_iterator it = code_chunk_map_.begin(); it != code_chunk_map_.end(); ++it)
			{
				if (!is_metafield(it->first))
				{
					registerCodeChunk(state, it->first.c_str(), it->second);
				}
			}
		}
		static bool is_metafield(const std::string& name)
		{
			return name.compare(0, 2, "__") == 0;
		}
		void registerMetamethods(lua_State* state)const
		{
			for (typename FuncMapType::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				if (is_metafield(it->first))
				{
					registerFunction(state, it->first.c_str(), it->second);
				}
			}
			for (typename ValueMapType::const_iterator it = value_map_.begin(); it != value_map_.end(); ++it)
			{
				if (is_metafield(it->first))
				{
					registerField(state, it->first.c_str(), it->second);
				}
			}
			for (typename CodeChunkMapType::const_iterator it = code_chunk_map_.begin(); it != code_chunk_map_.end(); ++it)
			{
				if (is_metafield(it->first))
				{
					registerCodeChunk(state, it->first.c_str(), it->second);
				}
			}
		}
		ClassMetatable& addField(const char* name, const std::string& str)
		{
			return addField(name, str.c_str());
		}
		ClassMetatable& addField(const char* name, const char* str)
		{
			value_map_[name] = ValueType(str);
			return *this;
		}
		ClassMetatable& addField(const char* name, double v)
		{
			value_map_[name] = ValueType(v);
			return *this;
		}
		ClassMetatable& addField(const char* name, long long v)
		{
			value_map_[name] = ValueType(v);
			return *this;
		}
		ClassMetatable& addField(const char* name, int v)
		{
			value_map_[name] = ValueType(v);
			return *this;
		}
		template<typename Fun>
		ClassMetatable& addFunction(const char* name, Fun f)
		{
			function_map_[name].push_back(f);
			return *this;
		}

		FuncMapType function_map_;
		ValueMapType value_map_;
		CodeChunkMapType code_chunk_map_;
		bool has_property_;
	};
};
