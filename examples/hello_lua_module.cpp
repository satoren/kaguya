#include <iostream>
#include <kaguya/kaguya.hpp>


void hello()
{
	std::cout << "hello cpp module" << std::endl;
}


#if defined(_WIN32) || defined(_WIN64)
extern "C" __declspec(dllexport)
#else
extern "C" __attribute__((visibility("default")))
#endif
int luaopen_hello_lua_module(lua_State* L)
{
	using namespace kaguya;

	State state(L);
	LuaTable module = state.newTable();

	module["hello"]=kaguya::function(&hello);

	
	return module.push();
}
