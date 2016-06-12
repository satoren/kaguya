#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_09_utility)

using namespace kaguya_test_util;


KAGUYA_TEST_FUNCTION_DEF(lua_resume_test)(kaguya::State& s)
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
KAGUYA_TEST_FUNCTION_DEF(lua_compare_test)(kaguya::State& s)
{
	using namespace kaguya;

	s.pushToStack(2);
	s.pushToStack(2);
	TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPEQ));
	TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLE));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLT));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2));//invalid option
	s.popFromStack();
	s.popFromStack();

	s.pushToStack(4);
	s.pushToStack(2);
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPEQ));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLT));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2));//invalid option
	s.popFromStack();
	s.popFromStack();

	s.pushToStack(2);
	s.pushToStack(4);
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPEQ));
	TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLE));
	TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLT));
	TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2));//invalid option
	s.popFromStack();
	s.popFromStack();
}


int lua_mylibf(lua_State* L)
{
	kaguya::State state(L);
	kaguya::LuaTable t = state.newTable();
	t["value"] = 111;
	return t.push();
}

KAGUYA_TEST_FUNCTION_DEF(luaL_requiref_test)(kaguya::State& s)
{
	using namespace kaguya;

	luaL_requiref(s.state(),"mylib", lua_mylibf,false);
	kaguya::LuaStackRef ref(s.state(), -1,true);
	TEST_EQUAL(ref["value"],111);
	TEST_CHECK(s("assert(mylib == nil)"));
}
KAGUYA_TEST_GROUP_END(test_09_utility)
