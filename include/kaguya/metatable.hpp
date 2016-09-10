// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include "kaguya/config.hpp"
#include "kaguya/push_any.hpp"
#include "kaguya/native_function.hpp"


#include "kaguya/lua_ref_function.hpp"

#define KAGUYA_PROPERTY_PREFIX "_prop_"

namespace kaguya
{

#define KAGUYA_PP_STRUCT_TDEF_REP(N) KAGUYA_PP_CAT(class A,N) = void
#define KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT(N) KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_STRUCT_TDEF_REP)

	template<KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT(KAGUYA_CLASS_MAX_BASE_CLASSES)>
	struct MultipleBase {
	};
#undef KAGUYA_PP_STRUCT_TDEF_REP
#undef KAGUYA_PP_STRUCT_TEMPLATE_DEF_REPEAT
}

#include "kaguya/deprecated_metatable.hpp"

namespace kaguya
{
	struct LuaCodeChunk
	{
		LuaCodeChunk(const std::string& src, const std::string& name = "") :code_(src), chunk_name_(name) {}
		LuaCodeChunk(const char* src, const char* name = "") :code_(src), chunk_name_(name ? name : "") {}
		std::string code_;
		std::string chunk_name_;
	};

	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for LuaCodeChunk
	template<>	struct lua_type_traits<LuaCodeChunk> {
		static int push(lua_State* state, const LuaCodeChunk& ref)
		{
			int status = luaL_loadbuffer(state, ref.code_.c_str(), ref.code_.size(), ref.chunk_name_.empty() ? ref.code_.c_str() : ref.chunk_name_.c_str());
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			return 1;
		}
	};
	struct LuaCodeChunkExecute : LuaCodeChunk {
		LuaCodeChunkExecute(const std::string& src, const std::string& name = "") :LuaCodeChunk(src, name) {}
		LuaCodeChunkExecute(const char* src, const char* name = "") :LuaCodeChunk(src, name) {}
	};
	typedef LuaCodeChunkExecute LuaCodeChunkResult;
	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for LuaCodeChunkResult
	template<>	struct lua_type_traits<LuaCodeChunkExecute> {
		static int push(lua_State* state, const LuaCodeChunkExecute& ref)
		{
			int status = luaL_loadbuffer(state, ref.code_.c_str(), ref.code_.size(), ref.chunk_name_.empty()? ref.code_.c_str(): ref.chunk_name_.c_str());
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			status = lua_pcall_wrap(state, 0, 1);
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			return 1;
		}
	};

	namespace detail
	{
		inline bool is_property_key(const char* keyname)
		{
			return keyname && strncmp(keyname, KAGUYA_PROPERTY_PREFIX, sizeof(KAGUYA_PROPERTY_PREFIX) - 1) != 0;
		}
		inline int property_index_function(lua_State* L)
		{
			//Lua
			//local arg = {...};local metatable = arg[1];
			//return function(table, index)
			//if string.find(index,KAGUYA_PROPERTY_PREFIX)~=0 then 
			//local propfun = metatable[KAGUYA_PROPERTY_PREFIX ..index];
			//if propfun then return propfun(table) end 
			//end 
			//return metatable[index]
			//end
			static const int table = 1;
			static const int key = 2;
			static const int metatable = lua_upvalueindex(1);
			const char* strkey = lua_tostring(L, key);

			if (lua_type(L, 1) == LUA_TUSERDATA && is_property_key(strkey))
			{
				int type = lua_getfield_rtype(L, metatable, (KAGUYA_PROPERTY_PREFIX + std::string(strkey)).c_str());
				if (type == LUA_TFUNCTION)
				{
					lua_pushvalue(L, table);
					lua_call(L, 1, 1);
					return 1;
				}
			}
			lua_pushvalue(L, key);
			lua_gettable(L, metatable);
			return 1;
		}
		inline int property_newindex_function(lua_State* L)
		{
			//Lua
			//local arg = {...};local metatable = arg[1];
			// return function(table, index, value) 
			// if type(table) == 'userdata' then 
			// if string.find(index,KAGUYA_PROPERTY_PREFIX)~=0 then 
			// local propfun = metatable[KAGUYA_PROPERTY_PREFIX..index];
			// if propfun then return propfun(table,value) end 
			// end 
			// end 
			// rawset(table,index,value) 
			// end
			static const int table = 1;
			static const int key = 2;
			static const int value = 3;
			static const int metatable = lua_upvalueindex(1);
			const char* strkey = lua_tostring(L, 2);

			if (lua_type(L, 1) == LUA_TUSERDATA && is_property_key(strkey))
			{
				int type = lua_getfield_rtype(L, metatable, (KAGUYA_PROPERTY_PREFIX + std::string(strkey)).c_str());
				if (type == LUA_TFUNCTION)
				{
					lua_pushvalue(L, table);
					lua_pushvalue(L, value);
					lua_call(L, 2, 0);
					return 0;
				}
			}
			lua_pushvalue(L, key);
			lua_pushvalue(L, value);
			lua_rawset(L, table);
			return 0;
		}


