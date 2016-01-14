#pragma once

#include <string>
#include <cstring>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"
#include "kaguya/exception.hpp"

namespace kaguya
{
#define KAGUYA_METATABLE_PREFIX "kaguya_object_type_"
	template<typename T>
	inline const std::string& metatableName()
	{
		typedef typename traits::remove_cv<T>::type noncv_type;
		typedef typename traits::remove_pointer<noncv_type>::type noncvpointer_type;
		typedef typename traits::remove_const_reference<noncvpointer_type>::type noncvpointerref_type;

		static const std::string v = std::string(KAGUYA_METATABLE_PREFIX) + typeid(noncvpointerref_type*).name();

		return v;
	}

	namespace class_userdata
	{
		template<typename T>bool get_metatable(lua_State* l)
		{
			luaL_getmetatable(l, metatableName<T>().c_str());
			return LUA_TNIL != lua_type(l, -1);
		}
		template<typename T>bool available_metatable(lua_State* l)
		{
			util::ScopedSavedStack save(l);
			return get_metatable<T>(l);
		}
		template<typename T>bool newmetatable(lua_State* l)
		{
			if (luaL_newmetatable(l, metatableName<T>().c_str()))
			{
#if LUA_VERSION_NUM < 503
				lua_pushstring(l, metatableName<T>().c_str());
				lua_setfield(l, -2, "__name");
#endif
				return true;
			}
			return false;
		}
		template<typename T>void setmetatable(lua_State* l)
		{
			return luaL_setmetatable(l, metatableName<T>().c_str());
		}

		template<typename T>bool is_userdata(lua_State* l, int index)
		{
			return luaL_testudata(l, index, metatableName<T>().c_str()) != 0;
		}
		template<typename T>T* test_userdata(lua_State* l, int index)
		{
			return static_cast<T*>(luaL_testudata(l, index, metatableName<T>().c_str()));
		}
	}
	inline bool available_metatable(lua_State* l, const char* t)
	{
		util::ScopedSavedStack save(l);
		luaL_getmetatable(l, t);
		return LUA_TNIL != lua_type(l, -1);
	}
	template<typename T>
	bool available_metatable(lua_State* l, types::typetag<T> type = types::typetag<T>())
	{
		return class_userdata::available_metatable<T>(l);
	}


	struct ObjectWrapperBase
	{
		template<typename T>
		bool is_metatable_object() { return is_metatable_object(metatableName<T>()); }
		virtual bool is_metatable_object(const std::string& metaname) = 0;

		virtual const void* cget() = 0;
		virtual void* get() = 0;

		virtual ~ObjectWrapperBase() {}
	};

	template<class T>
	struct ObjectWrapper : ObjectWrapperBase
	{
		T object;

		ObjectWrapper() :object() {}

		template<class Arg1>
		ObjectWrapper(const Arg1& v1) : object(v1) {}
		template<class Arg1, class Arg2>
		ObjectWrapper(const Arg1& v1, const Arg2& v2) : object(v1, v2) {}
		template<class Arg1, class Arg2, class Arg3>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3) : object(v1, v2, v3) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4) : object(v1, v2, v3, v4) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5) : object(v1, v2, v3, v4, v5) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5, const Arg6& v6) : object(v1, v2, v3, v4, v5, v6) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5, const Arg6& v6, const Arg7& v7) : object(v1, v2, v3, v4, v5, v6, v7) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5, const Arg6& v6, const Arg7& v7, const Arg8& v8) : object(v1, v2, v3, v4, v5, v6, v7, v8) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5, const Arg6& v6, const Arg7& v7, const Arg8& v8, const Arg9& v9) : object(v1, v2, v3, v4, v5, v6, v7, v8, v9) {}
#if KAGUYA_USE_RVALUE_REFERENCE
		template<class... Args>
		ObjectWrapper(Args&&... args) : object(standard::forward<Args>(args)...) {}
#endif

		bool is_metatable_object(const std::string& metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get()
		{
			return &object;
		}
		virtual const void* cget()
		{
			return &object;
		}
	};

	template<class T>
	struct ObjectWrapper<standard::shared_ptr<T> > : ObjectWrapperBase
	{
		standard::shared_ptr<T> object;

		ObjectWrapper(const standard::shared_ptr<T>& v1) :object(v1) {}

		bool is_metatable_object(const std::string& metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get()
		{
			return object.get();
		}
		virtual const void* cget()
		{
			return object.get();
		}
	};

	template<class T>
	struct ObjectPointerWrapper : ObjectWrapperBase
	{
		T* object;

		ObjectPointerWrapper(T* ptr) :object(ptr) {}

		bool is_metatable_object(const std::string& metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get()
		{
			if (traits::is_const<T>::value)
			{
				return 0;
			}
			return const_cast<void*>(static_cast<const void*>(object));
		}
		virtual const void* cget()
		{
			return object;
		}
	};

	inline bool recursive_base_type_check(lua_State* l, int index, const std::string& require_type)
	{
		if (lua_getmetatable(l, index))
		{
			lua_getfield(l, -1, "__name");
			const char* metatable_name = lua_tostring(l, -1);
			if (metatable_name)
			{
				if (require_type == metatable_name)
				{
					return true;
				}
				return recursive_base_type_check(l,-2, require_type);
			}
		}
		return false;	
	}

	inline ObjectWrapperBase* object_wrapper(lua_State* l, int index,const std::string& require_type= std::string())
	{
		util::ScopedSavedStack save(l);
		void* ptr = lua_touserdata(l, index);
		if (ptr && lua_type(l, index) == LUA_TUSERDATA)
		{
			if (require_type.empty() || recursive_base_type_check(l,index, require_type))
			{
				return static_cast<ObjectWrapperBase*>(ptr);
			}
		}
		return 0;
	}
	template<class T>
	T* get_pointer(lua_State* l, int index, types::typetag<T> tag)
	{
		int type = lua_type(l, index);

		if (type == LUA_TLIGHTUSERDATA)
		{
			return (T*)lua_topointer(l, index);
		}
		else if (type != LUA_TUSERDATA)
		{
			return 0;
		}
		else
		{
			ObjectWrapperBase* objwrapper = object_wrapper(l, index,metatableName<T>());
			if (objwrapper)
			{
				return static_cast<T*>(objwrapper->get());
			}
		}
		return 0;
	}
	template<class T>
	const T* get_const_pointer(lua_State* l, int index, types::typetag<T> tag)
	{
		int type = lua_type(l, index);

		if (type == LUA_TLIGHTUSERDATA)
		{
			return (T*)lua_topointer(l, index);
		}
		else if (type != LUA_TUSERDATA)
		{
			return 0;
		}
		else
		{
			ObjectWrapperBase* objwrapper = object_wrapper(l, index, metatableName<T>());
			if (objwrapper)
			{
				return static_cast<const T*>(objwrapper->cget());
			}
		}
		return 0;
	}
};
