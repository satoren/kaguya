extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <cstdlib>
#include <iostream>

int main(int argc,const char* argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	if (argc > 1 && luaL_dofile(L, argv[1]))
	{
		std::cerr << lua_tostring(L,-1) << std::endl;
	}
#ifdef WIN32
	system("PAUSE");
#endif
	lua_close(L);
}
