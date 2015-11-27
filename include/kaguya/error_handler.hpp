#pragma once

#include <string>

#include "kaguya/config.hpp"


namespace kaguya
{

	const char* get_error_message(lua_State *state)
	{
		return lua_tostring(state, -1);
	}
	struct ErrorHandler
	{
		typedef standard::function<void(int, const char* )> function_type;


		void handle(const char* message, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(0, message);
			}
		}
		void handle(int status_code, lua_State *state)
		{
			function_type handler = getHandler(state);
			if (handler)
			{
				handler(status_code,get_error_message(state));
			}
		}

		function_type getHandler(lua_State* state)const
		{
			standard::lock_guard<standard::mutex> lk(mutex_);
			std::map<lua_State*, function_type>::const_iterator match = handler_map_.find(state);
			if (match != handler_map_.end())
			{
				return match->second;
			}
			return function_type();
		}


		void unregisterHandler(lua_State* state)
		{
			if (state)
			{
				standard::lock_guard<standard::mutex> lk(mutex_);
				handler_map_.erase(state);
			}
		}
		void registerHandler(lua_State* state,function_type f)
		{
			if (state)
			{
				standard::lock_guard<standard::mutex> lk(mutex_);
				handler_map_[state] = f;
			}
		}

		static ErrorHandler& instance() {
			static ErrorHandler instance_;
			return instance_;
		}
	private:
		ErrorHandler() {}

		std::map<lua_State*, function_type> handler_map_;
		mutable  standard::mutex mutex_;

		ErrorHandler(const ErrorHandler&);
		ErrorHandler& operator=(const ErrorHandler&);
	};
};
