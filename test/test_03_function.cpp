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

struct Foo
{
	std::string bar;
	void setBar(std::string b) { bar = b; }
};

KAGUYA_TEST_FUNCTION_DEF(member_function_test)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addMember("setBar", &Foo::setBar)
		.addCodeChunkResult("luafunc", "return function(a,b) return a*b end")
		);
	Foo foo;
	kaguya::LuaRef lfoo = state["Foo"];
	lfoo["bar"] = kaguya::function(&Foo::setBar);
	lfoo["bar"](&foo, "bar");
	TEST_EQUAL(foo.bar, "bar");

	state["foo"] = &foo;

	state("foo:setBar('test')");
	TEST_EQUAL(foo.bar, "test");

	TEST_CHECK(state("assert(foo.luafunc(3,4) == 12)"));


	(state["foo"]->*"setBar")("test2");
	TEST_EQUAL(foo.bar, "test2");
#if KAGUYA_USE_CPP11
	lfoo["bar"] = kaguya::function<void(std::string)>([&foo](std::string str) { foo.bar = str; });
	TEST_CHECK(state("Foo.bar('test3')"));
	TEST_EQUAL(foo.bar, "test3");
	lfoo["bar"] = kaguya::function([&foo](std::string str) { foo.bar = str; });
	TEST_CHECK(state("Foo.bar('test4')"));
	TEST_EQUAL(foo.bar, "test4");
#else
#endif
}

struct VariFoo
{
	std::vector<kaguya::LuaRef> args;

	VariFoo() {}
	VariFoo(kaguya::VariadicArgType a) :args(a) {

	}

	void variadic_arg_func(kaguya::VariadicArgType args)
	{
		this->args = args;
	}

};
KAGUYA_TEST_FUNCTION_DEF(variadic_function_test)(kaguya::State& state)
{
	state["Vari"].setClass(kaguya::ClassMetatable<VariFoo>()
		.addConstructor()
		.addConstructorVariadicArg()
		.addMember("variadicfun", &VariFoo::variadic_arg_func)
		);
	state("var = Vari.new()");
	state("var:variadicfun('hanaregumi','hana-uta',5)");;
	VariFoo* ptr = state["var"];
	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "hanaregumi");
	TEST_EQUAL(ptr->args[1].get<std::string>(), "hana-uta");
	TEST_EQUAL(ptr->args[2].get<int>(), 5);

	bool typevalid;
	TEST_EQUAL(ptr->args[0].get<std::string>(typevalid), "hanaregumi");
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[1].get<std::string>(typevalid), "hana-uta");
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[2].get<int>(typevalid), 5);
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[2].get<std::string>(typevalid), "5");
	TEST_CHECK(typevalid);
	ptr->args[2].get<std::string>(typevalid, false);
	TEST_CHECK(!typevalid);


	state("var = Vari.new('abc')");
	ptr = state["var"];

	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "abc");

	state("var = Vari.new('abc', 'def')");
	ptr = state["var"];

	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "abc");
	TEST_EQUAL(ptr->args[1].get<std::string>(), "def");
}



kaguya::standard::tuple<int, std::string> tuplefun()
{
	return kaguya::standard::tuple<int, std::string>(12, "23");
}

void multireturn_pass_through_to_arg(int a, int b, int c, int d, int e)
{
	TEST_EQUAL(a, 1);
	TEST_EQUAL(b, 2);
	TEST_EQUAL(c, 4);
	TEST_EQUAL(d, 8);
	TEST_EQUAL(e, 16);
}

