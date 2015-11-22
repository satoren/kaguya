#include <iostream>
#include <cassert>

#include "kaguya/kaguya.hpp"


namespace selector_test
{
	namespace t_01_primitive
	{
		bool bool_get(kaguya::State& state)
		{
			state("value = true");
			return state["value"] == true;
		};
		bool int_get(kaguya::State& state)
		{
			state("value = 3");
			return state["value"] == int(3);
		};
		bool string_get(kaguya::State& state)
		{
			state("value = \"test\"");
			return state["value"] == "test" &&  state["value"] == std::string("test");
		};
		bool table_get1(kaguya::State& state)
		{
			state("value = {1,32,\"teststr\"}");
			return state["value"][1] == 1 && state["value"][2] == 32 && state["value"][3] == "teststr";
		};
		bool table_get2(kaguya::State& state)
		{
			state("value={out=32,str=\"gjgj\"}");
			state("value[\"in\"] = \"test\"");
			kaguya::Selector value = state["value"];
			return value["str"] == "gjgj" && value["in"] == "test" &&  value["out"] == 32;
		};

		bool bool_set(kaguya::State& state)
		{
			state["value"] = true;
			return state("assert(value == true)");
		};
		bool int_set(kaguya::State& state)
		{
			state["value"] = 3;
			return state("assert(value == 3)");
		};
		bool string_set(kaguya::State& state)
		{
			state["value"] = "test";
			return state("assert(value == \"test\")");
		};
		bool table_set(kaguya::State& state)
		{
			state["value"]["abc"]["def"] = 7;
			state["value"]["abc"]["bbb"] = "test";
			return state("assert(value.abc.def == 7 and value.abc.bbb == \"test\")");
		};
	}


	namespace t_02_classreg
	{
		struct ABC
		{
			int intmember;
			std::string stringmember;

			ABC() :intmember(0) {}
			ABC(int a) :intmember(a) { }
			ABC(const char* a) :stringmember(a) { }
			ABC(std::string a) :stringmember(a) { }
			ABC(int intmem, const std::string& strmem) :intmember(intmem), stringmember(strmem) { }
			ABC(const std::string& strmem, int intmem) :intmember(intmem), stringmember(strmem) { }
			ABC(const ABC&src) :intmember(src.intmember), stringmember(src.stringmember) { }

			int getInt() const {
				return intmember;
			}
			std::string getString()const {
				return stringmember;
			}
			void setString(std::string str) { stringmember = str; }

			ABC copy()const { return *this; }
			const ABC& references()const { return *this; }
			ABC& references() { return *this; }
			ABC* pointer() { return this; }
			kaguya::standard::shared_ptr<ABC> shared_copy() { return kaguya::standard::shared_ptr<ABC>(new ABC(*this)); }

		};

		bool default_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor()
				);

