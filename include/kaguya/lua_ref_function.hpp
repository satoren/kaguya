// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/exception.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"


#include "kaguya/detail/lua_function_def.hpp"

namespace kaguya
{

	/**
	* Reference of Lua function.
	*/
	class LuaFunction :public Ref::RegistoryRef, public LuaFunctionImpl<LuaFunction>, public LuaBasicTypeFunctions<LuaFunction>
	{
		void typecheck()
		{
			if (type() != TYPE_FUNCTION)
			{
				except::typeMismatchError(state(), "not function");
				RegistoryRef::unref();
			}
		}

	public:

		LuaFunction(lua_State* state, StackTop) :Ref::RegistoryRef(state, StackTop())
		{
			typecheck();
		}
		LuaFunction()
		{
			typecheck();
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
				ErrorHandler::instance().handle(status, state);
				return LuaRef(state);
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
				ErrorHandler::instance().handle(status, state);
				return LuaRef(state);
			}
			return LuaFunction(state, StackTop());
		}
	};

	/**
	* Reference of Lua thread(==coroutine).
	*/
	class LuaThread :public Ref::RegistoryRef, public LuaThreadImpl<LuaThread>, public LuaBasicTypeFunctions<LuaThread>
	{
		void typecheck()
		{
			if (type() != TYPE_THREAD)
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
			typecheck();
		}
		LuaThread(lua_State* state) :Ref::RegistoryRef(state, NewThread())
		{
			typecheck();
		}
		LuaThread()
		{
			typecheck();
		}

	};


    
    
    
	class FunctionResults:public LuaVariantImpl<FunctionResults>
	{
		FunctionResults() :state_(0), startIndex_(0), endIndex_(0)
		{
		}
		FunctionResults(lua_State* state) :state_(state), startIndex_(0), endIndex_(0)
		{
		}
		FunctionResults(lua_State* state, int startIndex) :state_(state), startIndex_(startIndex), endIndex_(lua_gettop(state) + 1)
		{
		}
		FunctionResults(lua_State* state, int startIndex, int endIndex) :state_(state), startIndex_(startIndex), endIndex_(endIndex)
		{
		}
		FunctionResults(const FunctionResults&src) :state_(src.state_), startIndex_(src.startIndex_), endIndex_(src.endIndex_)
		{
			src.state_ = 0;
		}
		friend class LuaRef;
		friend class mem_fun_binder;
		friend class TableKeyReference;
		template<typename Derived>friend class LuaVariantImpl;
		template<typename Derived>friend class LuaThreadImpl;
		template<typename Derived>friend class LuaFunctionImpl;
	public:
		~FunctionResults()
		{
			if (state_)
			{
				lua_settop(state_, startIndex_ - 1);
			}
		}

		struct reference :Ref::StackRef, public LuaVariantImpl<reference>
		{
			reference(lua_State* s, int index) :Ref::StackRef(s,index,false)
			{
			}

			const reference* operator->()const
			{
				return this;
			}
		};
		struct iterator
		{
			iterator(lua_State* s, int i): state(s), stack_index(i)
			{
			}
			reference operator*()const
			{
				return reference(state, stack_index);
			}
			reference operator->()const
			{
				return reference(state, stack_index);
			}
			const iterator& operator++()
			{
				stack_index++;
				return *this;
			}
			iterator operator++(int)
			{
				return iterator(state, stack_index++);
			}

			iterator operator+=(int n)
			{
				stack_index += n;
				return iterator(state, stack_index);
			}
			bool operator==(const iterator& other)const
			{
				return state == other.state && stack_index == other.stack_index;
			}
			bool operator!=(const iterator& other)const
			{
				return !(*this == other);
			}
		private:
			lua_State* state;
			int stack_index;
		};
		typedef iterator const_iterator;
		typedef reference const_reference;

		iterator begin()
		{
			return iterator(state_, startIndex_);
		}
		iterator end()
		{
			return iterator(state_, endIndex_);
		}
		const_iterator cbegin()const
		{
			return const_iterator(state_, startIndex_);
		}
		const_iterator cend()const
		{
			return const_iterator(state_, endIndex_);
		}


		template<class Result>
		Result get()const
		{
			return util::get_result<Result>(state_, startIndex_);
		}
		template<typename T>
		operator T()const {
			return get<T>();
		}
		lua_State* state()const {return state_;	}
		int pushStackIndex(lua_State* state)const {
			if (state_ != state)
			{
				throw std::runtime_error("can not now");//fixme
			}
			return startIndex_; 
		}


		template<typename T>
		typename lua_type_traits<T>::get_type result_at(size_t index)const
		{
			if (index < 0 || index >= result_size())
			{
				throw std::out_of_range("function result out of range");
			}
			return lua_type_traits<T>::get(state_, startIndex_ + static_cast<int>(index));
		}

		size_t result_size()const
		{
			return endIndex_ - startIndex_;
		}

		template<typename T>
		bool operator == (const T v)const
		{
			return get<T>() == v;
		}
		template<typename T>
		bool operator != (const T v)const
		{
			return !((*this) == v);
		}
		bool operator == (const char* v)const { return get<std::string>() == v; }

		//push first result
		int push(lua_State* state)const
		{
			if (state_ != state)
			{
				throw std::runtime_error("can not now");//fixme
			}
			lua_pushvalue(state, startIndex_);
			return 1;
		}
		int type()const
		{
			return lua_type(state_, startIndex_);
		}

		template<typename RetType>
		static RetType ReturnValue(lua_State* state, int retindex, types::typetag<RetType> tag)
		{
			return FunctionResults(state, retindex).get<RetType>();
		}
		static FunctionResults ReturnValue(lua_State* state, int retindex, types::typetag<FunctionResults> tag)
		{
			return FunctionResults(state, retindex);
		}
		static void ReturnValue(lua_State* state, int retindex, types::typetag<void> tag)
		{
			FunctionResults(state, retindex);
		}

	private:
		mutable lua_State* state_;
		int startIndex_;
		int endIndex_;

	};

	inline std::ostream& operator<<(std::ostream& os, const FunctionResults& res)
	{
		std::vector<LuaRef> results = res.get<std::vector<LuaRef> >();

		for (std::vector<LuaRef>::iterator it = results.end(); it != results.end(); ++it)
		{
			it->dump(os); os << ",";
		}

		return os;
	}


	template<>	struct lua_type_traits<FunctionResults> {
		static int push(lua_State* l, const FunctionResults& ref)
		{
			int size = 0;
			for (FunctionResults::const_iterator it = ref.cbegin(); it != ref.cend(); ++it)
			{
				size += it->push();
			}
			return size;
		}
	};
	template <unsigned int I>
	LuaRef get(const FunctionResults& res) { return (res.result_size() > I)?res.result_at<LuaRef>(I): LuaRef(); }

	template<>	struct lua_type_traits<LuaFunction> {
		typedef LuaFunction get_type;
		typedef LuaFunction push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isfunction(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isfunction(l, index) || lua_isnil(l, index);
		}
		static LuaFunction get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaFunction(l, StackTop());
		}
		static int push(lua_State* l, const LuaFunction& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaFunction&> :lua_type_traits<LuaFunction> {};

	template<>	struct lua_type_traits<LuaThread> {
		typedef LuaThread get_type;
		typedef LuaThread push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_isthread(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_isthread(l, index) || lua_isnil(l, index);
		}
		static LuaThread get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaThread(l, StackTop());
		}
		static int push(lua_State* l, const LuaThread& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaThread&> :lua_type_traits<LuaThread> {};


	inline LuaRef toLuaRef(const LuaFunction& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline LuaRef toLuaRef(const LuaThread& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline LuaRef toLuaRef(const FunctionResults& ref)
	{
		return ref.get<LuaRef>();
	}

	/**
	* @brief table and function binder.
	* state["table"]->*"fun"() is table:fun() in Lua
	* @param arg... function args
	*/
	class mem_fun_binder
	{
	public:
		template<class T>
		mem_fun_binder(LuaRef self, T key) :self_(self), fun_(self_.getField(key))
		{
		}

#define KAGUYA_DELEGATE_LUAREF fun_
#define KAGUYA_DELEGATE_FIRST_ARG self_
#include "kaguya/delegate_to_luaref.inl"
#undef KAGUYA_DELEGATE_FIRST_ARG
#undef KAGUYA_DELEGATE_LUAREF

	private:
		LuaRef self_;//Table or Userdata
		LuaFunction fun_;
	};
	/*
	inline mem_fun_binder LuaRef::operator->*(const char* key)
	{
		return mem_fun_binder(*this, key);
	}*/
}
