#pragma once

#include <string>
#include <cstring>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"
#include "kaguya/exception.hpp"

namespace kaguya
{
	namespace nodirectuse {
		typedef const std::string& metatableName_t;
		template<typename T>
		inline metatableName_t metatableNameNonCV(types::typetag<T>)
		{
			static const std::string v = std::string("kaguya_object_type_") + typeid(T*).name();
			return v;
		}
	}
	template<typename T>
	inline nodirectuse::metatableName_t metatableName()
	{
		typedef typename traits::remove_cv<T>::type noncv_type;
		typedef typename traits::remove_pointer<noncv_type>::type noncvpointer_type;
		typedef typename traits::remove_const_reference<noncvpointer_type>::type noncvpointerref_type;
		return nodirectuse::metatableNameNonCV(types::typetag<noncvpointerref_type>());
	}

	inline bool available_metatable(lua_State* l, const char* t)
	{
		util::ScopedSavedStack save(l);
		luaL_getmetatable(l, t);
		return LUA_TNIL != lua_type(l, -1);
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
	template<typename T>
	bool available_metatable(lua_State* l, types::typetag<T> type = types::typetag<T>())
	{
		return class_userdata::available_metatable<T>(l);
	}


	struct ObjectWrapperBase
	{
		template<class T> bool is_metatable_object()
		{
			return is_metatable_object(metatableName<T>().c_str());
		}
		virtual bool is_metatable_object(const char* metaname) = 0;

		virtual const void* get_const_pointer(const char* metaname) = 0;
		virtual void* get_pointer(const char* metaname) = 0;

		void* get_pointer(const std::string& metaname) { return get_pointer(metaname.c_str()); }
		virtual ~ObjectWrapperBase() {}
	};

	template<class T>
	struct ObjectWrapper : ObjectWrapperBase
	{
		T object;

		ObjectWrapper() :object() {}

		template<class Arg1>
		ObjectWrapper(const Arg1& v1) :object(v1) {}
		template<class Arg1, class Arg2>
		ObjectWrapper(const Arg1& v1, const Arg2& v2) : object(v1, v2) {}
		template<class Arg1, class Arg2, class Arg3>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3) : object(v1, v2, v3) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4) : object(v1, v2, v3, v4) {}
		template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		ObjectWrapper(const Arg1& v1, const Arg2& v2, const Arg3& v3, const Arg4& v4, const Arg5& v5) : object(v1, v2, v3, v4, v5) {}
#if KAGUYA_USE_RVALUE_REFERENCE
		template<class... Args>
		ObjectWrapper(Args&&... args) : object(standard::forward<Args>(args)...) {}
#endif

		bool is_metatable_object(const char* metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return &object;
			}
			return 0;
		}
		virtual const void* get_const_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return &object;
			}
			return 0;
		}
	};

	template<class T>
	struct ObjectWrapper<standard::shared_ptr<T> > : ObjectWrapperBase
	{
		standard::shared_ptr<T> object;

		ObjectWrapper(const standard::shared_ptr<T>& v1) :object(v1) {}
#if KAGUYA_USE_RVALUE_REFERENCE
		template<class... Args>
		ObjectWrapper(Args&&... args) : object(standard::forward<Args>(args)...) {}
#endif

		bool is_metatable_object(const char* metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return object.get();
			}
			return 0;
		}
		virtual const void* get_const_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return object.get();
			}
			return 0;
		}
	};

	template<class T>
	struct ObjectPointerWrapper : ObjectWrapperBase
	{
		T* object;

		ObjectPointerWrapper(T* ptr) :object(ptr) {}

		bool is_metatable_object(const char* metaname)
		{
			return metatableName<T>() == metaname;
		}
		virtual void* get_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return object;
			}
			return 0;
		}
		virtual const void* get_const_pointer(const char* metaname)
		{
			if (metatableName<T>() == metaname)
			{
				return object;
			}
			return 0;
		}
	};

	inline ObjectWrapperBase* object_wrapper(lua_State* l, int index)
	{
		index = lua_absindex(l, index);
		int type = lua_type(l, index);
		if (type == LUA_TUSERDATA)
		{
			if (lua_getmetatable(l, index))
			{
				void* ptr = 0;
				lua_getfield(l, -1, "__name");

				const char* metatable = lua_tostring(l, -1);
				if (metatable)
				{
					if (memcmp(metatable, "kaguya_object_type", strlen("kaguya_object_type")) == 0)
					{
						ptr = lua_touserdata(l, index);
					}
				}
				lua_pop(l, 2);
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
			ObjectWrapperBase* objwrapper = object_wrapper(l, index);
			if (objwrapper)
			{
				return static_cast<T*>(objwrapper->get_pointer(metatableName<T>()));
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
			ObjectWrapperBase* objwrapper = object_wrapper(l, index);
			if (objwrapper)
			{
				return static_cast<const T*>(objwrapper->get_const_pointer(metatableName<T>()));
			}
		}
		return 0;
	}
};
