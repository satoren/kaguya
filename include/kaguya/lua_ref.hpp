// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <ostream>
#include <istream>
#include "kaguya/config.hpp"
#include "kaguya/error_handler.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"

#include "kaguya/detail/lua_ref_impl.hpp"
#include "kaguya/detail/lua_variant_def.hpp"

namespace kaguya
{

	class LuaUserData;
	class LuaTable;
	class LuaFunction;
	class LuaThread;
	template<typename KEY>
	class TableKeyReference;
	class FunctionResults;
	class MemberFunctionBinder;

	class LuaRef;
	class LuaStackRef;



	namespace util
	{
		template<class Result>
		inline Result get_result_impl(lua_State* l, int startindex, types::typetag<Result>)
		{
			return lua_type_traits<Result>::get(l, startindex);
		}
#if KAGUYA_USE_CPP11
		inline standard::tuple<> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}
		template<typename T, typename... TYPES>
		inline standard::tuple<T, TYPES...> get_result_tuple_impl(lua_State* l, int index, types::typetag<standard::tuple<T, TYPES...> > tag)
		{
			return standard::tuple_cat(standard::tuple<T>(lua_type_traits<T>::get(l, index)),
				get_result_tuple_impl(l, index + 1, types::typetag<standard::tuple<TYPES...> >()));
		}
		template<typename... TYPES>
		inline standard::tuple<TYPES...> get_result_impl(lua_State* l, int startindex, types::typetag<standard::tuple<TYPES...> > tag)
		{
			return get_result_tuple_impl<TYPES...>(l, startindex, tag);
		}
#else

		inline standard::tuple<> get_result_impl(lua_State *l, int startindex, types::typetag<standard::tuple<> > tag)
		{
			return standard::tuple<>();
		}

#define KAGUYA_PP_TEMPLATE(N) KAGUYA_PP_CAT(typename A,N)
#define KAGUYA_PP_TARG(N) KAGUYA_PP_CAT(A,N)
#define KAGUYA_GET_DEF(N) lua_type_traits<KAGUYA_PP_CAT(A,N)>::get(l, N + startindex - 1) 
#define KAGUYA_GET_TUPLE_DEF(N) template<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TEMPLATE)>\
		inline standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> get_result_impl(lua_State *l,int startindex,types::typetag<standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG)> >)\
		{\
			return standard::tuple<KAGUYA_PP_REPEAT_ARG(N,KAGUYA_PP_TARG) >(KAGUYA_PP_REPEAT_ARG(N,KAGUYA_GET_DEF));\
		}
		KAGUYA_PP_REPEAT_DEF(9, KAGUYA_GET_TUPLE_DEF)
