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

namespace kaguya
{
	template<class A1, class A2 = void
		, class A3 = void, class A4 = void, class A5 = void
		, class A6 = void, class A7 = void, class A8 = void,
		class A9 = void>struct MultipleBase {
	};
}

#include "kaguya/deprecated_metatable.hpp"

namespace kaguya
{
	struct LuaCodeChunk
	{
		LuaCodeChunk(const std::string& src) :code_(src) {}
		LuaCodeChunk(const char* src) :code_(src) {}
		std::string code_;
	};

	template<>	struct lua_type_traits<LuaCodeChunk> {
		static int push(lua_State* state, const LuaCodeChunk& ref)
		{
			int status = luaL_loadstring(state, ref.code_.c_str());
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			return 1;
		}
	};
	struct LuaCodeChunkResult : LuaCodeChunk {
		LuaCodeChunkResult(const std::string& src) :LuaCodeChunk(src) {}
		LuaCodeChunkResult(const char* src) :LuaCodeChunk(src) {}
	};
	template<>	struct lua_type_traits<LuaCodeChunkResult> {
		static int push(lua_State* state, const LuaCodeChunkResult& ref)
		{
			int status = luaL_loadstring(state, ref.code_.c_str());
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			status = lua_pcall_wrap(state, 0, 1);
			if (!except::checkErrorAndThrow(status, state)) { return 0; }
			return 1;
		}
	};

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
			if (class_userdata::newmetatable<class_type>(state))
			{
				LuaStackRef metatable(state, -1);
				registerMember(state);

				if (!traits::is_same<base_class_type, void>::value || !property_map_.empty())//if base class has property and derived class hasnt property. need property access metamethod
				{
					LuaFunction indexfun = kaguya::LuaFunction::loadstring(state, "local arg = {...};local metatable = arg[1];"
						"return function(table, index)"
						//						" if type(table) == 'userdata' then "
						" local propfun = metatable['_prop_'..index];"
						" if propfun then return propfun(table) end "
						//						" end "
						" return metatable[index]"
						" end")(metatable);

					metatable.setField("__index", indexfun);

					LuaFunction newindexfn = LuaFunction::loadstring(state, "local arg = {...};local metatable = arg[1];"
						" return function(table, index, value) "
						" if type(table) == 'userdata' then "
						" local propfun = metatable['_prop_'..index];"
						" if propfun then return propfun(table,value) end "
						" end "
						" rawset(table,index,value) "
						" end")(metatable);
					metatable.setField("__newindex", newindexfn);
				}
				else
				{
					metatable.setField("__index", metatable);
				}

				set_base_metatable(state, metatable, types::typetag<base_class_type>());

				return metatable;
			}
			else
			{
				except::OtherError(state, typeid(class_type*).name() + std::string(" is already registered"));
			}
			return LuaTable();
		}


#if KAGUYA_USE_CPP11
		template<typename... ArgTypes>
		UserdataMetatable& setConstructors()
		{
			addOverloadedFunctions("new", typename ConstructorFunction<class_type, ArgTypes>::type()...);
			return *this;
		}
#else
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>		
#define KAGUYA_SET_CON_TYPE_DEF(N) typename ConstructorFunction<class_type,KAGUYA_PP_CAT(A,N)>::type()
#define KAGUYA_SET_CON_FN_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
	inline UserdataMetatable& setConstructors()\
	{\
		addOverloadedFunctions("new",KAGUYA_PP_REPEAT_ARG(N,KAGUYA_SET_CON_TYPE_DEF));\
		return *this;\
	}
			KAGUYA_PP_REPEAT_DEF(9, KAGUYA_SET_CON_FN_DEF)
#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_SET_CON_FN_DEF
#undef KAGUYA_SET_CON_TYPE_DEF