			return state("value = assert(ABC.new())");
		};
		bool int_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("getInt", &ABC::getInt)
				);

			if (!state("value = assert(ABC.new(32))")) { return false; }
			if (!state("assert(value:getInt() == 32)")) { return false; }
			return true;
		};
		bool string_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<const char*>()
				.addMember("getString", &ABC::getString)
				);

			if (!state("value = assert(ABC.new(\"string_value\"))")) { return false; }
			if (!state("assert(value:getString() == \"string_value\")")) { return false; }
			return true;
		};
		bool overloaded_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<const char*>()
				.addConstructor<int>()
				.addConstructor<std::string>()
				.addConstructor<int, const std::string&>()
				.addConstructor<const std::string&, int>()
				.addMember("getString", &ABC::getString)
				.addMember("getInt", &ABC::getInt)
				);

			if (!state("value = assert(ABC.new(32))")) { return false; }
			if (!state("assert(value:getInt() == 32)")) { return false; }
			if (!state("value = assert(ABC.new(\"string_value\"))")) { return false; }
			if (!state("assert(value:getString() == \"string_value\")")) { return false; }
			if (!state("value = assert(ABC.new(\"string_value2\",54))")) { return false; }
			if (!state("assert(value:getString() == \"string_value2\")")) { return false; }
			if (!state("assert(value:getInt() == 54)")) { return false; }
			if (!state("value = assert(ABC.new(64,\"string_value3\"))")) { return false; }
			if (!state("assert(value:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value:getInt() == 64)")) { return false; }
			return true;
		};
		bool copy_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<const char*>()
				.addConstructor<int>()
				.addConstructor<std::string>()
				.addConstructor<int, const std::string&>()
				.addConstructor<const std::string&, int>()
				.addMember("getString", &ABC::getString)
				.addMember("setString", &ABC::setString)
				.addMember("getInt", &ABC::getInt)
				.addMember("references", static_cast<ABC& (ABC::*)()>(&ABC::references))
				.addMember("references", static_cast<const ABC& (ABC::*)()const>(&ABC::references))
				.addMember("pointer", &ABC::pointer)
				.addMember("copy", &ABC::copy)
				.addMember("shared_copy", &ABC::shared_copy)
				);


			if (!state("value = assert(ABC.new(64,\"string_value3\"))")) { return false; }
			if (!state("value2 = assert(value:copy())")) { return false; }
			if (!state("assert(value2:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value2:getInt() == 64)")) { return false; }
			if (!state("value3 = assert(value:references())")) { return false; }
			if (!state("assert(value3:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value3:getInt() == 64)")) { return false; }
			if (!state("value4 = assert(value:pointer())")) { return false; }
			if (!state("assert(value4:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value4:getInt() == 64)")) { return false; }

			if (!state("value5 = assert(value:shared_copy())")) { return false; }
			if (!state("value =1")) { return false; }
			state.garbage_collect();//value3 and value4 to dangling pointer
			if (!state("assert(value5:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value5:getInt() == 64)")) { return false; }
			//if (!state("assert(value3:getString() == \"string_value3\")")) { return false; }
			//if (!state("assert(value3:getInt() == 64)")) { return false; }
			//if (!state("assert(value4:getString() == \"string_value3\")")) { return false; }
			//if (!state("assert(value4:getInt() == 64)")) { return false; }
			return true;
		};
	}

	namespace t_03_function
	{
		int result = 0;
		void free_standing_function(int r)
		{
			result = r;
		}
		int free_standing_function2()
		{
			return 12;
		}

		bool free_standing_function_test(kaguya::State& state)
		{
			state["ABC"] = kaguya::function(&free_standing_function);
			state["ABC"](54);
			state["free2"] = kaguya::function(&free_standing_function2);
			return result == 54 && state["free2"]() == 12.0;
		}

		struct Foo
		{
			std::string bar;
			void setBar(std::string b) { bar = b; }
		};

		bool member_function_test(kaguya::State& state)
		{
			state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
				.addMember("setBar", &Foo::setBar)
				, false);
			Foo foo;
			kaguya::Selector lfoo = state["Foo"];
			lfoo["bar"] = kaguya::function(&Foo::setBar);
			lfoo["bar"](&foo, "bar");
			if (foo.bar != "bar") { return false; }

			state["foo"] = &foo;
			state("foo:setBar(\"test\")");
			if (foo.bar != "test") { return false; }

#if __cplusplus >= 201104L
			lfoo["bar"] = kaguya::function(kaguya::standard::function<void(std::string)>([&foo](std::string str) { foo.bar = str; }));
			if (!state("Foo.bar(\"test\")")) { return false; }
			return foo.bar != "test";
#else
			return true;
#endif
		}

		struct VariFoo
		{
			std::vector<kaguya::LuaRef> args;

			VariFoo() {}
			VariFoo(kaguya::variadic_arg_type a):args(a){
			
			}

			void variadic_arg_func(kaguya::variadic_arg_type args)
			{
				this->args = args;
			}

		};
		bool variadic_function_test(kaguya::State& state)
		{
			state["Vari"].setClass(kaguya::ClassMetatable<VariFoo>()
				.addConstructor()
				.addConstructorVariadicArg()
				.addMember("variadicfun", &VariFoo::variadic_arg_func)
				);
			state("var = Vari.new()");
			state("var:variadicfun(\"hanaregumi\",\"hana-uta\",5)");;
			VariFoo* ptr = state["var"];
			bool res1 = ptr && ptr->args[0].get<std::string>() == "hanaregumi"
				&&  ptr->args[1].get<std::string>() == "hana-uta"
				&&  ptr->args[2].get<int>() == 5;

			state("var = Vari.new(\"abc\")");
			ptr = state["var"];
			bool res2 = ptr && ptr->args[0].get<std::string>() == "abc";
			return res1 && res2;
		}
	}
}



typedef bool(*test_function_t)(kaguya::State&);
typedef std::map<std::string, test_function_t> test_function_map_t;

bool execute_test(const test_function_map_t& testmap)
{
	int testcount = testmap.size();
	int testindex = 1;
	for (test_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it, ++testindex)
	{
		kaguya::State state; state.openlibs();

		const std::string& test_name = it->first;

		std::cout << test_name << "  (" << testindex << "/" << testcount << ") testing...";

		if (it->second(state))
		{
			//test pass
			int stack_leak = lua_gettop(state.state());
			if (stack_leak == 0)
			{
				std::cout << "done" << std::endl;
			}
			else
			{
				std::cout << "stack leaked count=" << stack_leak << std::endl;
				return false;
			}
		}
		else
		{
			//test failure
			std::cout << "failure" << std::endl;
			return false;
		}
	}

	return true;
}

int main()
{
	bool test_result = false;
	{
		test_function_map_t testmap;
#define ADD_TEST(function) testmap[#function] = &function;

		ADD_TEST(selector_test::t_01_primitive::bool_get);
		ADD_TEST(selector_test::t_01_primitive::int_get);
		ADD_TEST(selector_test::t_01_primitive::string_get);
		ADD_TEST(selector_test::t_01_primitive::table_get1);
		ADD_TEST(selector_test::t_01_primitive::table_get2);
		ADD_TEST(selector_test::t_01_primitive::bool_set);
		ADD_TEST(selector_test::t_01_primitive::int_set);
		ADD_TEST(selector_test::t_01_primitive::string_set);
		ADD_TEST(selector_test::t_01_primitive::table_set);

		ADD_TEST(selector_test::t_02_classreg::default_constructor);
		ADD_TEST(selector_test::t_02_classreg::int_constructor);
		ADD_TEST(selector_test::t_02_classreg::string_constructor);
		ADD_TEST(selector_test::t_02_classreg::overloaded_constructor);
		ADD_TEST(selector_test::t_02_classreg::copy_constructor);

		ADD_TEST(selector_test::t_03_function::free_standing_function_test);
		ADD_TEST(selector_test::t_03_function::member_function_test);
		ADD_TEST(selector_test::t_03_function::variadic_function_test);
		
		test_result = execute_test(testmap);
	}
	return test_result ? 0 : -1;
}
