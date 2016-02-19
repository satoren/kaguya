// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

	template<typename class_type, typename base_class_type = void>
	struct ClassMetatable
	{
		struct DataHolderBase
		{
			virtual int push_to_lua(lua_State* data)const = 0;
			virtual ~DataHolderBase(){}
		};
		template<typename T>
		struct DataHolder:DataHolderBase
		{
			DataHolder(const T& d):data(d) {}
			T data;
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<T>::push(state, data);
			}
		};
		template<int N>
		struct DataHolder<char[N]> :DataHolderBase {
			std::string data;
			DataHolder(const std::string& d) :data(d) {}
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<std::string>::push(state, data);
			}		
		};
		template<int N>
		struct DataHolder<const char[N]> :DataHolderBase {
			std::string data;
			DataHolder(const std::string& d) :data(d) {}
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<std::string>::push(state, data);
			}
		};
#if KAGUYA_USE_CPP11
		template<typename T>
		struct MoveDataHolder :DataHolderBase
		{
			MoveDataHolder(T&& d) :data(std::move(d)) {}
			T data;
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<T>::push(state, std::move(data));
			}
		};
		template<int N>
		struct MoveDataHolder<char[N]> :DataHolderBase {
			std::string data;
			MoveDataHolder(const std::string& d) :data(d) {}
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<std::string>::push(state, data);
			}
		};
		template<int N>
		struct MoveDataHolder<const char[N]> :DataHolderBase {
			std::string data;
			MoveDataHolder(const std::string& d) :data(d) {}
			virtual int push_to_lua(lua_State* state)const
			{
				return lua_type_traits<std::string>::push(state, data);
			}
		};
#endif
		typedef standard::shared_ptr<DataHolderBase> DataHolderType;

		typedef std::vector<FunctorType> FunctorOverloadType;
		typedef std::map<std::string, FunctorOverloadType> FuncMapType;

		typedef std::map<std::string, DataHolderType> MemberMapType;
		typedef std::map<std::string, std::string> CodeChunkMapType;


		ClassMetatable() :has_property_(false)
		{
			FunctorType dtor(&class_userdata::destructor<ObjectWrapperBase>);
			function_map_["__gc"].push_back(dtor);

			//type check
			class_type* check = 0;
			base_class_type* ptr = check; (void)(ptr);//unused


			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value || !traits::is_std_vector<class_type>::value, "std::vector is binding to lua-table by default.If you wants register for std::vector yourself,"
				"please define KAGUYA_NO_STD_VECTOR_TO_TABLE");

			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value || !traits::is_std_map<class_type>::value, "std::map is binding to lua-table by default.If you wants register for std::map yourself,"
				"please define KAGUYA_NO_STD_MAP_TO_TABLE");

			//can not register push specialized class
			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value,
				"Can not register specialized of type conversion class. e.g. std::tuple");
		}

		LuaRef registerClass(lua_State* state)const
		{
			util::ScopedSavedStack save(state);
			if (class_userdata::newmetatable<class_type>(state))
			{
				LuaTable metatable(state, StackTop());
				metatable.push();
				registerMember(state);

				if (has_property_)
				{
					LuaRef indexfun = kaguya::LuaFunction::loadstring(state, "local arg = {...};local indextable = arg[1];"
						"return function(table, index)"
						" local propfun = indextable['_prop_'..index];"
						" if propfun then return propfun(table) end "
						" return indextable[index]"
						" end")(metatable);

					metatable.setField("__index", indexfun);

					LuaRef newindexfn = LuaFunction::loadstring(state, "return function(table, index, value) "
						"if type(table) == 'table' then rawset(table,index,value)"
						" else "
						" table['_prop_'..index](table,value);"
						" end"
						" end")();
					metatable.setField("__newindex", newindexfn);
				}
				else
				{
					metatable.setField("__index", metatable);
				}

				if (!traits::is_void<base_class_type>::value)
				{
					class_userdata::setmetatable<base_class_type>(state);
				}

				return metatable;
			}
			else
			{
				except::OtherError(state, typeid(class_type*).name() + std::string(" is already registered"));
			}
			return LuaRef(state);
		}

#if KAGUYA_USE_CPP11
		template<typename... Args>
		ClassMetatable& addConstructor()
		{
			typedef typename nativefunction::constructor_signature_type<class_type, Args...> cons;
			function_map_["new"].push_back(cons());
			return *this;
		}