#endif


		//add member property
		template<typename Ret>
		UserdataMetatable& addProperty(const char* name, Ret class_type::* mem)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(function(mem));
			return *this;
		}
		
		/**
		* @name addProperty
		* @brief add member property with getter function.(experimental)
		* @param name function name for lua
		* @param getter getter function
		*/
		template<typename GetType>
		UserdataMetatable& addProperty(const char* name, GetType(class_type::*getter)()const)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(function(getter));
			return *this;
		}

        /**
        * @name addProperty
        * @brief add member property with external getter function.(experimental)
        * @param name function name for lua
        * @param getter getter function
        */
        template<typename GetType>
        UserdataMetatable& addProperty(const char* name, GetType(*getter)(const class_type*))
        {
            if (has_key(name))
            {
                throw KaguyaException("already registered.");
                return *this;
            }
            property_map_[name] = AnyDataPusher(function(getter));
            return *this;
        }

		/**
		* @name addProperty
		* @brief add member property with setter, getter functions.(experimental)
		* @param name function name for lua
		* @param getter getter function
		* @param setter setter function
		*/
		template<typename GetType, typename SetType>
		UserdataMetatable& addProperty(const char* name, GetType(class_type::*getter)()const, void (class_type::*setter)(SetType))
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			property_map_[name] = AnyDataPusher(overload(getter, setter));
			return *this;
		}

        /**
        * @name addProperty
        * @brief add member property with external setter, getter functions.(experimental)
        * @param name function name for lua
        * @param getter getter function
        * @param setter setter function
        */
        template<typename GetType, typename SetType>
        UserdataMetatable& addProperty(const char* name, GetType(*getter)(const class_type*), void (*setter)(class_type*, SetType))
        {
            if (has_key(name))
            {
                throw KaguyaException("already registered.");
                return *this;
            }
            property_map_[name] = AnyDataPusher(overload(getter, setter));
            return *this;
        }

		/**
		* @name addStaticFunction
		* @brief add non member function
		* @param name function name for lua
		* @param f function
		*/
		template<typename Fun>
		UserdataMetatable& addStaticFunction(const char* name, Fun f)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(function(f));
			return *this;
		}

		//add field to 
		KAGUYA_DEPRECATED_FEATURE("addCodeChunkResult is deprecated. use addStaticField(kaguya::LuaCodeChunkResult(\"luacode\")).")
		UserdataMetatable& addCodeChunkResult(const char* name, const std::string& lua_code_chunk)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			
			member_map_[name] = AnyDataPusher(LuaCodeChunkResult(lua_code_chunk));
			return *this;
		}



		template<typename Data>
		UserdataMetatable& addStaticField(const char* name, const Data& d)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(d);
			return *this;
		}
#if KAGUYA_USE_CPP11
		template<typename... Funcs>
		UserdataMetatable& addOverloadedFunctions(const char* name, Funcs... f)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered."); 
				return *this;
			}

			member_map_[name] = AnyDataPusher(overload(f...));

			return *this;
		}
		template<typename Data>
		UserdataMetatable& addStaticField(const char* name, Data&& d)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered.");
				return *this;
			}
			member_map_[name] = AnyDataPusher(std::move(d));
			return *this;
		}
#else

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_FARG(N) const KAGUYA_PP_CAT(A,N)& KAGUYA_PP_CAT(a,N)
#define KAGUYA_PP_FUNCS(N) KAGUYA_PP_CAT(a,N)
#define KAGUYA_ADD_OVERLOAD_FUNCTION_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline UserdataMetatable& addOverloadedFunctions(const char* name,KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_FARG))\
		{\
			if (has_key(name))\
			{\
				throw KaguyaException("already registered.");\
				return *this;\
			}\
			member_map_[name] = AnyDataPusher(overload(KAGUYA_PP_ARG_REPEAT(N)));\
			return *this;\
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_ADD_OVERLOAD_FUNCTION_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_FARG
#undef KAGUYA_PP_FUNCS
#undef KAGUYA_ADD_OVERLOAD_FUNCTION_DEF
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1800
		//can not write  Ret class_type::* f on MSC++2013
		template<typename Fun>
		UserdataMetatable& addFunction(const char* name, Fun f)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered. if you want function overload,use addOverloadedFunctions");
				return *this;
			}
			member_map_[name] = AnyDataPusher(function(f));
			return *this;
		}
