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
		T get()const
		{
			evaluate(1);
			if (eval_info_ && !eval_info_->results.empty())
			{
				return eval_info_->results.back().get<T>();
			}
			//error
			assert(false);
			return T();
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
			util::ScopedSavedStack save(state_);

			if (eval_info_->owner == this && !eval_info_->invoked)
			{
				eval_info_->invoked = true;
				const std::vector<LuaRef>& args = eval_info_->args;
				if (eval_info_->coroutine)
				{
					for (size_t i = 0; i < args.size(); ++i)
					{
						args[i].push(state_);
					}
#if LUA_VERSION_NUM >= 502
					int result = lua_resume(state_, 0, args.size()? args.size()-1:0);
#else
					int result = lua_resume(state_, args.size() ? args.size() - 1 : 0);
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
					int result = lua_pcall(state_, eval_info_->args.size(), resultnum, 0);

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
			eval() :invoked(false), coroutine(false) {}
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
