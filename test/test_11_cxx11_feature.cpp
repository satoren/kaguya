#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

#if KAGUYA_USE_CPP11


KAGUYA_TEST_GROUP_START(test_11_cxx11_feature)

using namespace kaguya_test_util;

enum class testenumclass
{
	Foo = 0,
	Bar = 1,
};


KAGUYA_TEST_FUNCTION_DEF(enum_class_set)(kaguya::State& state)
{
	state["value"] = testenumclass::Foo;
	TEST_CHECK(state("assert(value == 0)"));
};
KAGUYA_TEST_FUNCTION_DEF(enum_class_get)(kaguya::State& state)
{
	state("value = 1");
	TEST_CHECK(state["value"] == testenumclass::Bar);
};



struct MoveOnlyClass
{
	MoveOnlyClass(int i) :member(i) {}
	int member;

	MoveOnlyClass(MoveOnlyClass&& src) :member(src.member) {}
private:
	MoveOnlyClass();
	MoveOnlyClass(const MoveOnlyClass&);
	MoveOnlyClass& operator=(const MoveOnlyClass&);
};
KAGUYA_TEST_FUNCTION_DEF(movable_class_test)(kaguya::State& state)
{
	state["MoveOnlyClass"].setClass(kaguya::UserdataMetatable<MoveOnlyClass>()
		.setConstructors<MoveOnlyClass(int)>()
		.addProperty("member", &MoveOnlyClass::member)
		);

	state["moveonly"] = MoveOnlyClass(2);

	const MoveOnlyClass* ref = state["moveonly"];
	TEST_CHECK(ref);
	TEST_CHECK(ref->member == 2);

	state("func =function(arg) return assert(arg.member == 5) end");
	state["func"](MoveOnlyClass(5));

	state.newRef(MoveOnlyClass(5));
}

