#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_06_state)

using namespace kaguya_test_util;


int error_count = 0;
void error_fun(int status, const char* message)
{
	error_count++;
}
KAGUYA_TEST_FUNCTION_DEF(set_error_function)(kaguya::State& state)
{
	error_count = 0;
	state.setErrorHandler(error_fun);
	TEST_CHECK(!state("awdorgkwl;gw"));

	TEST_CHECK(error_count == 1);
	state["yy"]["yy"]["yy"]();
	TEST_CHECK(error_count > 0);
}
KAGUYA_TEST_FUNCTION_DEF(function_call_error)(kaguya::State& state)
{
	error_count = 0;
	state.setErrorHandler(error_fun);

	state["errofun"] = kaguya::function(error_fun);
	state["errofun"](33);

	TEST_CHECK(error_count == 1);

	kaguya::LuaRef f;
	f.resume<void>();
	f.call<void>();
	TEST_COMPARE_EQ(f.threadStatus(), LUA_ERRRUN);
	TEST_COMPARE_EQ(state.newRef(1).threadStatus(), LUA_ERRRUN);
}

KAGUYA_TEST_FUNCTION_DEF(other_state)(kaguya::State& unused)
{
	lua_State* L = luaL_newstate();

	kaguya::State state(L);
	kaguya::State state2(L);
	kaguya::State state3(L);
	kaguya::State state4(L);
	kaguya::State state5(L);

	state5["value"] = 1;
	TEST_EQUAL(state["value"], 1);
	TEST_EQUAL(state5["value"], state2["value"]);

	lua_close(L);
}

void ignore_error_fun(int status, const char* message)
{
}
KAGUYA_TEST_FUNCTION_DEF(load_string)(kaguya::State& state)
{
	kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
	TEST_CHECK(luafun() != false);
	state.setErrorHandler(ignore_error_fun);
	kaguya::LuaRef errorref = state.loadstring("function() e");//syntax error
	TEST_CHECK(!errorref);
}


KAGUYA_TEST_FUNCTION_DEF(load_with_other_env)(kaguya::State& state)
{
	state.dostring("foo = 'bar'");

	state["otherEnv"] = kaguya::NewTable();

	state.dostring("foo = 'dar'", state["otherEnv"]);

	TEST_CHECK(state("assert(foo == 'bar')"));

	TEST_CHECK(state("assert(otherEnv.foo == 'dar')"));
}
KAGUYA_TEST_FUNCTION_DEF(no_standard_lib)(kaguya::State&)
{
	kaguya::State state(kaguya::NoLoadLib());
	TEST_CHECK(!state("assert(true)"));//can not call assert
}
KAGUYA_TEST_FUNCTION_DEF(load_lib_constructor)(kaguya::State&)
{
	{
		kaguya::LoadLibs libs;
		libs.push_back(kaguya::LoadLib("_G", luaopen_base));
		libs.push_back(kaguya::LoadLib(LUA_MATHLIBNAME, luaopen_math));

		kaguya::State state(libs);
		TEST_CHECK(state("assert(true)"));
		TEST_CHECK(state("assert(math.abs(-2) == 2)"));
	}
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER>=1800)
	{//use c++11 initializer list
		kaguya::State state({ { "_G", luaopen_base },{ LUA_MATHLIBNAME, luaopen_math } });
		TEST_CHECK(state("assert(true)"));
		TEST_CHECK(state("assert(math.abs(-2) == 2)"));
	}
#endif
}

bool errorOccurred = false;
std::string lastMsg;
void registerError(int status, const char* message)
{
	if (errorOccurred)
		throw std::runtime_error("Should have reset errorOccurred");
	errorOccurred = true;
	lastMsg = message;
}

struct Foo {};

void foobar1(int) { throw std::runtime_error("MyRuntimeError"); }
void foobar2(int, int) {}
void foobar3(int, Foo, int) {}

KAGUYA_TEST_FUNCTION_DEF(errorThrowing)(kaguya::State& state)
{
	state["foobar"] = kaguya::overload(foobar1, foobar2, foobar3);
	state["Foo"].setClass(kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());
	state.setErrorHandler(registerError);
	TEST_CHECK(!state("foobar()"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);
	lastMsg = lastMsg.substr(lastMsg.find("candidate is:\n"));
	lastMsg = lastMsg.substr(0, lastMsg.find("stack "));
	std::vector<std::string> parts = remove_empty(split(lastMsg, '\n'));
	TEST_EQUAL(parts.size(), 4);
	std::string intName = typeid(int).name();
	TEST_CHECK(parts[1].find(intName) != std::string::npos);
	TEST_CHECK(parts[2].find(intName + "," + intName) != std::string::npos);
	TEST_CHECK(parts[3].find(intName + "," + typeid(Foo).name() + "," + intName) != std::string::npos);

	errorOccurred = false;
	TEST_CHECK(!state("foobar(Foo.new(), 1, 1)"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);

	errorOccurred = false;
	TEST_CHECK(!state("foobar(1)"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("MyRuntimeError") != std::string::npos);

#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	errorOccurred = false;
	state["noTable"] = 5;
	std::map<int, int> map = state["noTable"];
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("type mismatch") != std::string::npos);

	errorOccurred = false;
	state["noTable"] = 5;
	std::vector<int> vec = state["noTable"];
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("type mismatch") != std::string::npos);
#endif
}

KAGUYA_TEST_GROUP_END(test_06_state)