		inline int multiple_base_index_function(lua_State* L)
		{
			//Lua
			//local arg = {...};local metabases = arg[1];
			//return function(t, k)
			// for i = 1,#metabases do 
			// local v = metabases[i][k] 
			// if v then 
			// t[k] = v 
			// return v end 
			// end
			//end
			static const int table = 1;
			static const int key = 2;
			static const int metabases = lua_upvalueindex(1);

			lua_pushnil(L);
			while (lua_next(L, metabases) != 0)
			{
				if (lua_type(L, -1) == LUA_TTABLE)
				{
					lua_pushvalue(L, key);
					int type = lua_gettable_rtype(L, -2);
					if (type != LUA_TNIL)
					{
						lua_pushvalue(L, key);
						lua_pushvalue(L, -2);
						lua_settable(L, table);
						return 1;
					}
				}
				lua_settop(L, 3);//pop value
			}
			return 0;
		}
	}


	/// class binding interface.
	template<typename class_type, typename base_class_type = void>
	class UserdataMetatable
	{
		typedef std::map<std::string, AnyDataPusher> PropMapType;
		typedef std::map<std::string, AnyDataPusher> MemberMapType;
	public:

		UserdataMetatable()
		{
			addStaticFunction("__gc", &class_userdata::destructor<ObjectWrapperBase>);

			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value || !traits::is_std_vector<class_type>::value, "std::vector is binding to lua-table by default.If you wants register for std::vector yourself,"
				"please define KAGUYA_NO_STD_VECTOR_TO_TABLE");

			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value || !traits::is_std_map<class_type>::value, "std::map is binding to lua-table by default.If you wants register for std::map yourself,"
				"please define KAGUYA_NO_STD_MAP_TO_TABLE");

			//can not register push specialized class
			KAGUYA_STATIC_ASSERT(is_registerable<class_type>::value,
				"Can not register specialized of type conversion class. e.g. std::tuple");
		}

		LuaTable createMatatable(lua_State* state)const
		{
			util::ScopedSavedStack save(state);
			if (!class_userdata::newmetatable<class_type>(state))
			{
				except::OtherError(state, typeid(class_type*).name() + std::string(" is already registered"));
				return LuaTable();
			}
			LuaStackRef metatable(state, -1);
			registerMember(state);

			if (!traits::is_same<base_class_type, void>::value || !property_map_.empty())//if base class has property and derived class hasnt property. need property access metamethod
			{

				if (member_map_.count("__index") == 0)
				{
					metatable.push(state);
					lua_pushcclosure(state, &detail::property_index_function, 1);
					LuaStackRef indexfun(state, -1);
					metatable.setRawField("__index", indexfun);
				}



				if (member_map_.count("__newindex") == 0)
				{
					metatable.push(state);
					lua_pushcclosure(state, &detail::property_newindex_function, 1);
					LuaStackRef newindexfun(state, -1);
					metatable.setRawField("__newindex", newindexfun);
				}
			}
			else
			{
				if (member_map_.count("__index") == 0)
				{
					metatable.setRawField("__index", metatable);
				}
			}

			set_base_metatable(state, metatable, types::typetag<base_class_type>());

			class_userdata::get_call_constructor_metatable(state);
			LuaStackRef call_construct_table(state, -1, true);
			LuaTable basemetatable = metatable.getMetatable();
			if (basemetatable)
			{
				basemetatable.setRawField("__call", call_construct_table.getRawField("__call"));
			}
			else
			{
				metatable.setMetatable(call_construct_table);
			}

			return metatable;
		}