KAGUYA_TEST_FUNCTION_DEF(multi_return_function_test)(kaguya::State& state)
{
	state("multresfun =function() return 1,2,4,8,16 end");
	int a, b, c, d, e;
	a = b = c = d = e = 0;
	kaguya::tie(a, b, c, d, e) = state["multresfun"]();
	TEST_EQUAL(a, 1);
	TEST_EQUAL(b, 2);
	TEST_EQUAL(c, 4);
	TEST_EQUAL(d, 8);
	TEST_EQUAL(e, 16);


	a = b = c = d = e = 0;
	kaguya::tie(a, b, c, d, e) = state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >();
	TEST_EQUAL(a, 1);
	TEST_EQUAL(b, 2);
	TEST_EQUAL(c, 4);
	TEST_EQUAL(d, 8);
	TEST_EQUAL(e, 16);

	using kaguya::standard::get;
	kaguya::standard::tuple<int, int, int, int, int> tuple_res = state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >();
	TEST_EQUAL(get<0>(tuple_res), 1);
	TEST_EQUAL(get<1>(tuple_res), 2);
	TEST_EQUAL(get<2>(tuple_res), 4);
	TEST_EQUAL(get<3>(tuple_res), 8);
	TEST_EQUAL(get<4>(tuple_res), 16);

	state["multireturn_pass_through_to_arg"] = kaguya::function(multireturn_pass_through_to_arg);
	state["multireturn_pass_through_to_arg"](state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >());
	state["multireturn_pass_through_to_arg"](state["multresfun"]());

	state["multresfun2"] = kaguya::function(tuplefun);

	state("a,b = multresfun2()");
	TEST_CHECK(state("assert(a == 12  and b == '23')"));
}

KAGUYA_TEST_FUNCTION_DEF(vector_and_map_from_table_mapping)(kaguya::State& state)
{
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
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
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	state("tablefn =function() return {a=32,b=1,c=2} end");
	std::map<std::string, int> m = state["tablefn"]();

	TEST_EQUAL(m["a"], 32);
	TEST_EQUAL(m["b"], 1);
	TEST_EQUAL(m["c"], 2);
	TEST_CHECK(!state["tablefn"]().typeTest<std::vector<int> >());
#endif
}

KAGUYA_TEST_FUNCTION_DEF(vector_and_map_to_table_mapping)(kaguya::State& state)
{
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	std::vector<double> v; v.push_back(3); v.push_back(13); v.push_back(2); v.push_back(99);
	state["v"] = v;
	TEST_CHECK(state("assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)"));

#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	std::map<std::string, double> m; m["a"] = 4; m["b"] = 32; m["c"] = 24;
	state["m"] = m;
	TEST_CHECK(state("assert(m['a'] == 4 and m['b'] == 32 and m['c'] == 24)"));
#endif
}



KAGUYA_TEST_FUNCTION_DEF(coroutine)(kaguya::State& state)
{
	{
		TEST_CHECK(state("cor = coroutine.create( function()"
			"coroutine.yield(32) "
			"coroutine.yield(53) "
			"return 2 "
			" end)"));

		kaguya::LuaRef cor = state["cor"];
		int r1 = cor();
		int r2 = cor();
		int r3 = cor();

		TEST_EQUAL(r1, 32);
		TEST_EQUAL(r2, 53);
		TEST_EQUAL(r3, 2);

		TEST_CHECK(state("cor3 = coroutine.create( function(arg)"
			"coroutine.yield(arg) "
			"coroutine.yield(arg*2) "
			"coroutine.yield(arg*3) "
			"return arg*4 "
			" end)"));

		kaguya::LuaThread cor3 = state["cor3"];
		std::vector<int> results;
		while (!cor3.isThreadDead())
		{
			results.push_back(cor3(3));
		}

		TEST_EQUAL(results.size(), 4);
		TEST_EQUAL(results[0], 3);
		TEST_EQUAL(results[1], 6);
		TEST_EQUAL(results[2], 9);
		TEST_EQUAL(results[3], 12);
	}
	{
		kaguya::LuaThread cor2 = state.newRef(kaguya::NewThread());
		TEST_CHECK(state("corfun = function(arg)"
			"coroutine.yield(arg) "
			"coroutine.yield(arg*2) "
			"coroutine.yield(arg*3) "
			"return arg*4 "
			" end"));

		kaguya::LuaFunction corfun = state["corfun"];
		int r1 = cor2(corfun, 3);
		int r2 = cor2.resume<int>();
		int r3 = cor2.resume<int>();
		int r4 = cor2.resume<int>();

		TEST_EQUAL(r1, 3);
		TEST_EQUAL(r2, 6);
		TEST_EQUAL(r3, 9);
		TEST_EQUAL(r4, 12);


		cor2.resume<void>(corfun, 3);
	}
	{

		state["cor2"] = kaguya::NewThread();
		kaguya::LuaRef cor2 = state["cor2"];
		TEST_CHECK(state("corfun = function(arg)"
			"for i = 1,arg do "
			"coroutine.yield() "
			"end "
			"end"));
		kaguya::LuaRef corfun = state["corfun"];
		cor2(corfun, 10);
		int yieldnum = 0;
		while (cor2.threadStatus() == LUA_YIELD)
		{
			cor2();
			yieldnum++;
		}


		TEST_EQUAL(yieldnum, 10);
	}
}

