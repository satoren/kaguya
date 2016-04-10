#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_07_vector_map_to_luatable)


#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
KAGUYA_TEST_FUNCTION_DEF(vector_from_table)(kaguya::State& state)
{
	state("arraytablefn =function() return {32,1,2,4,8,16} end");
	std::vector<int> b = state["arraytablefn"]();
	TEST_EQUAL(b.size(), 6);
	TEST_EQUAL(b[0], 32);
	TEST_EQUAL(b[1], 1);
	TEST_EQUAL(b[2], 2);
	TEST_EQUAL(b[3], 4);
	TEST_EQUAL(b[4], 8);
	TEST_EQUAL(b[5], 16);
	TEST_CHECK(state["arraytablefn"]().typeTest<std::vector<int> >());
}

KAGUYA_TEST_FUNCTION_DEF(vector_to_table)(kaguya::State& state)
{
	std::vector<double> v; v.push_back(3); v.push_back(13); v.push_back(2); v.push_back(99);
	state["v"] = v;
	TEST_CHECK(state("assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)"));

}
#endif

#ifndef KAGUYA_NO_STD_MAP_TO_TABLE


KAGUYA_TEST_FUNCTION_DEF(map_from_table)(kaguya::State& state)
{
	state("tablefn =function() return {a=32,b=1,c=2} end");
	std::map<std::string, int> m = state["tablefn"]();

	TEST_EQUAL(m["a"], 32);
	TEST_EQUAL(m["b"], 1);
	TEST_EQUAL(m["c"], 2);
	TEST_CHECK(!state["tablefn"]().typeTest<std::vector<int> >());
}

KAGUYA_TEST_FUNCTION_DEF(map_to_table)(kaguya::State& state)
{
	std::map<std::string, double> m; m["a"] = 4; m["b"] = 32; m["c"] = 24;
	state["m"] = m;
	TEST_CHECK(state("assert(m['a'] == 4 and m['b'] == 32 and m['c'] == 24)"));

}
std::map<unsigned, unsigned> myMap;
int myVal;

class FooClass {
public:
	void mapCallback(const std::map<unsigned, unsigned>& map)
	{
		myMap = map;
	}
	void mapOverload()
	{
		myMap.clear();
	}
	void mapOverload(const std::map<unsigned, unsigned>& map)
	{
		myMap = map;
	}
	void mapOverload(const std::map<unsigned, unsigned>& map, int val)
	{
		myMap = map;
		myVal = val;
	}
};

KAGUYA_TEST_FUNCTION_DEF(testWrongClassUseWihMap)(kaguya::State& state)
{
	state["testMap"].setClass(kaguya::ClassMetatable<FooClass>()
		.addMemberFunction("testMap", &FooClass::mapCallback)
		.addMemberFunction("mapOverload", static_cast<void (FooClass::*)()>(&FooClass::mapOverload))
		.addMemberFunction("mapOverload", static_cast<void (FooClass::*)(const std::map<unsigned, unsigned>&)>(&FooClass::mapOverload))
		.addMemberFunction("mapOverload", static_cast<void (FooClass::*)(const std::map<unsigned, unsigned>&, int)>(&FooClass::mapOverload))
		);
	state["foo"] = FooClass();
	state("myMap = {[1] = 2, [3] = 4, [5] = 6}");
	try {
		// This uses an invalid function call which should throw an error
		state("foo.testMap(myMap)");
		TEST_CHECK(false);
	}
	catch (std::runtime_error&)
	{
	}
	TEST_CHECK(myMap.empty());
	// This is the correct call
	state("foo:testMap(myMap)");
	TEST_EQUAL(myMap[1], 2);
	TEST_EQUAL(myMap[3], 4);
	TEST_EQUAL(myMap[5], 6);
	state("foo:mapOverload()");
	TEST_CHECK(myMap.empty());
	try {
		// This uses an invalid function call which should throw an error
		state("foo.mapOverload(myMap)");
		TEST_CHECK(false);
	}
	catch (std::runtime_error&)
	{
	}
	state("foo:mapOverload(myMap)");
	TEST_EQUAL(myMap[1], 2);
	TEST_EQUAL(myMap[3], 4);
	TEST_EQUAL(myMap[5], 6);
	myMap.clear();
	state("foo:mapOverload(myMap, 42)");
	TEST_EQUAL(myMap[1], 2);
	TEST_EQUAL(myMap[3], 4);
	TEST_EQUAL(myMap[5], 6);
	TEST_EQUAL(myVal, 42);
}

#endif

KAGUYA_TEST_GROUP_END(test_07_vector_map_to_luatable)
