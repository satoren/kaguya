#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_09_utility)

using namespace kaguya_test_util;


KAGUYA_TEST_FUNCTION_DEF(resume)(kaguya::State& s)
{
	using namespace kaguya;

	
	LuaThread t = s.newThread(s.loadstring("v={...}"));

	lua_State* co = t.get<lua_State*>();
	lua_pushnumber(co, 2);
	lua_pushnumber(co, 3);

	lua_resume(co,s.state(),2);

	TEST_EQUAL(s["v"][1], 2);
	TEST_EQUAL(s["v"][2], 3);
}

KAGUYA_TEST_GROUP_END(test_09_utility)