KAGUYA_TEST_FUNCTION_DEF(lambdafun)(kaguya::State& state)
{
	state["ABC"] = kaguya::function([](int a) {return a * 2; });
	int a = state["ABC"](54);
	TEST_EQUAL(a, 108);

	state["free2"] = kaguya::function([]() {return 12; });
	TEST_EQUAL(state["free2"](), 12.0);


	state["sum"] = kaguya::function([](kaguya::VariadicArgType args) {
		int sum = 0;
		for (int arg : args)
		{
			sum += arg;
		}

		TEST_EQUAL(args.size(), 10);
		TEST_EQUAL(args[0], 1);
		TEST_EQUAL(args[1], 2);
		TEST_EQUAL(args[2], 3);
		TEST_EQUAL(args[3], 4);
		TEST_EQUAL(args[4], 5);
		TEST_EQUAL(args[5], 6);
		TEST_EQUAL(args[6], 7);
		TEST_EQUAL(args[7], 8);
		TEST_EQUAL(args[8], 9);
		TEST_EQUAL(args[9], 10);
		return sum;
	});
	TEST_EQUAL(state["sum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 55);



	state["ssum"] = kaguya::function([](kaguya::VariadicArgType args) {
		std::string sum;
		for (std::string arg : args)
		{
			sum += arg;
		}
		return sum;
	});
	TEST_EQUAL(state["ssum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), "12345678910");


	state["overload"] = kaguya::overload([](int) {return 1; },
		[](const std::string&) {return 2; },
		[]() {return 3; }
	);
	state("assert(overload(2) == 1)");//int version
	state("assert(overload('2') == 2)");//string version
	state("assert(overload() == 3)");//no argument version

}

KAGUYA_TEST_FUNCTION_DEF(put_unique_ptr)(kaguya::State& state)
{
	state["MoveOnlyClass"].setClass(kaguya::UserdataMetatable<MoveOnlyClass>()
		.setConstructors<MoveOnlyClass(int)>()
		.addProperty("member", &MoveOnlyClass::member)
		);

	state["uniqueptr"] = std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(2));

	const MoveOnlyClass* ref = state["uniqueptr"];
	TEST_CHECK(ref);
	TEST_EQUAL(ref->member, 2);

	state["uniqueptr"].typeTest<const std::unique_ptr<MoveOnlyClass>&>();
	const std::unique_ptr<MoveOnlyClass>& cuptr = state["uniqueptr"].get<const std::unique_ptr<MoveOnlyClass>&>();
	TEST_CHECK(cuptr);
	TEST_EQUAL(cuptr->member, 2);

	std::unique_ptr<MoveOnlyClass>& uptr = state["uniqueptr"].get<std::unique_ptr<MoveOnlyClass>&>();
	TEST_CHECK(uptr);
	TEST_EQUAL(uptr->member, 2);
	uptr.reset();

	state("func =function(arg) return assert(arg.member == 5) end");
	TEST_CHECK(state["func"](std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(5))) == true);


	bool catch_except = false;
	try
	{
		std::unique_ptr<MoveOnlyClass>& uptr = state["nil"].get<std::unique_ptr<MoveOnlyClass>&>();
		TEST_CHECK(uptr);
		TEST_EQUAL(uptr->member, 2);
	}
	catch (const kaguya::LuaTypeMismatch&)
	{
		catch_except = true;
	}
	TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(compare_null_ptr)(kaguya::State& state)
{
	{
		kaguya::LuaRef nullref = state.newRef(nullptr);
		TEST_CHECK(nullref.typeTest<std::nullptr_t>());
		TEST_CHECK(nullref.weakTypeTest<std::nullptr_t>());
		TEST_CHECK(nullref == nullptr);
		void* ptr = nullref.get<std::nullptr_t>();
		TEST_CHECK(!ptr);
	}
	{
		kaguya::LuaRef ref = state.newRef(1);
		TEST_CHECK(!(ref.typeTest<std::nullptr_t>()));
		TEST_CHECK(!(ref.weakTypeTest<std::nullptr_t>()));
		TEST_CHECK(ref != nullptr);
		bool catch_except = false;
		try
		{
			ref.get<std::nullptr_t>();
		}
		catch (const kaguya::LuaTypeMismatch&)
		{
			catch_except = true;
		}
		TEST_CHECK(catch_except);
	}
}

KAGUYA_TEST_FUNCTION_DEF(nullptr_set)(kaguya::State& state)
{
	state["value"] = nullptr;
	TEST_CHECK(state["value"] == nullptr);
	TEST_CHECK(!state["value"]);
	TEST_CHECK(state("assert(value == nil)"));
};


KAGUYA_TEST_FUNCTION_DEF(null_unique_ptr)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::UserdataMetatable<MoveOnlyClass>());

	state["test"] =std::unique_ptr<MoveOnlyClass>();
	TEST_CHECK(state("assert(test==nil)"));
}


KAGUYA_TEST_FUNCTION_DEF(result_range_based_for)(kaguya::State& state)
{
	state("fn =function() return 1,2,4,8,16 end");
	{
		std::vector<int> res;
		kaguya::LuaFunction fn = state["fn"];
		auto result = fn();
		for (int v : result)
		{
			res.push_back(v);
		}
		TEST_EQUAL(res.size(), 5);
		TEST_EQUAL(res[0], 1);
		TEST_EQUAL(res[1], 2);
		TEST_EQUAL(res[2], 4);
		TEST_EQUAL(res[3], 8);
		TEST_EQUAL(res[4], 16);
	}
	{
		std::vector<int> res;
		kaguya::LuaFunction fn = state["fn"];
		const auto result = fn();
		for (int v : result)
		{
			res.push_back(v);
		}
		TEST_EQUAL(res.size(), 5);
		TEST_EQUAL(res[0], 1);
		TEST_EQUAL(res[1], 2);
		TEST_EQUAL(res[2], 4);
		TEST_EQUAL(res[3], 8);
		TEST_EQUAL(res[4], 16);
	}

}

KAGUYA_TEST_FUNCTION_DEF(initializer_list)(kaguya::State& state)
{
	state["table"] = kaguya::TableData{ 23,"test",{"key","value"},
	{ "childtable",kaguya::TableData{3} }};
	kaguya::LuaTable tbl = state["table"];


	state("assert(table[1] == 23)");
	state("assert(table[2] == 'test')");
	state("assert(table['key'] == 'value')");

	TEST_EQUAL(tbl[1], 23);
	TEST_EQUAL(tbl[2], "test");
	TEST_EQUAL(tbl["key"], "value");
	TEST_EQUAL(tbl["childtable"][1], 3);
}

KAGUYA_TEST_GROUP_END(test_11_cxx11_feature)


#endif