#if KAGUYA_USE_CPP11
		template<typename... ArgTypes>
		UserdataMetatable& setConstructors()
		{
			addOverloadedFunctions("new", typename ConstructorFunction<class_type, ArgTypes>::type()...);
			return *this;
		}
#else	
#define KAGUYA_SET_CON_TYPE_DEF(N) typename ConstructorFunction<class_type,KAGUYA_PP_CAT(A,N)>::type()
#define KAGUYA_SET_CON_FN_DEF(N) \
		template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
		inline UserdataMetatable& setConstructors()\
		{\
			addOverloadedFunctions("new",KAGUYA_PP_REPEAT_ARG(N,KAGUYA_SET_CON_TYPE_DEF));\
			return *this;\
		}

		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_OVERLOADS, KAGUYA_SET_CON_FN_DEF)
#undef KAGUYA_SET_CON_FN_DEF
#undef KAGUYA_SET_CON_TYPE_DEF

#endif


		/// @brief add member property with getter function.(experimental)
		/// @param name function name for lua
		/// @param mem bind member data
		template<typename Ret>
		UserdataMetatable& addProperty(const char* name, Ret class_type::* mem)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(kaguya::function(mem));
			return *this;
		}

		/// @brief add member property with getter function.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		template<typename GetType>
		UserdataMetatable& addProperty(const char* name, GetType(class_type::*getter)()const)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(kaguya::function(getter));
			return *this;
		}

		/// @brief add member property with setter, getter functions.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		/// @param setter setter function
        template<typename GetType>
        UserdataMetatable& addProperty(const char* name, GetType(*getter)(const class_type*))
        {
            if (has_key(name))
            {
                throw KaguyaException(std::string(name) + " is already registered.");
                return *this;
            }
            property_map_[name] = AnyDataPusher(function(getter));
            return *this;
        }

		/// @brief add member property with setter, getter functions.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		/// @param setter setter function
		template<typename GetType, typename SetType>
		UserdataMetatable& addProperty(const char* name, GetType(class_type::*getter)()const, void (class_type::*setter)(SetType))
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(overload(getter, setter));
			return *this;
		}


		/// @brief add member property with external setter, getter functions.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		/// @param setter setter function
        template<typename GetType, typename SetType>
        UserdataMetatable& addProperty(const char* name, GetType(*getter)(const class_type*), void (*setter)(class_type*, SetType))
        {
            if (has_key(name))
            {
                throw KaguyaException(std::string(name) + " is already registered.");
                return *this;
            }
            property_map_[name] = AnyDataPusher(overload(getter, setter));
            return *this;
        }

		/// @brief add member property with getter function.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		template<typename GetterType>
		UserdataMetatable& addPropertyAny(const char* name, GetterType getter)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(function(getter));
			return *this;
		}
		/// @brief add member property with setter, getter functions.(experimental)
		/// @param name function name for lua
		/// @param getter getter function
		/// @param setter setter function
		template<typename GetterType, typename SetterType>
		UserdataMetatable& addPropertyAny(const char* name, GetterType getter, SetterType setter)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(overload(getter, setter));
			return *this;
		}

		/// @brief add non member function
		/// @param name function name for lua
		/// @param f function
		template<typename Fun>
		UserdataMetatable& addStaticFunction(const char* name, Fun f)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(kaguya::function(f));
			return *this;
		}

		/// @brief assign data by return value from evaluate code chunk. 
		/// this function is deprecated.  use addStaticField(kaguya::LuaCodeChunkResult(\"luacode\")).
		KAGUYA_DEPRECATED_FEATURE("addCodeChunkResult is deprecated. use addStaticField(kaguya::LuaCodeChunkResult(\"luacode\")).")
		UserdataMetatable& addCodeChunkResult(const char* name, const std::string& lua_code_chunk)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}

			member_map_[name] = AnyDataPusher(LuaCodeChunkResult(lua_code_chunk));
			return *this;
		}

		/// @brief assign data by argument value. 
		/// @param name name for lua
		/// @param d data
		template<typename Data>
		UserdataMetatable& addStaticField(const char* name, const Data& d)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(d);
			return *this;
		}
