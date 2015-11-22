#pragma once

#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/error_handler.hpp"

namespace kaguya
{
	struct Evaluator
	{
		template<typename T>
		T get()
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
		template<typename T1>
		standard::tuple<T1> getTuple()
		{
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.size() >= 1)
			{
				const std::vector<LuaRef>& r = res_->results;
				return standard::tuple<T1>(r[0].get<T1>());
			}
			error_handler_.handle(0, "no result");
			return T();
		}

		template<typename T>
		operator T() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
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
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<T*>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}

		operator bool() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<bool>();
			}
			error_handler_.handle(0, "no result");
			return false;
		}
		operator float() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<float>();
			}
			error_handler_.handle(0, "no result");
			return (0);
		}
		operator double() {
			evaluate(1);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<double>();
			}
			error_handler_.handle(0, "no result");
			return (0);
		}

		operator int() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<int>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}
		operator int64_t() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<int64_t>();
			}
			error_handler_.handle(0, "no result");
			return 0;
		}

		operator std::string() {
			evaluate(1);
			utils::ScopedSavedStack save(state_);
			if (res_ && !res_->results.empty())
			{
				return res_->results.back().get<std::string>();
			}
			return "";
		}
		bool operator == (bool v) { return get<bool>() == v; }
		bool operator == (const std::string& v) { return get<std::string>() == v; }
		bool operator == (const char* v) { return get<std::string>() == v; }
		bool operator == (float v) { return get<float>() == v; }
		bool operator == (double v) { return get<double>() == v; }
		bool operator == (int v) { return get<int>() == v; }
		bool operator == (int64_t v) { return get<int64_t>() == v; }

		template<typename T>
		bool operator == (const T* v)
		{
			return get<T>() == v;
		}
		template<typename T>
		bool operator != (const T& v)
		{
			return !((*this) == v);
		}

		~Evaluator()
		{
			evaluate(0);
		}
	private:


		void evaluate(int resultnum)
		{
			if (res_ && res_->owner == this && !res_->invoked)
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
				lua_settop(state_, res_->stack_top_);
			}
		}

		friend class Selector;

		Evaluator(lua_State* state, size_t argnum, int stacktop, ErrorHandler& error_handler) :state_(state), error_handler_(error_handler)
		{
			res_ = standard::shared_ptr<eval>(new eval());
			res_->arg_num_ = argnum;
			res_->stack_top_ = stacktop;
			res_->owner = this;
			res_->invoked = false;
		}
		Evaluator(const Evaluator& src) :state_(src.state_), error_handler_(src.error_handler_), res_(src.res_)
		{
			res_->owner = this;
		}

		struct eval
		{
			int stack_top_;
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
