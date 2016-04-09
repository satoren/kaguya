#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_07_any_type_test)
enum testenum
{
	Foo = 0,
	Bar = 1,
};


template<class T, class T2> bool setget_eq(kaguya::State& state, T setvalue, T2 condvalue)
{
	return state.newRef(setvalue) == condvalue;
}

template< class T2> struct callback_function
{

	T2 v_;
	callback_function(T2& v) :v_(v) {}

	bool operator()(T2 v)
	{
		return v_ == v;
	}
};
template<class T, class T2> bool function_call(kaguya::State& state, T setvalue, T2 condvalue)
{
	callback_function<T2> fun(condvalue);
	state["testfn"] = kaguya::function<bool(T2)>(fun);
	state["value"] = setvalue;
	return state("assert(testfn(value))");
}

template<class T, class T2>
bool test(kaguya::State& state, T value1, T2 value2)
{
	return setget_eq(state, value1, value2) && function_call(state, value1, value2);
}
KAGUYA_TEST_FUNCTION_DEF(any_type_test)(kaguya::State& state)
{
	TEST_CHECK(test(state, true, true));
	TEST_CHECK(test(state, 0, 0));
	TEST_CHECK(test(state, "", std::string("")));
	TEST_CHECK(test(state, "false", std::string("false")));
	TEST_CHECK(test(state, std::string(""), std::string("")));
	TEST_CHECK(test(state, 1, 1));
	TEST_CHECK(test(state, kaguya::LuaRef(), kaguya::LuaRef()));
	TEST_CHECK(test(state, Foo, Foo));
	TEST_CHECK(test(state, Bar, Bar));

	TEST_CHECK(test(state, kaguya::LuaRef(), false));
	TEST_CHECK(test(state, (void*)0, false));
	TEST_CHECK(test(state, 0, true));
	TEST_CHECK(test(state, "", true));
	TEST_CHECK(test(state, "false", true));
	TEST_CHECK(test(state, std::string(""), true));
	TEST_CHECK(test(state, 1, true));
	TEST_CHECK(test(state, std::numeric_limits<float>::min(), std::numeric_limits<float>::min()));
	TEST_CHECK(test(state, std::numeric_limits<double>::min(), std::numeric_limits<double>::min()));
	TEST_CHECK(test(state, std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
	TEST_CHECK(test(state, std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
	TEST_CHECK(test(state, std::numeric_limits<kaguya::luaInt>::min(), std::numeric_limits<kaguya::luaInt>::min()));
	TEST_CHECK(test(state, std::numeric_limits<kaguya::luaInt>::max(), std::numeric_limits<kaguya::luaInt>::max()));
	TEST_CHECK(test(state, short(5), short(5)));
	TEST_CHECK(test(state, 1, state.newRef(1)));
	TEST_CHECK(test(state, "test", state.newRef("test")));
	TEST_CHECK(test(state, false, state.newRef(false)));
	TEST_CHECK(test(state, 5.674f, state.newRef(5.674f)));
	TEST_CHECK(test(state, 5.674, state.newRef(5.674)));
	TEST_CHECK(test(state, state.newRef(5.674f), 5.674f));
	TEST_CHECK(test(state, state.newRef(5.674), 5.674));
	TEST_CHECK(test(state, state.newRef(short(5)), short(5)));
	TEST_CHECK(test(state, state.newRef(long(5)), long(5)));

	{
		std::map<std::string, int> setmap;
		setmap["3"] = 23232;
		setmap["4"] = 232;
		setmap["5"] = 23;
		std::map<std::string, int> copy = setmap;
		TEST_CHECK(test(state, setmap, copy));
	}
	{
		std::vector<std::string> setvec;
		setvec.push_back("342");
		setvec.push_back("32");
		std::vector<std::string> copy = setvec;
		TEST_CHECK(test(state, setvec, copy));
	}
	{
		std::vector<std::vector<std::string> > setvec;
		setvec.push_back(std::vector<std::string>(12, "332"));
		setvec.push_back(std::vector<std::string>(42, "3232"));
		std::vector<std::vector<std::string> > copy = setvec;
		TEST_CHECK(test(state, setvec, copy));
	}
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

KAGUYA_TEST_GROUP_END(test_07_any_type_test)