#if KAGUYA_USE_CPP11
		/// @brief assign overloaded from functions. 
		/// @param name name for lua
		/// @param f functions
		template<typename... Funcs>
		UserdataMetatable& addOverloadedFunctions(const char* name, Funcs... f)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}

			member_map_[name] = AnyDataPusher(overload(f...));

			return *this;
		}

		/// @brief assign data by argument value. 
		/// @param name name for lua
		/// @param d data
		template<typename Data>
		UserdataMetatable& addStaticField(const char* name, Data&& d)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(std::move(d));
			return *this;
		}
#else

#define KAGUYA_ADD_OVERLOAD_FUNCTION_DEF(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
		inline UserdataMetatable& addOverloadedFunctions(const char* name,KAGUYA_PP_ARG_CR_DEF_REPEAT(N))\
		{\
			if (has_key(name))\
			{\
				throw KaguyaException(std::string(name) + " is already registered.");\
				return *this;\
			}\
			member_map_[name] = AnyDataPusher(kaguya::overload(KAGUYA_PP_ARG_REPEAT(N)));\
			return *this;\
		}

		KAGUYA_PP_REPEAT_DEF(KAGUYA_FUNCTION_MAX_OVERLOADS, KAGUYA_ADD_OVERLOAD_FUNCTION_DEF)
#undef KAGUYA_ADD_OVERLOAD_FUNCTION_DEF
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1800
		//can not write  Ret class_type::* f on MSC++2013
		template<typename Fun>
		UserdataMetatable& addFunction(const char* name, Fun f)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered. To overload a function, use addOverloadedFunctions");
				return *this;
			}
			member_map_[name] = AnyDataPusher(kaguya::function(f));
			return *this;
		}
#else
		/// @brief assign function
		/// @param name name for lua
		/// @param f pointer to member function.
		template<typename Ret>
		UserdataMetatable& addFunction(const char* name, Ret class_type::* f)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered. To overload a function, use addOverloadedFunctions");
				return *this;
			}
			member_map_[name] = AnyDataPusher(kaguya::function(f));
			return *this;
		}
#endif
		/// @brief assign function
		/// @param name name for lua
		/// @param f member function object.
		UserdataMetatable& addFunction(const char* name, PolymorphicMemberInvoker f)
		{
			if (has_key(name))
			{
				throw KaguyaException(std::string(name) + " is already registered. To overload a function, use addOverloadedFunctions");
				return *this;
			}
			member_map_[name] = AnyDataPusher(kaguya::function(f));
			return *this;
		}


#if defined(_MSC_VER) && _MSC_VER <= 1800
		//can not use add at MSVC2013
#else
		template<typename Data>
		KAGUYA_DEPRECATED_FEATURE("add is deprecated. use addStaticField instead.")
			UserdataMetatable& add(const char* name, const Data& d)
		{
			return addStaticField(name, d);
		}

#if KAGUYA_USE_CPP11
		template<typename Data>
		KAGUYA_DEPRECATED_FEATURE("add is deprecated. use addStaticField instead.")
			UserdataMetatable& add(const char* name, Data&& d)
		{
			return addStaticField(name, d);
		}
#endif
		template<typename Ret>
		KAGUYA_DEPRECATED_FEATURE("add is deprecated. use addFunction instead.")
		UserdataMetatable& add(const char* name, Ret class_type::* f)
		{
			return addFunction(name, f);
		}
#endif

	private:

		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<void>)const
		{
		}
		template<class Base>
		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<Base>)const
		{
			class_userdata::get_metatable<Base>(state);
			metatable.setMetatable(LuaTable(state, StackTop()));

			PointerConverter& pconverter = PointerConverter::get(state);
			pconverter.add_type_conversion<Base, class_type>();
		}

		void set_multiple_base(lua_State* state, LuaStackRef& metatable, const LuaTable& metabases)const
		{
			LuaTable newmeta(state, NewTable());

			metabases.push(state);
			lua_pushcclosure(state, &detail::multiple_base_index_function, 1);
			LuaStackRef indexfun(state, -1);
			newmeta.setField("__index", indexfun);

			metatable.setMetatable(newmeta);

		}