#else
		template<typename Ret>
		UserdataMetatable& addFunction(const char* name, Ret class_type::* f)
		{
			if (has_key(name))
			{
				throw KaguyaException("already registered. if you want function overload,use addOverloadedFunctions");
				return *this;
			}
			member_map_[name] = AnyDataPusher(function(f));
			return *this;
		}
#endif
		

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
			return addFunction(name,f);
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

			LuaFunction indexfun = kaguya::LuaFunction::loadstring(state, "local arg = {...};local metabases = arg[1];"
				"return function(t, k)"
				" for i = 1,#metabases do "
				" local v = metabases[i][k] "
				" if v then "
				" t[k] = v "
				" return v end "
				" end"
				" end")(metabases);

			newmeta.setField("__index", indexfun);

			metatable.setMetatable(newmeta);

		}
#if KAGUYA_USE_CPP11

		template<typename Base>
		void metatables(lua_State* state, LuaStackRef& metabases, PointerConverter& pvtreg, types::typetag<MultipleBase<Base> >)const
		{
			class_userdata::get_metatable<Base>(state);
			metabases.setField(metabases.size() + 1, LuaTable(state, StackTop()));
			pvtreg.add_type_conversion<Base, class_type>();
		}
		template<typename Base, typename... Remain>
		void metatables(lua_State* state, LuaStackRef& metabases, PointerConverter& pvtreg, types::typetag<MultipleBase<Base, Remain...> >)const
		{
			class_userdata::get_metatable<Base>(state);
			metabases.setField(metabases.size() + 1, LuaTable(state, StackTop()));
			pvtreg.add_type_conversion<Base, class_type>();
			metatables(state, metabases, pvtreg, types::typetag<MultipleBase<Remain...> >());
		}

		template<typename... Bases>
		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<MultipleBase<Bases...> > metatypes)const
		{
			PointerConverter& pconverter = PointerConverter::get(state);

			util::one_push(state, NewTable(sizeof...(Bases),0));
			LuaStackRef metabases(state,-1, true);
			metatables(state, metabases, pconverter, metatypes);
			set_multiple_base(state, metatable, metabases);
		}

#else
#define KAGUYA_TEMPLATE_PARAMETER(N) template<KAGUYA_PP_TEMPLATE_DEF_REPEAT(N)>
#define KAGUYA_GET_BASE_METATABLE(N) class_userdata::get_metatable<KAGUYA_PP_CAT(A,N)>(state);\
		metabases.setField(metabases.size() + 1, LuaTable(state, StackTop())); \
		pconverter.add_type_conversion<KAGUYA_PP_CAT(A,N),class_type>();
#define KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF(N) \
	KAGUYA_TEMPLATE_PARAMETER(N)\
		void set_base_metatable(lua_State* state, LuaStackRef& metatable, types::typetag<MultipleBase<KAGUYA_PP_TEMPLATE_ARG_REPEAT(N)> > metatypes)const\
		{\
			PointerConverter& pconverter = PointerConverter::get(state);\
			util::one_push(state, NewTable(N,0));\
			LuaStackRef metabases(state,-1, true);\
			KAGUYA_PP_REPEAT(N, KAGUYA_GET_BASE_METATABLE); \
			set_multiple_base(state, metatable, metabases);\
		}\

		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF)
#undef KAGUYA_TEMPLATE_PARAMETER
#undef KAGUYA_MULTIPLE_INHERITANCE_SETBASE_DEF
#undef KAGUYA_GET_BASE_METATABLE
#undef KAGUYA_TYPE_CHECK_REP
#endif

		bool has_key(const std::string& key)
		{
			if (member_map_.count(key) > 0 )
			{
				return true;
			}
			if (property_map_.count(key) > 0 )
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
				registerField(state, ("_prop_" + it->first).c_str(), it->second);
			}
		}

		PropMapType property_map_;
		MemberMapType member_map_;
	};
}
