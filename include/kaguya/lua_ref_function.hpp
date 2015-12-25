#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/exception.hpp"
#include "kaguya/type.hpp"
#include "kaguya/utility.hpp"


namespace kaguya
{

	class LuaFunction :private LuaRef
	{
	public:
		LuaFunction() :LuaRef()
		{
		}
		LuaFunction(const LuaRef& ref) :LuaRef(ref)
		{
			if (ref.type() != TYPE_FUNCTION)
			{
				except::typeMismatchError(state_, "not function");
				LuaRef::unref();
			}
		}
		using LuaRef::operator();
		using LuaRef::setFunctionEnv;
		using LuaRef::getFunctionEnv;
		using LuaRef::isNilref;
		using LuaRef::push;

		bool operator==(const LuaFunction& other)const
		{
			return static_cast<const LuaRef&>(*this) == static_cast<const LuaRef&>(other);
		}
		bool operator!=(const LuaFunction& other)const
		{
			return !(*this == other);
		}
		bool operator<=(const LuaFunction& other)const
		{
			return static_cast<const LuaRef&>(*this) <= static_cast<const LuaRef&>(other);
		}
		bool operator<(const LuaFunction& other)const
		{
			return static_cast<const LuaRef&>(*this) < static_cast<const LuaRef&>(other);
		}
		bool operator>=(const LuaFunction& other)const
		{
			return other <= *this;
		}
		bool operator>(const LuaFunction& other)const
		{
			return other < *this;
		}
	};
	class LuaThread:private LuaRef
	{
	public:
		LuaThread() :LuaRef()
		{
		}
		LuaThread(lua_State* state) :LuaRef(state, NewThread())
		{
		}
		LuaThread(const LuaRef& ref) :LuaRef(ref)
		{
			if (ref.type() != TYPE_THREAD)
			{
				except::typeMismatchError(state_, "not lua thread");
				LuaRef::unref();
			}
		}
		using LuaRef::operator();
		using LuaRef::isNilref;
		using LuaRef::push;
		using LuaRef::threadStatus;
		using LuaRef::isThreadDead;

		bool operator==(const LuaThread& other)const
		{
			return static_cast<const LuaRef&>(*this) == static_cast<const LuaRef&>(other);
		}
		bool operator!=(const LuaThread& other)const
		{
			return !(*this == other);
		}
		bool operator<=(const LuaThread& other)const
		{
			return static_cast<const LuaRef&>(*this) <= static_cast<const LuaRef&>(other);
		}
		bool operator<(const LuaThread& other)const
		{
			return static_cast<const LuaRef&>(*this) < static_cast<const LuaRef&>(other);
		}
		bool operator>=(const LuaThread& other)const
		{
			return other <= *this;
		}
		bool operator>(const LuaThread& other)const
		{
			return other < *this;
		}
	};

	namespace types
	{
		template<>
		inline bool strictCheckType(lua_State* l, int index, typetag<LuaThread>)
		{
			return lua_isthread(l, index);
		}
		template<>
		inline bool checkType(lua_State* l, int index, typetag<LuaThread>)
		{
			return lua_isthread(l, index);
		}
		template<>
		inline LuaThread get(lua_State* l, int index, typetag<LuaThread> tag)
		{
			lua_pushvalue(l, index);
			return LuaRef(l, StackTop());
		}
		template<>
		inline int push(lua_State* l, const LuaThread& ref)
		{
			ref.push(l);
			return 1;
		}