#undef KAGUYA_PP_TEMPLATE
#undef KAGUYA_PP_TARG
#undef KAGUYA_GET_DEF
#undef KAGUYA_GET_TUPLE_DEF
#endif

			template<class Result>
		inline Result get_result(lua_State* l, int startindex)
		{
			return get_result_impl(l, startindex, types::typetag<Result>());
		}
		template<>
		inline void get_result<void>(lua_State* l, int startindex)
		{
		}
	}


	/**
	* Reference of Lua any type value.
	*/
	class LuaRef :public Ref::RegistoryRef, public detail::LuaVariantImpl<LuaRef>
	{
		friend class LuaUserData;
		friend class LuaTable;
		friend class LuaFunction;
		friend class LuaThread;
	private:
		typedef void (LuaRef::*bool_type)() const;
		void this_type_does_not_support_comparisons() const {}


		static lua_State* toMainThread(lua_State* state)
		{
#if LUA_VERSION_NUM >= 502
			if (state)
			{
				lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
				lua_State* mainthread = lua_tothread(state, -1);
				lua_pop(state, 1);
				if (mainthread)
				{
					return mainthread;
				}
			}
#endif
			return state;
		}
	public:


		LuaRef(const Ref::RegistoryRef& src) :Ref::RegistoryRef(src)
		{
		}
		LuaRef(const LuaRef& src) :Ref::RegistoryRef(src)
		{
		}
		LuaRef& operator =(const LuaRef& src)
		{
			static_cast<RegistoryRef&>(*this) = src;
			return *this;
		}

#if KAGUYA_USE_CPP11

		LuaRef(LuaRef&& src)throw() :Ref::RegistoryRef(std::move(src)) {}

		LuaRef& operator =(LuaRef&& src)throw()
		{
			swap(src);
			return *this;
		}

		LuaRef(RegistoryRef&& src)throw() :Ref::RegistoryRef(std::move(src)) {}
		template<typename T>
		LuaRef(lua_State* state, T&& v, Ref::NoMainCheck) : Ref::RegistoryRef(state, std::move(v), Ref::NoMainCheck()) {}
		template<typename T>
		LuaRef(lua_State* state, T&& v) : Ref::RegistoryRef(state, std::move(v)) {}
#endif

		LuaRef() {}
		LuaRef(lua_State* state) :Ref::RegistoryRef(state) {}

		LuaRef(lua_State* state, StackTop, Ref::NoMainCheck) :Ref::RegistoryRef(state, StackTop(), Ref::NoMainCheck()) {}

		LuaRef(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
		}


		template<typename T>
		LuaRef(lua_State* state, const T& v, Ref::NoMainCheck) : Ref::RegistoryRef(state, v, Ref::NoMainCheck()) {}
		template<typename T>
		LuaRef(lua_State* state, const T& v) : Ref::RegistoryRef(state, v) {}

		bool isNilref()const { return state() == 0 || ref_ == LUA_REFNIL; }



		//push to Lua stack
		int push()const
		{
			return push(state());
		}
		int push(lua_State* s)const
		{
			if (isNilref())
			{
				lua_pushnil(s);
				return 1;
			}
#if LUA_VERSION_NUM >= 502
			if (s != state())
			{//state check
				assert(toMainThread(s) == toMainThread(state()));
			}
#endif
			lua_rawgeti(s, LUA_REGISTRYINDEX, ref_);
			return 1;
		}
		int pushStackIndex(lua_State* state)const
		{
			push(state);
			return lua_gettop(state);
		}



		const void* native_pointer()const
		{
			util::ScopedSavedStack save(state());
			push(state());
			return lua_topointer(state(), -1);
		}
		//@}
		static void putindent(std::ostream& os, int indent)
		{
			while (indent-- > 0)
			{
				os << "  ";
			}
		}
	};


	template<>
	struct lua_type_traits<LuaRef>
	{
		typedef LuaRef get_type;
		typedef const LuaRef& push_type;

		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return false;
		}

		static get_type get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaRef(l, StackTop());
		}
		static int push(lua_State* l, push_type v)
		{
			return v.push(l);
		}
	};
	template<>	struct lua_type_traits<const LuaRef&> :lua_type_traits<LuaRef> {};




	class LuaStackRef :public Ref::StackRef, public detail::LuaVariantImpl<LuaStackRef>
	{
	public:
		LuaStackRef() :Ref::StackRef()
		{
		}
		LuaStackRef(lua_State* s, int index) :Ref::StackRef(s, index, false)
		{
		}
		LuaStackRef(lua_State* s, int index,bool popAtDestruct) :Ref::StackRef(s, index, popAtDestruct)
		{
		}
#if KAGUYA_USE_CPP11
		LuaStackRef(LuaStackRef&& src) : Ref::StackRef(std::move(src))
		{
			src.pop_ = false;
		}
		LuaStackRef& operator=(LuaStackRef&& src)
		{
			if (this != &src)
			{
				Ref::StackRef::operator=(std::move(src));
				src.pop_ = false;
			}
			return *this;
		}
		LuaStackRef(const LuaStackRef&src) = delete;
#else
		LuaStackRef(const LuaStackRef&src) : Ref::StackRef(src)
		{
			src.pop_ = false;
		}
		LuaStackRef& operator=(const LuaStackRef&src)
		{
			if (this != &src)
			{
				Ref::StackRef::operator=(src);
				src.pop_ = false;
			}
			return *this;
		}
#endif
	};



	template<>
	struct lua_type_traits<LuaStackRef>
	{
		typedef LuaStackRef get_type;
		typedef const LuaStackRef& push_type;

		static bool checkType(lua_State* l, int index)
		{
			return true;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			return false;
		}

		static get_type get(lua_State* l, int index)
		{
			return LuaStackRef(l, index);
		}
		static int push(lua_State* l, push_type v)
		{
			return v.push(l);
		}
	};
	template<>	struct lua_type_traits<const LuaStackRef&> :lua_type_traits<LuaStackRef> {};
	
	//! Reference to Lua userdata
	class  LuaUserData :public Ref::RegistoryRef
		, public detail:: LuaTableOrUserDataImpl<LuaUserData>
		, public detail::LuaBasicTypeFunctions<LuaUserData>
	{

		void typecheck()
		{
			int t = type();
			if (t != TYPE_USERDATA && t != TYPE_LIGHTUSERDATA &&t != TYPE_NIL && t != TYPE_NONE)
			{
				except::typeMismatchError(state(), "not user data");
				Ref::RegistoryRef::unref();
			}
		}
	public:
		operator LuaRef() {
			push(state());
			return LuaRef(state(), StackTop());
		}
		LuaUserData(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
			typecheck();
		}
		template<typename TYPE>
		LuaUserData(lua_State* state, const TYPE& table) :Ref::RegistoryRef(state, table)
		{
			typecheck();
		}
		LuaUserData(lua_State* state) :Ref::RegistoryRef(state, NilValue())
		{
			typecheck();
		}
		LuaUserData()
		{
			typecheck();
		}
		template<typename T>
		bool typeTest()const
		{
			util::ScopedSavedStack save(state());
			return lua_type_traits<T>::strictCheckType(state(), pushStackIndex(state()));
		}
		template<typename T>
		bool weakTypeTest()const
		{
			util::ScopedSavedStack save(state());
			return lua_type_traits<T>::checkType(state(), pushStackIndex(state()));
		}
	};


	template<>	struct lua_type_traits<LuaUserData> {
		typedef LuaUserData get_type;
		typedef LuaUserData push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TUSERDATA;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TUSERDATA || lua_isnil(l, index);
		}
		static LuaUserData get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaUserData(l, StackTop());
		}
		static int push(lua_State* l, const LuaUserData& ref)
		{
			return ref.push(l);
		}
	};
	template<>	struct lua_type_traits<const LuaUserData&> :lua_type_traits<LuaUserData> {};


	class LuaTable :public Ref::RegistoryRef
		, public detail::LuaTableImpl<LuaTable>
		, public detail::LuaTableOrUserDataImpl<LuaTable>
		, public detail::LuaBasicTypeFunctions<LuaTable>
	{

		void typecheck()
		{
			int t = type();
			if (t != TYPE_TABLE && t != TYPE_NIL && t != TYPE_NONE)
			{
				except::typeMismatchError(state(), "not table");
				Ref::RegistoryRef::unref();
			}
		}
	public:
		operator LuaRef() {
			push(state());
			return LuaRef(state(), StackTop());
		}
		LuaTable(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
			typecheck();
		}
		LuaTable(lua_State* state, const NewTable& table) :Ref::RegistoryRef(state, table)
		{
			typecheck();
		}
		LuaTable(lua_State* state) :Ref::RegistoryRef(state, NewTable())
		{
			typecheck();
		}
		LuaTable()
		{
			typecheck();
		}
	};

	template<>	struct lua_type_traits<LuaTable> {
		typedef LuaTable get_type;
		typedef LuaTable push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_istable(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_istable(l, index) || lua_isnil(l, index);
		}
		static LuaTable get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaTable(l, StackTop());
		}
		static int push(lua_State* l, const LuaTable& ref)
		{
			return ref.push(l);
		}
	};
	template<>	struct lua_type_traits<const LuaTable&> :lua_type_traits<LuaTable> {};

	/**
	* Reference of Lua function.
	*/
	class LuaFunction :public Ref::RegistoryRef
		, public detail::LuaFunctionImpl<LuaFunction>
		, public detail::LuaBasicTypeFunctions<LuaFunction>
	{
		void typecheck()
		{
			int t = type();
			if (t != TYPE_FUNCTION && t != TYPE_NIL && t != TYPE_NONE)
			{
				except::typeMismatchError(state(), "not function");
				RegistoryRef::unref();
			}
		}

	public:

		/**
		* @name constructor
		* @brief construct with state and function .
		* @param state pointer to lua_State
		* @param function e.g. kaguya::function(function_ptr),kaguya::overload(function_ptr)
		*/
		template<typename F>
		LuaFunction(lua_State* state, F f) :Ref::RegistoryRef(state, f)
		{
			typecheck();
		}

		/**
		* @name constructor
		* @brief construct with stack top value.
		* @param state pointer to lua_State
		* @param StackTop tag
		*/
		LuaFunction(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
			typecheck();
		}

		/**
		* @name constructor
		* @brief construct nil reference.
		*/
		LuaFunction()
		{
		}

		/**
		* @name loadstring
		* @brief load lua code .
		* @param state pointer to lua_State
		* @param luacode string
		*/
		static LuaFunction loadstring(lua_State* state, const std::string& luacode)
		{
			return loadstring(state, luacode.c_str());
		}
		/**
		* @name loadstring
		* @brief load lua code .
		* @param state pointer to lua_State
		* @param luacode string
		*/
		static LuaFunction loadstring(lua_State* state, const char* luacode)
		{
			util::ScopedSavedStack save(state);
			int status = luaL_loadstring(state, luacode);

			if (status)
			{
				ErrorHandler::handle(status, state);
				lua_pushnil(state);
			}
			return LuaFunction(state, StackTop());
		}


		/**
		* @name loadfile
		* @brief If there are no errors,compiled file as a Lua function and return.
		*  Otherwise send error message to error handler and return nil reference
		* @param file  file path of lua script
		* @return reference of lua function
		*/
		static LuaFunction loadfile(lua_State* state, const std::string& file)
		{
			return loadfile(state, file.c_str());
		}
		static LuaFunction loadfile(lua_State* state, const char* file)
		{
			util::ScopedSavedStack save(state);

			int status = luaL_loadfile(state, file);

			if (status)
			{
				ErrorHandler::handle(status, state);
				lua_pushnil(state);
			}
			return LuaFunction(state, StackTop());
		}


		struct LuaLoadStreamWrapper
		{
			LuaLoadStreamWrapper(std::istream& stream) :preloaded_(false), stream_(stream)
			{
				buffer_.reserve(512);
				skipComment();
				preloaded_ = !buffer_.empty();
			}

			void skipComment()
			{
				//skip bom
				const char* bom = "\xEF\xBB\xBF";
				const char* bomseq = bom;
				char c;
				while (stream_.get(c))
				{
					if (c != *bomseq)// not bom sequence
					{
						buffer_.assign(bom, bomseq);
						buffer_.push_back(c);
						break;
					}
					bomseq++;
					if ('\0' == *bomseq)
					{
						return;
					}
				}
				
				//skip comment
				if (!buffer_.empty() && buffer_.front() == '#')
				{
					buffer_.clear();
					std::string comment;
					std::getline(stream_, comment);
				}
			}
			

			static const char *getdata(lua_State *, void *ud, size_t *size) {
				LuaLoadStreamWrapper *loader = static_cast<LuaLoadStreamWrapper *>(ud);

				if (loader->preloaded_)
				{
					loader->preloaded_ = false;
				}
				else
				{
					loader->buffer_.clear();
				}

				char c = 0;
				while (loader->buffer_.size() < loader->buffer_.capacity() && loader->stream_.get(c))
				{
					loader->buffer_.push_back(c);
				}
				*size = loader->buffer_.size();
				return loader->buffer_.empty()?0:&loader->buffer_[0];
			}
		private:
			bool preloaded_;
			std::vector<char> buffer_;
			std::istream& stream_;
		};

		/**
		* @name loadstream
		* @brief If there are no errors,compiled stream as a Lua function and return.
		*  Otherwise send error message to error handler and return nil reference
		* @param file  file path of lua script
		* @return reference of lua function
		*/
		static LuaStackRef loadstreamtostack(lua_State* state, std::istream& stream, const char* chunkname = 0)
		{
			LuaLoadStreamWrapper wrapper(stream);
#if LUA_VERSION_NUM >= 502
			int status = lua_load(state, &LuaLoadStreamWrapper::getdata, &wrapper, chunkname, 0);
#else
			int status = lua_load(state, &LuaLoadStreamWrapper::getdata, &wrapper, chunkname);
#endif
			if (status)
			{
				ErrorHandler::handle(status, state);
				lua_pushnil(state);
			}
			return LuaStackRef(state,-1,true);
		}

		static LuaFunction loadstream(lua_State* state, std::istream& stream, const char* chunkname = 0)
		{
			util::ScopedSavedStack save(state);
			LuaLoadStreamWrapper wrapper(stream);
#if LUA_VERSION_NUM >= 502
			int status = lua_load(state, &LuaLoadStreamWrapper::getdata, &wrapper, chunkname, 0);
#else
			int status = lua_load(state, &LuaLoadStreamWrapper::getdata, &wrapper, chunkname);
#endif
			if (status)
			{
				ErrorHandler::handle(status, state);
				lua_pushnil(state);
			}
			return LuaFunction(state, StackTop());
		}
	};

	/**
	* Reference of Lua thread(==coroutine).
	*/
	class LuaThread :public Ref::RegistoryRef
		, public detail::LuaThreadImpl<LuaThread>
		, public detail::LuaBasicTypeFunctions<LuaThread>
	{
		void typecheck()
		{
			int t = type();
			if (t != TYPE_THREAD && t != TYPE_NIL && t != TYPE_NONE)
			{
				except::typeMismatchError(state(), "not lua thread");
				RegistoryRef::unref();
			}
		}
	public:
		LuaThread(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
			typecheck();
		}
		LuaThread(lua_State* state, const NewThread& t) :Ref::RegistoryRef(state, t)
		{
		}
		LuaThread(lua_State* state) :Ref::RegistoryRef(state, NewThread())
		{
		}
		LuaThread()
		{
		}
		void setFunction(const LuaFunction& f)
		{
			typecheck();
			lua_State* corstate = get<lua_State*>();
			lua_settop(corstate,0);
			f.push(corstate);
		}
	};


}



#if KAGUYA_USE_CPP11
#else
namespace std
{
	template <> inline void swap(kaguya::LuaUserData& a, kaguya::LuaUserData& b)
	{
		a.swap(b);
	}
	template <> inline void swap(kaguya::LuaTable& a, kaguya::LuaTable& b)
	{
		a.swap(b);
	}
	template <> inline void swap(kaguya::LuaFunction& a, kaguya::LuaFunction& b)
	{
		a.swap(b);
	}
	template <> inline void swap(kaguya::LuaThread& a, kaguya::LuaThread& b)
	{
		a.swap(b);
	}
	template <> inline void swap(kaguya::LuaRef& a, kaguya::LuaRef& b)
	{
		a.swap(b);
	}
}
#endif
