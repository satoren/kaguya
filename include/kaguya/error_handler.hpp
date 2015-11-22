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
		ErrorHandler() {}

		void handle(int status_code, lua_State *state)
		{
			if (handler_)
			{
				handler_(status_code,get_error_message(state));
			}
		}
		void handle(int status_code,const char* message)
		{
			if (handler_)
			{
				handler_(status_code,message);
			}			
		}
		void setFunction(standard::function<void(int,const char* message)> f)
		{
			handler_ = f;
		}
	private:
		standard::function<void(int,const char* message)> handler_;

		ErrorHandler(const ErrorHandler&);
		ErrorHandler& operator=(const ErrorHandler&);
	};
};
