#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_03_function)
using namespace kaguya_test_util;

int arg = 0;
void free_standing_function(int r)
{
	arg = r;
}
int free_standing_function2()
{
	return 12;
}

KAGUYA_TEST_FUNCTION_DEF(free_standing_function_test)(kaguya::State& state)
{
	state["ABC"] = &free_standing_function;
	state["ABC"](54);
	state["free2"] = &free_standing_function2;
	TEST_EQUAL(arg, 54);
	TEST_CHECK(state["free2"]() == 12.0);
}




kaguya::standard::tuple<int, std::string> tuplefun()
{
	return kaguya::standard::tuple<int, std::string>(12, "23");
}

KAGUYA_TEST_FUNCTION_DEF(multi_return_function_test)(kaguya::State& state)
{
	state["multresfun2"] = kaguya::function(tuplefun);

	state("a,b = multresfun2()");
	TEST_CHECK(state("assert(a == 12  and b == '23')"));
}

void pointerfun(void* pointer)
{
	TEST_EQUAL(pointer, 0);
}
void const_pointerfun(const void* pointer)
{
	TEST_EQUAL(pointer, 0);
}
std::string last_error_message;
void ignore_error_fun(int status, const char* message)
{
	last_error_message = message ? message : "";
}
KAGUYA_TEST_FUNCTION_DEF(zero_to_nullpointer)(kaguya::State& state)
{
	state["pointerfun"] = kaguya::function(pointerfun);
	TEST_CHECK(state("pointerfun(0)"));
	TEST_CHECK(state("pointerfun(nil)"));
	state["const_pointerfun"] = kaguya::function(const_pointerfun);
	TEST_CHECK(state("const_pointerfun(0)"));
	TEST_CHECK(state("const_pointerfun(nil)"));

	state.setErrorHandler(ignore_error_fun);
	TEST_CHECK(!state("pointerfun(32)"));// is error
	TEST_CHECK(!state("pointerfun('232')"));// is error
}
KAGUYA_TEST_FUNCTION_DEF(noargs_to_nullpointer)(kaguya::State& state)
{
	state["pointerfun"] = kaguya::function(pointerfun);
	TEST_CHECK(state("pointerfun()"));
	state["const_pointerfun"] = kaguya::function(const_pointerfun);
	TEST_CHECK(state("const_pointerfun()"));
}


struct Foo
{
	std::string bar;
	void setBar(std::string b) { bar = b; }
};
struct Bar
{
	std::string member;
	Bar() {}
	Bar(const std::string& data) :member(data) {}
};
KAGUYA_TEST_FUNCTION_DEF(native_function_call_test)(kaguya::State& state)
{
	using namespace kaguya::nativefunction;
	Foo foo;
	state.newRef(6).push();
	state.newRef(9).push();
	state.newRef(2).push();

	call(state.state(), &free_standing_function);

	lua_settop(state.state(), 0);
	state.newRef(&foo).push();
	state.newRef("Bar").push();
	call(state.state(), &Foo::setBar);
#if KAGUYA_USE_CPP11
	state.newRef(&foo).push();
	state.newRef(9).push();
	call(state.state(), [](Foo* foo, int b) {
		foo->setBar("fromlambda");
	});
	TEST_EQUAL(foo.bar, "fromlambda");

	std::string capture("capture");
	call(state.state(), [=](Foo* foo, int b) {
		foo->setBar(capture + "lambda");
	});
	TEST_EQUAL(foo.bar, "capturelambda");
#endif

	lua_settop(state.state(), 0);
}

int overload1()
{
	return 1;
}
int overload2(const std::string&)
{
	return 2;
}
int overload3(int)
{
	return 3;
}

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
int overload4(const std::vector<int>& a)
{
	return 4;
}
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE	
int overload5(const std::map<std::string, std::string>& a)
{
	return 5;
}
// This function after the first to trigger weak type test
int overload6(const std::map<std::string, std::string>& a, int b)
{
	return b;
}
#endif
int overload7(void*)
{
	return 7;
}

int overload8(Foo*)
{
	return 8;
}
int overload9(Bar*)
{
	return 9;
}
int overload10(const kaguya::standard::function<int()> fn)
{
	TEST_EQUAL(fn() , 1);
	return 10;
}

KAGUYA_TEST_FUNCTION_DEF(overload)(kaguya::State& state)
{
	state["overloaded_function"] = kaguya::overload(overload1, overload2, overload3
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		, overload4
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
		, overload5
		, overload6
#endif
		, overload7
		, overload8
		, overload9
		);
	kaguya::LuaFunction f = state["overloaded_function"];
	TEST_EQUAL(f(), 1);
	TEST_EQUAL(f(""), 2);
	TEST_EQUAL(f(121), 3);

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	TEST_CHECK(state("assert(overloaded_function({3,4,2,4,5}) == 4)"));
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	TEST_CHECK(state("assert(overloaded_function({a='3',b='3'}) == 5)"));
	TEST_CHECK(state("assert(overloaded_function({a='3',b='3'}, 6) == 6)"));
#endif
	TEST_CHECK(state("assert(overloaded_function(nil) == 7)"));
	TEST_EQUAL(f((void*)0), 7);


	state["Foo"].setClass(kaguya::UserdataMetatable<Foo>());
	state["Bar"].setClass(kaguya::UserdataMetatable<Bar>());

	TEST_EQUAL(f(Foo()), 8);
	TEST_EQUAL(f(Bar()), 9);



	state["overloaded_function2"] = kaguya::overload(overload1, overload2, overload3
		, overload10
	);
	kaguya::LuaFunction f2 = state["overloaded_function2"];
	TEST_EQUAL(f2(kaguya::standard::function<int()>(overload1)), 10);
}


KAGUYA_TEST_FUNCTION_DEF(result_to_table)(kaguya::State& state)
{
	state["result_to_table"] = kaguya::function(overload1);
	state["result"] = state["result_to_table"]();
	TEST_EQUAL(state["result"], 1);
}

void nested_function_call(kaguya::LuaStackRef function, kaguya::LuaStackRef value, kaguya::LuaStackRef value2)
{
	function(value, value2);
}

KAGUYA_TEST_FUNCTION_DEF(nested_function_call_test)(kaguya::State& state)
{
	state["nested_function_call"] = kaguya::function(nested_function_call);

	state("nested_function_call(function(value,value2) assert(value == 32);assert(value2 == 'text') end,32,'text')");
}

void nested_function_call2(kaguya::LuaStackRef function,const kaguya::LuaStackRef& table, kaguya::LuaStackRef value2)
{
	function(table["key"], value2);
}

KAGUYA_TEST_FUNCTION_DEF(nested_function_call_test2)(kaguya::State& state)
{
	state["nested_function_call"] = kaguya::function(nested_function_call2);

	state("nested_function_call(function(value,value2) assert(value==32);assert(value2 == 'text') end,{key=32},'text')");
}


KAGUYA_TEST_GROUP_END(test_03_function)