void corresult_to_main(kaguya::VariadicArgType args)
{
	TEST_EQUAL(args.size(), 9);
	TEST_EQUAL(args[0], 1);
	TEST_EQUAL(args[1], 2);
	TEST_EQUAL(args[2], 3);
	TEST_EQUAL(args[3], 4);
	TEST_EQUAL(args[4], 5);
	TEST_EQUAL(args[5], 6);
	TEST_EQUAL(args[6], 7);
	TEST_EQUAL(args[7], 8);
	TEST_EQUAL(args[8], 9);
}
void corresult_to_main2(kaguya::VariadicArgType args)
{
	TEST_EQUAL(args.size(), 1);
	TEST_EQUAL(args[0], 6);
}


KAGUYA_TEST_FUNCTION_DEF(coroutine_stack)(kaguya::State& state)
{
	state["corresult_to_main"] = &corresult_to_main;
	state["cor2"] = kaguya::NewThread();
	kaguya::LuaRef cor2 = state["cor2"];
	TEST_CHECK(state("corfun = function(arg)"
		"for i = 1,arg do "
		"coroutine.yield(1,2,3,4,5,6,7,8,9) "
		"end "
		"end"));
	state["corresult_to_main"](cor2(state["corfun"], 10));


	state["corresult_to_main2"] = &corresult_to_main2;
	state["corresult_to_main2"](cor2(state["corfun"], 10).result_at(5));
}


void pointerfun(VariFoo* pointer)
{
	TEST_EQUAL(pointer, 0);
}
void const_pointerfun(const VariFoo* pointer)
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

void reffun(Foo& ref)
{
	ref.setBar("BarBar");
}
KAGUYA_TEST_FUNCTION_DEF(arg_class_ref)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addMember("setBar", &Foo::setBar)
		);

	Foo foo;
	state["reffun"] = kaguya::function(reffun);

	state["reffun"](kaguya::standard::ref(foo));
	TEST_EQUAL(foo.bar, "BarBar");
}

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


KAGUYA_TEST_FUNCTION_DEF(overload)(kaguya::State& state)
{
	state["overloaded_function"] = kaguya::overload(overload1, overload2, overload3
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		, overload4
#endif
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		, overload5
		, overload6
#endif
		, overload7
		);
	kaguya::LuaFunction f = state["overloaded_function"];
	TEST_EQUAL(f(), 1);
	TEST_EQUAL(f(""), 2);
	TEST_EQUAL(f(121), 3);

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	TEST_CHECK(state("assert(overloaded_function({3,4,2,4,5}) == 4)"));
#endif
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	TEST_CHECK(state("assert(overloaded_function({a='3',b='3'}) == 5)"));
	TEST_CHECK(state("assert(overloaded_function({a='3',b='3'}, 6) == 6)"));
#endif
	TEST_CHECK(state("assert(overloaded_function(nil) == 7)"));
	TEST_EQUAL(f((void*)0), 7);

}


KAGUYA_TEST_FUNCTION_DEF(result_to_table)(kaguya::State& state)
{
	state["result_to_table"] = kaguya::function(overload1);
	state["result"] = state["result_to_table"]();
	TEST_EQUAL(state["result"], 1);
}

KAGUYA_TEST_GROUP_END(test_03_function)
