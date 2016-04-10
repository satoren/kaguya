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
	TEST_EQUAL(state["value"] , true);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == true)"));
	state["value"] = false;
	TEST_EQUAL(state["value"], false);
	TEST_CHECK(!state["value"]);
	TEST_CHECK(state("assert(value == false)"));
};
KAGUYA_TEST_FUNCTION_DEF(int_set)(kaguya::State& state)
{
	state["value"] = 3;
	TEST_EQUAL(state["value"], 3);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 3)"));
	state["value"] = 0;
	TEST_EQUAL(state["value"], 0);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 0)"));
};
KAGUYA_TEST_FUNCTION_DEF(short_set)(kaguya::State& state)
{
	state["value"] = short(3);
	TEST_EQUAL(state["value"], short(3));
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 3)"));
	state["value"] = short(0);
	TEST_EQUAL(state["value"], short(0));
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 0)"));
};

KAGUYA_TEST_FUNCTION_DEF(char_set)(kaguya::State& state)
{
	state["value"] = char(3);
	TEST_EQUAL(state["value"], char(3));
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 3)"));
	state["value"] = char(0);
	TEST_EQUAL(state["value"], char(0));
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 0)"));
};
KAGUYA_TEST_FUNCTION_DEF(float_set)(kaguya::State& state)
{
	state["value"] = 5.5f;
	TEST_EQUAL(state["value"], 5.5f);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 5.5,value)"));
};

KAGUYA_TEST_FUNCTION_DEF(double_set)(kaguya::State& state)
{
	state["value"] = 5.5;
	TEST_EQUAL(state["value"], 5.5);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 5.5,value)"));
};


KAGUYA_TEST_FUNCTION_DEF(string_set)(kaguya::State& state)
{
	state["value"] = "test";
	TEST_EQUAL(state["value"], "test");
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 'test')"));
	state["value"] = "test2";
	TEST_EQUAL(state["value"], "test2");
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 'test2')"));
	state["value"] = "";
	TEST_EQUAL(state["value"], "");
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == '')"));

	state["value"] = "false";
	TEST_EQUAL(state["value"], "false");
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 'false')"));

	state["value"] = std::string("test");
	TEST_EQUAL(state["value"], std::string("test"));
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
KAGUYA_TEST_FUNCTION_DEF(nullptr_set)(kaguya::State& state)
{
	state["value"] = (void*)0;
	TEST_EQUAL(state["value"], 0);
	TEST_EQUAL(state["value"], (void*)0);
	TEST_CHECK(!state["value"]);
	TEST_CHECK(state("assert(value == nil)"));
};

enum testenum
{
	Foo = 0,
	Bar = 1,
};

KAGUYA_TEST_FUNCTION_DEF(enum_set)(kaguya::State& state)
{
	state["value"] = Foo;
	TEST_EQUAL(state["value"], Foo);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 0)"));
};
KAGUYA_TEST_FUNCTION_DEF(enum_get)(kaguya::State& state)
{
	state("value = 1");
	TEST_EQUAL(state["value"], Bar);
	TEST_CHECK(state["value"]);
	TEST_CHECK(state("assert(value == 1)"));
};


KAGUYA_TEST_FUNCTION_DEF(minmax_overflow_check)(kaguya::State& state)
{
	state["value"] = std::numeric_limits<float>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<float>::min());
	state["value"] = std::numeric_limits<float>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<float>::max());

	state["value"] = std::numeric_limits<lua_Number>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<lua_Number>::min());
	state["value"] = std::numeric_limits<lua_Number>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<lua_Number>::max());

	state["value"] = std::numeric_limits<kaguya::luaInt>::min();
	TEST_EQUAL(state["value"], std::numeric_limits<kaguya::luaInt>::min());
	state["value"] = std::numeric_limits<kaguya::luaInt>::max();
	TEST_EQUAL(state["value"], std::numeric_limits<kaguya::luaInt>::max());
}


KAGUYA_TEST_FUNCTION_DEF(map_set)(kaguya::State& state)
{
	std::map<std::string, int> setmap;
	setmap["3"] = 23232;
	setmap["4"] = 232;
	setmap["5"] = 23;
	state["value"] = setmap;
	TEST_CHECK(state["value"]);
	TEST_CHECK(state["value"] == setmap);
}

KAGUYA_TEST_FUNCTION_DEF(vector_set)(kaguya::State& state)
{
	std::vector<std::string> setvec;
	setvec.push_back("342");
	setvec.push_back("32");
	state["value"] = setvec;
	TEST_CHECK(state["value"]);
	TEST_CHECK(state["value"] == setvec);
}
KAGUYA_TEST_FUNCTION_DEF(vector_set2)(kaguya::State& state)
{
	std::vector<std::vector<std::string> > setvec;
	setvec.push_back(std::vector<std::string>(12, "332"));
	setvec.push_back(std::vector<std::string>(42, "3232"));
	state["value"] = setvec;
	TEST_CHECK(state["value"]);
	TEST_CHECK(state["value"] == setvec);
}


KAGUYA_TEST_GROUP_END(test_01_primitive)
