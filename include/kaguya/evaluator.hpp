#pragma once

#include <algorithm>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/error_handler.hpp"


namespace kaguya
{
	struct Evaluator
	{
		template<typename T>
		T get()const
		{
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<T>();
			}
			error_handler_.handle(0, "no result");
			return T();
		}

		template<typename T>
		operator T()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<T>();
			}
			error_handler_.handle(0, "no result");
			return T();
		}
		// can not support msvc..
		//template<typename T>
		//operator T&()const  {
		//  evaluate(1);
		//  if(!results.empty())
		//  {
		//    return results.back().get<T&>();
		//  }
		//}
		template<typename T>
		operator T*()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<T*>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}

		operator bool()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<bool>();
			}
			error_handler_.handle(0, "no result");
			return false;
		}
		operator float()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<float>();
			}
			error_handler_.handle(0, "no result");
			return (0);
		}
		operator double()const {
			evaluate(1);
			if (!res_->results.empty())
			{
				return res_->results.back().get<double>();
			}
			error_handler_.handle(0, "no result");
			return (0);
		}

		operator int()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<int>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}
		operator int64_t()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<int64_t>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}

		operator std::string()const {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (!res_->results.empty())
			{
				return res_->results.back().get<std::string>();
			}
			return "";
		}
		bool operator == (bool v)const { return get<bool>() == v; }
		bool operator == (const std::string& v)const { return get<std::string>() == v; }
		bool operator == (const char* v)const { return get<std::string>() == v; }
		bool operator == (float v)const { return get<float>() == v; }
		bool operator == (double v)const { return get<double>() == v; }
		bool operator == (int v)const { return get<int>() == v; }
		bool operator == (int64_t v)const { return get<int64_t>() == v; }

		template<typename T>
		bool operator == (const T* v)const
		{
			return get<T>() == v;
		}
		template<typename T>
		bool operator != (const T& v)const
		{
			return !((*this) == v);
		}

		~Evaluator()
		{
			evaluate(0);
		}

		const std::vector<LuaRef>& get_result(int result_count)const
		{
			evaluate(result_count);
			return res_->results;
		}
	private:
		void evaluate(int resultnum)const
		{
			if (res_->owner == this && !res_->invoked)
			{
				res_->invoked = true;
				int result = lua_pcall(state_, res_->arg_num_, resultnum, 0);
				if (result)
				{
					error_handler_.handle(result, state_);
				}
				for (int i = 0; i < resultnum; ++i)
				{
					res_->results.push_back(LuaRef(state_, StackTop()));
				}
				std::reverse(res_->results.begin(), res_->results.end());

				lua_settop(state_, res_->reset_stack_top_);
			}
		}

		friend class Selector;

		Evaluator(lua_State* state, size_t argnum, int stacktop, ErrorHandler& error_handler) :state_(state), error_handler_(error_handler)
		{
			res_ = standard::shared_ptr<eval>(new eval());
			res_->arg_num_ = argnum;
			res_->reset_stack_top_ = stacktop;
			res_->owner = this;
			res_->invoked = false;
		}
		Evaluator(const Evaluator& src) :state_(src.state_), error_handler_(src.error_handler_), res_(src.res_)
		{
			res_->owner = this;
		}

		struct eval
		{
			int reset_stack_top_;
			size_t arg_num_;
			std::vector<LuaRef> results;
			bool invoked;
			Evaluator* owner;
		};

		Evaluator& operator=(const Evaluator& src);

		lua_State* state_;
		ErrorHandler& error_handler_;
		standard::shared_ptr<eval> res_;
	};
}

#include "kaguya/ref_tuple.hpp"