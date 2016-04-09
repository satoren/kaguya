#include "kaguya/kaguya.hpp"
#include "test_util.hpp"


KAGUYA_TEST_GROUP_START(test_01_primitive)

using namespace kaguya_test_util;

KAGUYA_TEST_FUNCTION_DEF(bool_get)(kaguya::State& state)
{
	state("value = true");
	TEST_EQUAL(state["value"], true);
};
KAGUYA_TEST_FUNCTION_DEF(int_get)(kaguya::State& state)
{
	state("value = 3");
	TEST_EQUAL(state["value"], int(3));
};
KAGUYA_TEST_FUNCTION_DEF(string_get)(kaguya::State& state)
{
	state("value = 'test'");
	TEST_EQUAL(state["value"], "test");
	TEST_EQUAL(state["value"], std::string("test"));
};
KAGUYA_TEST_FUNCTION_DEF(table_get1)(kaguya::State& state)
{
	state("value = {1,32,'teststr'}");

	TEST_EQUAL(state["value"][1], 1);
	TEST_EQUAL(state["value"][2], 32);
	TEST_EQUAL(state["value"][3], "teststr");
};
KAGUYA_TEST_FUNCTION_DEF(table_get2)(kaguya::State& state)
{
	state("value={out=32,str='gjgj'}");
	state("value['in'] = 'test'");
	kaguya::LuaRef value = state["value"];

	TEST_EQUAL(value["str"], "gjgj");
	TEST_EQUAL(value["in"], "test");
	TEST_EQUAL(value["out"], "32");
};

KAGUYA_TEST_FUNCTION_DEF(bool_set)(kaguya::State& state)
{
	state["value"] = true;
	assert(state["value"] == true);
	TEST_CHECK(state("assert(value == true)"));
};
KAGUYA_TEST_FUNCTION_DEF(int_set)(kaguya::State& state)
{
	state["value"] = 3;
	TEST_CHECK(state("assert(value == 3)"));
};
KAGUYA_TEST_FUNCTION_DEF(string_set)(kaguya::State& state)
{
	state["value"] = "test";
	TEST_CHECK(state("assert(value == 'test')"));
};
KAGUYA_TEST_FUNCTION_DEF(table_set)(kaguya::State& state)
{
	state["value"] = kaguya::NewTable();
	state["value"]["abc"] = kaguya::NewTable();
	state["value"]["abc"]["def"] = 7;
	state["value"]["abc"]["bbb"] = "test";
	TEST_CHECK(state("assert(value.abc.def == 7 and value.abc.bbb == 'test')"));
};

enum testenum
{
	Foo = 0,
	Bar = 1,
};

KAGUYA_TEST_FUNCTION_DEF(enum_set)(kaguya::State& state)
{
	state["value"] = Foo;
	TEST_CHECK(state("assert(value == 0)"));
};
KAGUYA_TEST_FUNCTION_DEF(enum_get)(kaguya::State& state)
{
	state("value = 1");
	TEST_CHECK(state["value"] == Bar);
};


KAGUYA_TEST_FUNCTION_DEF(minmax_overflow_check)(kaguya::State& state)
{
	state["value"] = std::numeric_limits<float>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<float>::min());
	state["value"] = std::numeric_limits<float>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<float>::max());

	state["value"] = std::numeric_limits<double>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<double>::min());
	state["value"] = std::numeric_limits<double>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<double>::max());


	state["value"] = std::numeric_limits<kaguya::luaInt>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<kaguya::luaInt>::min());
	state["value"] = std::numeric_limits<kaguya::luaInt>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<kaguya::luaInt>::max());


}


KAGUYA_TEST_GROUP_END(test_01_primitive)