		template<>
		inline bool strictCheckType(lua_State* l, int index, typetag<LuaFunction>)
		{
			return lua_isfunction(l, index);
		}
		template<>
		inline bool checkType(lua_State* l, int index, typetag<LuaFunction>)
		{
			return lua_isfunction(l, index);
		}
		template<>
		inline LuaFunction get(lua_State* l, int index, typetag<LuaFunction> tag)
		{
			lua_pushvalue(l, index);
			return LuaRef(l, StackTop());
		}
		template<>
		inline int push(lua_State* l, const LuaFunction& ref)
		{
			ref.push(l);
			return 1;
		}
	}
	class FunEvaluator
	{
	public:
		FunEvaluator(lua_State* state, LuaRef fun, std::vector<LuaRef> args) :state_(state), eval_info_(new eval())
		{
			eval_info_->owner = this;
			std::swap(eval_info_->fun, fun);
			std::swap(eval_info_->args, args);

			if (eval_info_->fun.type() == LuaRef::TYPE_THREAD)
			{
				state_ = eval_info_->fun.get<lua_State*>();//get coroutine thread
				eval_info_->coroutine = true;
			}
		}

		FunEvaluator(lua_State* state) :state_(state), eval_info_()
		{
			//error!
		}

		FunEvaluator(const FunEvaluator&other) :state_(other.state_), eval_info_(other.eval_info_)
		{
			eval_info_->owner = this;
		}

		~FunEvaluator()
		{
			evaluate(0);
		}

		template<typename T>
		typename traits::arg_get_type<T>::type get()const
		{
			evaluate(1);
			if (eval_info_ && !eval_info_->results.empty())
			{
				return eval_info_->results.back().get<typename traits::arg_get_type<T>::type>();
			}
			return LuaRef(state_);//method invoke error
		}

		template<typename T>
		operator T()const {
			return get<T>();
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

		const std::vector<LuaRef>& get_result(int result_count)const
		{
			evaluate(result_count);
			return eval_info_->results;
		}
	private:

		void evaluate(int resultnum)const
		{

			if (eval_info_&& eval_info_->owner == this && !eval_info_->invoked)
			{
				util::ScopedSavedStack save(state_, 0);
				eval_info_->invoked = true;
				const std::vector<LuaRef>& args = eval_info_->args;
				if (eval_info_->coroutine)
				{
					for (size_t i = 0; i < args.size(); ++i)
					{
						args[i].push(state_);
					}
					int argnum = lua_gettop(state_);
#if LUA_VERSION_NUM >= 502
					int result = lua_resume(state_, 0, argnum > 0 ? argnum - 1 : 0);
#else
					int result = lua_resume(state_, argnum > 0 ? argnum - 1 : 0);
#endif
					except::checkErrorAndThrow(result, state_);
					for (int i = 0; i < resultnum; ++i)
					{
						eval_info_->results.push_back(LuaRef(state_, StackTop()));
					}
					std::reverse(eval_info_->results.begin(), eval_info_->results.end());
				}
				else
				{
					eval_info_->fun.push(state_);

					for (size_t i = 0; i < args.size(); ++i)
					{
						args[i].push(state_);
					}
					int result = lua_pcall(state_, int(args.size()), resultnum, 0);

					except::checkErrorAndThrow(result, state_);
					for (int i = 0; i < resultnum; ++i)
					{
						eval_info_->results.push_back(LuaRef(state_, StackTop()));
					}
					std::reverse(eval_info_->results.begin(), eval_info_->results.end());
				}
			}
		}

		struct eval
		{
			eval() :invoked(false), coroutine(false), owner(0) {}
			LuaRef fun;
			std::vector<LuaRef> args;
			std::vector<LuaRef> results;
			bool invoked;
			bool coroutine;
			FunEvaluator* owner;
		};

		FunEvaluator& operator=(const FunEvaluator& src);

		lua_State* state_;
		standard::shared_ptr<eval> eval_info_;
	};


	class mem_fun_binder
	{
	public:
		template<class T>
		mem_fun_binder(LuaRef self, T key)
		{
			std::swap(self, self_);
			fun_ = self_.getField(key);
		}

#include "kaguya/gen/luaref_mem_fun_def.inl"
	private:
		LuaRef self_;
		LuaRef fun_;
	};

	mem_fun_binder LuaRef::operator->*(const char* key)
	{
		return mem_fun_binder(*this, key);
	}
}
//#include "kaguya/metatable.hpp"

namespace kaguya
{
#include "kaguya/gen/luaref_fun.inl"
}