#else
#define KAGUYA_TEMPLATE_PARAMETER(N)
#define KAGUYA_ADD_CON_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline ClassMetatable& addConstructor()\
	{\
		typedef typename nativefunction::constructor_signature_type<class_type KAGUYA_PP_TEMPLATE_ARG_REPEAT_CONCAT(N)> cons;\
		function_map_["new"].push_back(cons());\
		return *this;\
	}
	KAGUYA_ADD_CON_FN_DEF(0)
#undef KAGUYA_TEMPLATE_PARAMETER
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>
	KAGUYA_PP_REPEAT_DEF(9, KAGUYA_ADD_CON_FN_DEF)
#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_ADD_CON_FN_DEF
#endif
		//variadic arguments constructor(receive const std::vector<LuaRef>&)
//		template<>ClassMetatable& addConstructor(types::typetag<VariadicArgType>* )
		ClassMetatable& addConstructorVariadicArg()
		{
			typedef typename nativefunction::constructor_signature_type<class_type, VariadicArgType> cons;
			function_map_["new"].push_back(FunctorType(cons()));
			return *this;
		}

#if defined(_MSC_VER) && _MSC_VER <= 1800
	//deprecated  rename to addMemberFunction
		//can not write  Ret class_type::* f on MSC++2013
		template<typename Fun>
		ClassMetatable& addMember(const char* name, Fun f)
		{
			return addMemberFunction(name,f);
		}
		//can not write  Ret class_type::* f on MSC++2013
		template<typename Fun>
		ClassMetatable& addMemberFunction(const char* name, Fun f)
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
	//deprecated  rename to addMemberFunction
		template<typename Ret>
		ClassMetatable& addMember(const char* name, Ret class_type::* f)
		{
			return addMemberFunction(name,f);
		}

		template<typename Ret>
		ClassMetatable& addMemberFunction(const char* name, Ret class_type::* f)
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
		//add member property
		template<typename Ret>
		ClassMetatable& addProperty(const char* name, Ret class_type::* mem)
		{
			has_property_ = true;
			return addFunction((std::string("_prop_") + name).c_str(), mem);
		}


		//deprecated rename to addStaticFunction
		template<typename Fun>
		ClassMetatable& addStaticMember(const char* name, Fun f)
		{
			return addStaticFunction(name,f);
		}
		template<typename Fun>
		ClassMetatable& addStaticFunction(const char* name, Fun f)
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
		ClassMetatable& addStaticField(const char* name,const Data& f)
		{
			if (has_key(name))
			{
				//already registerd
				return *this;
			}
			member_map_[name] = DataHolderType(new DataHolder<Data>(f));
			return *this;
		}

#if KAGUYA_USE_CPP11
		template<typename Data>
		ClassMetatable& addStaticField(const char* name, Data&& f)
		{
			if (has_key(name))
			{
				//already registerd
				return *this;
			}
			member_map_[name] = DataHolderType(new MoveDataHolder<Data>(std::move(f)));
			return *this;
		}
#endif
	private:

		bool has_key(const std::string& key, bool exclude_function = false)
		{
			if (!exclude_function && function_map_.find(key) != function_map_.end())
			{
				return true;
			}
			if (member_map_.find(key) != member_map_.end())
			{
				return true;
			}
			return false;
		}
		void registerFunction(lua_State* state, const char* name, const FunctorOverloadType& func_array)const
		{
			if (lua_type_traits<FunctorOverloadType>::push(state, func_array))
			{
				lua_setfield(state, -2, name);
			}
		}
		void registerField(lua_State* state, const char* name, const DataHolderType& value)const
		{
			int count = value->push_to_lua(state);
			if (count > 1)
			{
				lua_pop(state,count - 1);
				count = 1;
			}
			if (count == 1)
			{
				lua_setfield(state, -2, name);
			}
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
			for (typename FuncMapType::const_iterator it = function_map_.begin(); it != function_map_.end(); ++it)
			{
				registerFunction(state, it->first.c_str(), it->second);
			}
			for (typename MemberMapType::const_iterator it = member_map_.begin(); it != member_map_.end(); ++it)
			{
				registerField(state, it->first.c_str(), it->second);
			}
			for (typename CodeChunkMapType::const_iterator it = code_chunk_map_.begin(); it != code_chunk_map_.end(); ++it)
			{
				registerCodeChunk(state, it->first.c_str(), it->second);
			}
		}
		static bool is_metafield(const std::string& name)
		{
			return name.compare(0, 2, "__") == 0;
		}
		template<typename Fun>
		ClassMetatable& addFunction(const char* name, Fun f)
		{
			function_map_[name].push_back(FunctorType(f));
			return *this;
		}

		FuncMapType function_map_;
		MemberMapType member_map_;
		CodeChunkMapType code_chunk_map_;
		bool has_property_;
	};
};
