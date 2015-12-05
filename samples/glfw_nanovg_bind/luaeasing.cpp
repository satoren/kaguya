
#include "kaguya/kaguya.hpp"
#include "easing.hpp"

using namespace kaguya;


extern "C"
#ifdef _MSC_VER
__declspec(dllexport)
#endif
int luaopen_luaeasing(lua_State *L)
{
	State state(L);
	LuaRef easing = state.newLib();
	
#define EASING_LUA_REGFUNC(FNAME) easing[#FNAME] = function(&Easing:: FNAME <double>)
	EASING_LUA_REGFUNC(Liner);
	EASING_LUA_REGFUNC(InSine);
	EASING_LUA_REGFUNC(OutSine);
	EASING_LUA_REGFUNC(InOutSine);
	EASING_LUA_REGFUNC(InQuad);
	EASING_LUA_REGFUNC(OutQuad);
	EASING_LUA_REGFUNC(InOutQuad);
	EASING_LUA_REGFUNC(InCubic);
	EASING_LUA_REGFUNC(OutCubic);
	EASING_LUA_REGFUNC(InOutCubic);
	EASING_LUA_REGFUNC(InQuart);
	EASING_LUA_REGFUNC(OutQuart);
	EASING_LUA_REGFUNC(InOutQuart);
	EASING_LUA_REGFUNC(InQuint);
	EASING_LUA_REGFUNC(OutQuint);
	EASING_LUA_REGFUNC(InOutQuint);
	EASING_LUA_REGFUNC(InExpo);
	EASING_LUA_REGFUNC(OutExpo);
	EASING_LUA_REGFUNC(InOutExpo);
	EASING_LUA_REGFUNC(InCirc);
	EASING_LUA_REGFUNC(OutCirc);
	EASING_LUA_REGFUNC(InOutCirc);
	EASING_LUA_REGFUNC(InBack);
	EASING_LUA_REGFUNC(OutBack);
	EASING_LUA_REGFUNC(InOutBack);	
	EASING_LUA_REGFUNC(InElastic);
	EASING_LUA_REGFUNC(OutElastic);
	EASING_LUA_REGFUNC(InOutElastic);
	EASING_LUA_REGFUNC(InBounce);
	EASING_LUA_REGFUNC(OutBounce);
	EASING_LUA_REGFUNC(InOutBounce);
	return 1;
}