#if KAGUYA_USE_CPP11

		template<typename Base>
		void metatables(lua_State* state, LuaStackRef& metabases, PointerConverter& pvtreg, types::typetag<MultipleBase<Base> >)const
		{
			class_userdata::get_metatable<Base>(state);
			metabases.setRawField(metabases.size() + 1, LuaTable(state, StackTop()));
			pvtreg.add_type_conversion<Base, class_type>();
		}
		template<typename Base, typename... Remain>
		void metatables(lua_State* state, LuaStackRef& metabases, PointerConverter& pvtreg, types::typetag<MultipleBase<Base, Remain...> >)const
		{
			class_userdata::get_metatable<Base>(state);
			metabases.setRawField(metabases.size() + 1, LuaTable(state, StackTop()));
			pvtreg.add_type_conversion<Base, class_type>();
			metatables(state, metabases, pvtreg, types::typetag<MultipleBase<Remain...> >());
		}

		template<typename... Bases>
		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<MultipleBase<Bases...> > metatypes)const
		{
			PointerConverter& pconverter = PointerConverter::get(state);

			util::one_push(state, NewTable(sizeof...(Bases), 0));
			LuaStackRef metabases(state, -1, true);
			metatables(state, metabases, pconverter, metatypes);
			set_multiple_base(state, metatable, metabases);
		}

#else
#define KAGUYA_GET_BASE_METATABLE(N) class_userdata::get_metatable<KAGUYA_PP_CAT(A,N)>(state);\
		metabases.setRawField(metabases.size() + 1, LuaTable(state, StackTop())); \
		pconverter.add_type_conversion<KAGUYA_PP_CAT(A,N),class_type>();
#define KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF(N) \
		template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>\
		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<MultipleBase<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)> > metatypes)const\
		{\
			PointerConverter& pconverter = PointerConverter::get(state);\
			util::one_push(state, NewTable(N,0));\
			LuaStackRef metabases(state,-1, true);\
			KAGUYA_PP_REPEAT(N, KAGUYA_GET_BASE_METATABLE); \
			set_multiple_base(state, metatable, metabases);\
		}\

		KAGUYA_PP_REPEAT_DEF(KAGUYA_CLASS_MAX_BASE_CLASSES, KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF)
#undef KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF
#undef KAGUYA_GET_BASE_METATABLE
#endif

		bool has_key(const std::string& key)
		{
			if (member_map_.count(key) > 0)
			{
				return true;
			}
			if (property_map_.count(key) > 0)
			{
				return true;
			}
			std::string propkey = KAGUYA_PROPERTY_PREFIX + key;
			if (member_map_.count(propkey) > 0)//_prop_keyname is reserved for property
			{
				return true;
			}
			return false;
		}
		void registerField(lua_State* state, const char* name, const AnyDataPusher& value)const
		{
			int count = value.pushToLua(state);
			if (count > 1)
			{
				lua_pop(state, count - 1);
				count = 1;
			}
			if (count == 1)
			{
				lua_setfield(state, -2, name);
			}
		}
		void registerMember(lua_State* state)const
		{
			for (typename MemberMapType::const_iterator it = member_map_.begin(); it != member_map_.end(); ++it)
			{
				registerField(state, it->first.c_str(), it->second);
			}
			for (typename PropMapType::const_iterator it = property_map_.begin(); it != property_map_.end(); ++it)
			{
				registerField(state, (KAGUYA_PROPERTY_PREFIX + it->first).c_str(), it->second);
			}
		}

		PropMapType property_map_;
		MemberMapType member_map_;
	};

	/// @ingroup lua_type_traits
	/// @brief lua_type_traits for UserdataMetatable
	template<typename T,typename Base>	struct lua_type_traits<UserdataMetatable<T,Base> > {
		typedef const UserdataMetatable<T, Base>& push_type;

		static int push(lua_State* l, push_type ref)
		{
			return ref.createMatatable(l).push(l);
		}
	};
}
