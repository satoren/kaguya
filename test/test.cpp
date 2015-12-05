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
			assert(state["value"] == true);
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
			state["value"] = kaguya::NewTable();
			state["value"]["abc"] = kaguya::NewTable();
			state["value"]["abc"]["def"] = 7;
			state["value"]["abc"]["bbb"] = "test";
			return state("assert(value.abc.def == 7 and value.abc.bbb == \"test\")");
		};

		enum testenum
		{
			Foo = 0,
			Bar = 1,
		};

		bool enum_set(kaguya::State& state)
		{
			state["value"] = Foo;
			return state("assert(value == 0)");
		};
		bool enum_get(kaguya::State& state)
		{
			state("value = 1");
			return state["value"] == Bar;
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
			ABC(const char* a) :intmember(0),stringmember(a) { }
			ABC(std::string a) :intmember(0), stringmember(a) { }
			ABC(int intmem, const std::string& strmem) :intmember(intmem), stringmember(strmem) { }
			ABC(const std::string& strmem, int intmem) :intmember(intmem), stringmember(strmem) { }
			ABC(const ABC&src) :intmember(src.intmember), stringmember(src.stringmember) {}


			bool operator ==(const ABC& rhs)const {
				return intmember == rhs.intmember && stringmember == rhs.stringmember;
			}
			bool operator <(const ABC& rhs)const {
				return intmember < rhs.intmember || (intmember == rhs.intmember && stringmember < rhs.stringmember);
			}
			bool operator !=(const ABC& rhs)const {
				return !(*this == rhs);
			}
			bool operator >(const ABC& rhs)const {
				return (rhs < *this);
			}
			bool operator >=(const ABC& rhs)const {
				return !(*this < rhs);
			}
			bool operator <=(const ABC& rhs)const {
				return !(*this > rhs);
			}

			int getInt() const {
				return intmember;
			}
			void setInt(const int& n)  {
				intmember = n;
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
				.addMember("setInt", &ABC::setInt)
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

			if (!state("value:setInt(33)")) { return false; }
			if (!state("assert(value:getInt() == 33)")) { return false; }
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
			state.garbageCollect();//warning!! value3 and value4 to dangling pointer
			if (!state("assert(value5:getString() == \"string_value3\")")) { return false; }
			if (!state("assert(value5:getInt() == 64)")) { return false; }


			{
				kaguya::standard::shared_ptr<ABC> shared_ptr(new ABC("shared_object", 53));
				state["shared_object"] = shared_ptr;
			}
			if (!state("assert(shared_object:getInt() == 53)")) { return false; }
			if (!state("assert(shared_object:getString() =='shared_object')")) { return false; }


			return true;
		};

		bool data_member_bind(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("intmember", &ABC::intmember)
				.addConstructor<std::string>()
				.addMember("stringmember", &ABC::stringmember)
				);

			if (!state("value = assert(ABC.new(64))")) { return false; }
			if (!state("assert(value:intmember() == 64)")) { return false; }


			if (!state("value:intmember(4)")) { return false; }
			if (!state("assert(value:intmember() == 4)")) { return false; }


			if (!state("value:stringmember(\"test\")")) { return false; }
			if (!state("assert(value:stringmember() == \"test\")")) { return false; }
			return true;
		}

		bool operator_bind(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("__eq", &ABC::operator==)
				.addMember("__lt", &ABC::operator<)
				.addMember("__le", &ABC::operator<=)
				);

			if (!state("value1 = assert(ABC.new(64))")) { return false; }
			if (!state("value2 = assert(ABC.new(64))")) { return false; }
			if (!state("assert(value1 == value2)")) { return false; }
			if (!state("assert(value1 >= value2)")) { return false; }

			if (!state("value1 = assert(ABC.new(64))")) { return false; }
			if (!state("value2 = assert(ABC.new(61))")) { return false; }
			if (!state("assert(value1 ~= value2)")) { return false; }
			if (!state("assert(value1 > value2)")) { return false; }
			if (!state("assert(value1 >= value2)")) { return false; }

			return true;
		}
		bool add_field(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addStaticField("TEST", 1)
				.addStaticField("TEST2", "343")
				.addStaticField("TEST3", 133.23)
				);

			if (!state("assert(ABC.TEST == 1)")) { return false; }
			if (!state("assert(ABC.TEST2 == \"343\")")) { return false; }
			if (!state("assert(ABC.TEST3 == 133.23)")) { return false; }
			return true;
		}


		struct CopyableClass
		{
			CopyableClass(int i) :member(i) {}
			bool operator==(const CopyableClass& rhs)const
			{
				return member == rhs.member;
			}
			bool operator!=(const CopyableClass& rhs)const
			{
				return member != rhs.member;
			}
		private:
			CopyableClass();
			int member;
		};
		bool copyable_class_test(kaguya::State& state)
		{
			state["CopyableClass"].setClass(kaguya::ClassMetatable<CopyableClass>()
				.addConstructor<int>()
				.addMember("__eq", &CopyableClass::operator==)
				);
			state["copy"] = CopyableClass(2);
			if (state["copy"] != CopyableClass(2)) { return false; }
			if (CopyableClass(2) != state["copy"]) { return false; }
			CopyableClass copy = state["copy"];
			if (copy != CopyableClass(2)) { return false; }
			return true;
		}


		struct NoncopyableClass
		{
			NoncopyableClass(int i) :member(i) {}
			bool operator==(const NoncopyableClass& rhs)const
			{
				return member == rhs.member;
			}
			bool operator!=(const NoncopyableClass& rhs)const
			{
				return member != rhs.member;
			}
		private:
			NoncopyableClass();
			NoncopyableClass(const NoncopyableClass&);
			int member;
		};
		bool noncopyable_class_test(kaguya::State& state)
		{
			state["NoncopyableClass"].setClass(kaguya::ClassMetatable<NoncopyableClass>()
				.addConstructor<int>()
				.addMember("__eq", &NoncopyableClass::operator==)
				);

			NoncopyableClass noncopy(2);
			state["noncopy"] = &noncopy;

			const NoncopyableClass* noncpyref = state["noncopy"];
			if (noncpyref != &noncopy) { return false; }
			return true;
		}
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
				.addCodeChunkResult("luafunc", "return function(a,b) return a*b end")
				);
			Foo foo;
			kaguya::Selector lfoo = state["Foo"];
			lfoo["bar"] = kaguya::function(&Foo::setBar);
			lfoo["bar"](&foo, "bar");
			if (foo.bar != "bar") { return false; }

			state["foo"] = &foo;

			state("foo:setBar(\"test\")");
			if (foo.bar != "test") { return false; }

			if (!state("assert(foo.luafunc(3,4) == 12)")) { return false; }


			(state["foo"]->*"setBar")("test2");
			if (foo.bar != "test2") { return false; }
#if __cplusplus >= 201103L || defined(__cpp_lambdas)
			lfoo["bar"] = kaguya::function(kaguya::standard::function<void(std::string)>([&foo](std::string str) { foo.bar = str; }));
			if (!state("Foo.bar(\"test\")")) { return false; }
			return foo.bar == "test";
#else
			return true;
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



		kaguya::standard::tuple<int, std::string> tuplefun()
		{
			return kaguya::standard::tuple<int, std::string>(12, "23");
		}

		bool multi_return_function_test(kaguya::State& state)
		{
			state("multresfun =function() return 1,2,4,8,16 end");
			int a, b, c, d, e;
			a = b = c = d = e = 0;
			kaguya::tie(a, b, c, d, e) = state["multresfun"]();
			if (!(a == 1 && b == 2 && c == 4 && d == 8 && e == 16)) { return false; }

			state["multresfun"] = kaguya::function(tuplefun);

			state("a,b = multresfun()");
			return state("assert(a == 12  and b == \"23\")");
		}

		bool vector_and_map_from_table_mapping(kaguya::State& state)
		{
			state("arraytablefn =function() return {32,1,2,4,8,16} end");
			std::vector<int> b = state["arraytablefn"]();

			if (!(b.size() == 6 && b[0] == 32 && b[1] == 1 && b[2] == 2 && b[3] == 4 && b[4] == 8 && b[5] == 16)) return false;


			state("tablefn =function() return {a=32,b=1,c=2} end");
			std::map<std::string, int> m = state["tablefn"]();
			if (!(m["a"] == 32 && m["b"] == 1 && m["c"] == 2)) return false;

			return true;
		}

		bool vector_and_map_to_table_mapping(kaguya::State& state)
		{
			std::vector<double> v; v.push_back(3); v.push_back(13); v.push_back(2); v.push_back(99);
			state["v"] = v;
			if (!state("print(v[0])assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)")) { return false; }


			std::map<std::string,double> m; m["a"]=4; m["b"] = 32; m["c"] = 24;
			state["m"] = m;
			if (!state("assert(m['a'] == 4 and m['b'] == 32 and m['c'] == 24)")) { return false; }

			return true;
		}

		bool coroutine(kaguya::State& state)
		{
			{
				if (!state("cor = coroutine.create( function()"
					"coroutine.yield(32) "
					"coroutine.yield(53) "
					"return 2 "
					" end)")) return false;

				kaguya::LuaRef cor = state["cor"];
				int r1 = cor();
				int r2 = cor();
				int r3 = cor();
				if (!(r1 == 32 && r2 == 53 && r3 == 2)) return false;
			}
			{
				state["cor2"] = kaguya::NewThread();
				kaguya::LuaRef cor2 = state["cor2"];
				if (!state("corfun = function(arg)"
					"coroutine.yield(arg) "
					"coroutine.yield(arg*2) "
					"coroutine.yield(arg*3) "
					"return arg*4 "
					" end")) return false;

				kaguya::LuaRef corfun = state["corfun"];
				int r1 = cor2(corfun, 3);
				int r2 = cor2();
				int r3 = cor2();
				int r4 = cor2();

				if (!(r1 == 3 && r2 == 6 && r3 == 9 && r4 == 12)) return false;
			}
			{

				state["cor2"] = kaguya::NewThread();
				kaguya::LuaRef cor2 = state["cor2"];
				if (!state("corfun = function(arg)"
					"for i = 1,arg do "
					"coroutine.yield() "
					"end "
					"end")) return false;
				kaguya::LuaRef corfun = state["corfun"];
				cor2(corfun, 10);
				int yieldnum = 0;
				while (cor2.thread_status() == LUA_YIELD)
				{
					cor2();
					yieldnum++;
				}

				if (!(yieldnum == 10)) return false;
			}

			return true;
		}
#if KAGUYA_USE_DECLTYPE
		bool lambdafun(kaguya::State& state)
		{
			state["ABC"] = kaguya::function([](int a) {return a * 2; });
			int a = state["ABC"](54);
			if (a != 108) { return false; }

			state["free2"] = kaguya::function([]() {return 12; });
			return state["free2"]() == 12.0;
		}
#endif

		enum TestEnum
		{
			Fooe = 0,
			Bare = 1,
			DoDoe = 32,
		};
		TestEnum enumdispatchtest(TestEnum arg)
		{
			assert(arg == Bare);
			return DoDoe;
		}
		bool enumdispatch(kaguya::State& state)
		{
			state["enumfun"] = kaguya::function(&enumdispatchtest);
			TestEnum result = state["enumfun"](Bare);

			return result == DoDoe;
		}
	}

	namespace t_04_lua_ref
	{
		bool access(kaguya::State& state)
		{
			kaguya::LuaRef ref(state.state(), "abc");
			if (ref.get<std::string>() != "abc") { return false; }

			state("abc={d =1,e=3,f=64,g=\"sss\"}");
			kaguya::LuaRef abctable = state["abc"];

			if (abctable["d"] != 1) { return false; }
			if (abctable["e"] != 3) { return false; }
			if (abctable["f"] != 64) { return false; }
			if (abctable["g"] != std::string("sss")) { return false; }

			typedef std::map<kaguya::LuaRef, kaguya::LuaRef> maptype;
			const maptype& map = abctable.map();

			if (map.size() != 4) { return false; }

			abctable.setField("a", "test");
			if (abctable["a"] != std::string("test")) { return false; }

			abctable.setField("a", 22);
			if (abctable["a"] != 22) { return false; }

			return true;
		}
		bool newtable(kaguya::State& state)
		{
			kaguya::LuaRef ref(state.state(), kaguya::NewTable());

			ref["tbl"] = kaguya::NewTable();
			kaguya::LuaRef othertable = ref["tbl"];

			if (othertable.map().size() != 0) { return false; }
			if (othertable.type() != kaguya::LuaRef::TYPE_TABLE) { return false; }

			othertable["foo"] = 3;
			if (othertable["foo"] != 3) { return false; }

			return true;
		}

		int free_standing_function()
		{
			return 12;
		}

		bool callfunction(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
			globalTable["tbl"] = kaguya::NewTable();
			kaguya::LuaRef tbl = globalTable["tbl"];

			state("tbl.fun=function() return 1 end");
			if (tbl.map().size() != 1) { return false; }

			int result = tbl["fun"]();
			if (result != 1) { return false; }

			state("tbl.value=6");
			state("tbl.memfun=function(self) return self.value end");
			result = (tbl->*"memfun")();//==tbl["memfun"](tbl) like tbl:memfun() in Lua

			if (result != 6) { return false; }

			globalTable["free2"] = kaguya::function(&free_standing_function);

			if (!state("assert(free2()==12)")) { return false; }
			;
			return true;
		}

		struct ob
		{
			operator bool()const { return true; }
		};
		bool test_operator_bool(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
			return ob() != globalTable;
		}
		bool typetyest_function(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
			return true;
		}
		bool operator_equal_test(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();

			if (globalTable != state.globalTable()) { return false; }
			if (!(globalTable == state.globalTable())) { return false; }

			kaguya::LuaRef luanum = state.newRef(422);
			kaguya::LuaRef luanum2 = state.newRef(42);
			kaguya::LuaRef luanum3 = state.newRef(422);

			if (luanum == luanum2) { return false; }
			if (!(luanum != luanum2)) { return false; }
			if (luanum < luanum2) { return false; }
			if (luanum <= luanum2) { return false; }
			if (!(luanum > luanum2)) { return false; }
			if (!(luanum >= luanum2)) { return false; }

			if (!(luanum == luanum3)) { return false; }
			if (luanum != luanum3) { return false; }
			if (luanum < luanum3) { return false; }
			if (!(luanum <= luanum3)) { return false; }
			if (luanum > luanum3) { return false; }
			if (!(luanum >= luanum3)) { return false; }

			return true;
		}
		bool typetest(kaguya::State& state)
		{
			kaguya::LuaRef luanum = state.newRef(422);
			if (!luanum.typeTest<int>()) { return false; }
			if (luanum.typeTest<std::string>()) { return false; }
			if (luanum.typeTest<ob>()) { return false; }

			return true;
		}


		struct CallbackTest
		{
			void callCallback(int x)
			{
				if (function_)
				{
					function_(x);
				}
			}
			void setCallback(kaguya::LuaRef fun)
			{
				function_ = fun;
			}
			kaguya::LuaRef function_;
		};

		bool luafun_callback(kaguya::State& state)
		{

			state["CallbackTest"].setClass(kaguya::ClassMetatable<CallbackTest>()
				.addConstructor()
				.addMember("setCallback", &CallbackTest::setCallback)
				.addMember("callCallback", &CallbackTest::callCallback));


			state("callback = CallbackTest.new();"
				"receive_data ={};"
				"callback:setCallback(function(x) table.insert(receive_data,x) end);"
				"callback:callCallback(54)"
				"callback:callCallback(24)"
				);

			(state["callback"]->*"callCallback")(454);
			std::vector<int> callback_data = state["receive_data"];

			if (!(callback_data.size() == 3 && callback_data[0] == 54 
				&& callback_data[1] == 24 && callback_data[2] == 454)) { return false; }

			return true;
		}

		
	}

	namespace t_05_error_handler
	{
		int error_count = 0;
		void error_fun(int status, const char* message)
		{
			error_count++;
		}
		bool set_error_function(kaguya::State& state)
		{
			error_count = 0;
			state.setErrorHandler(error_fun);
			if (state("awdorgkwl;gw")) { return false; }

			if (error_count != 1) { return false; }
			state["yy"]["yy"]["yy"]();
			return error_count > 0;
		}
		bool function_call_error(kaguya::State& state)
		{
			error_count = 0;
			state.setErrorHandler(error_fun);

			state["errofun"] = kaguya::function(error_fun);
			state["errofun"](33);

			return error_count == 1;
		}
	}

	namespace t_06_state
	{
		bool other_state(kaguya::State& unused)
		{
			lua_State* L = luaL_newstate();

			kaguya::State state(L);
			kaguya::State state2(L);
			kaguya::State state3(L);
			kaguya::State state4(L);
			kaguya::State state5(L);

			lua_close(L);
			return true;
		}

		void ignore_error_fun(int status, const char* message)
		{
		}
		bool load_string(kaguya::State& state)
		{
			kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
			if (luafun() == false) { return false; }
			state.setErrorHandler(ignore_error_fun);
			kaguya::LuaRef errorref = state.loadstring("function() e");//syntax error
			if (errorref) { return false; }
			return true;
		}
	}
}

void test_error_handler(int status, const char* message)
{
	throw std::runtime_error(std::string(message));
}

typedef bool(*test_function_t)(kaguya::State&);
typedef std::map<std::string, test_function_t> test_function_map_t;

bool execute_test(const test_function_map_t& testmap)
{
	bool fail = false;
	int testcount = testmap.size();
	int testindex = 1;

	std::vector<std::string> pass_tests;
	std::vector<std::string> fail_tests;
	for (test_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it, ++testindex)
	{
		kaguya::State state; state.openlibs();

		state.setErrorHandler(test_error_handler);

		const std::string& test_name = it->first;

		std::cout << test_name << "  (" << testindex << "/" << testcount << ") testing...";

		bool result = false;
		try
		{
			result = it->second(state);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		if (result)
		{
			//test pass
			int stack_leak = lua_gettop(state.state());
			if (stack_leak == 0)
			{
				std::cout << "done" << std::endl;
				pass_tests.push_back(test_name);
			}
			else
			{
				std::cout << "stack leaked count=" << stack_leak << std::endl;
				fail = true;
				fail_tests.push_back(test_name);
			}
		}
		else
		{
			//test failure
			std::cout << "failure" << std::endl;
			fail = true;
			fail_tests.push_back(test_name);
		}
	}
	if (fail)
	{
		std::cout << "test failed!!" << std::endl;
		std::cout << "error in ";

		for (size_t i = 0; i < fail_tests.size(); ++i)
		{

			std::cout << fail_tests[i] << ",";
		}

		std::cout << std::endl;

	}
	else
	{
	}

	return !fail;
}

void ignore_error_handler(int status, const char* message)
{
}
bool microbenchmark(const test_function_map_t& testmap)
{
	kaguya::State state; state.openlibs();
	state.setErrorHandler(ignore_error_handler);
	for (int i = 0; i < 10000; ++i)
	{
		for (test_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it)
		{
			it->second(state);
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


		ADD_TEST(selector_test::t_01_primitive::enum_set);
		ADD_TEST(selector_test::t_01_primitive::enum_get);

		ADD_TEST(selector_test::t_02_classreg::default_constructor);
		ADD_TEST(selector_test::t_02_classreg::int_constructor);
		ADD_TEST(selector_test::t_02_classreg::string_constructor);
		ADD_TEST(selector_test::t_02_classreg::overloaded_constructor);
		ADD_TEST(selector_test::t_02_classreg::copy_constructor);
		ADD_TEST(selector_test::t_02_classreg::data_member_bind);
		ADD_TEST(selector_test::t_02_classreg::operator_bind);
		ADD_TEST(selector_test::t_02_classreg::add_field);
		ADD_TEST(selector_test::t_02_classreg::copyable_class_test);
		ADD_TEST(selector_test::t_02_classreg::noncopyable_class_test);




		ADD_TEST(selector_test::t_03_function::free_standing_function_test);
		ADD_TEST(selector_test::t_03_function::member_function_test);
		ADD_TEST(selector_test::t_03_function::variadic_function_test);
		ADD_TEST(selector_test::t_03_function::multi_return_function_test);
		ADD_TEST(selector_test::t_03_function::vector_and_map_from_table_mapping);
		ADD_TEST(selector_test::t_03_function::vector_and_map_to_table_mapping);
		ADD_TEST(selector_test::t_03_function::coroutine);
#if KAGUYA_USE_DECLTYPE
		ADD_TEST(selector_test::t_03_function::lambdafun);

#endif

		ADD_TEST(selector_test::t_04_lua_ref::access);
		ADD_TEST(selector_test::t_04_lua_ref::newtable);
		ADD_TEST(selector_test::t_04_lua_ref::callfunction);
		ADD_TEST(selector_test::t_04_lua_ref::test_operator_bool);
		ADD_TEST(selector_test::t_04_lua_ref::typetyest_function);
		ADD_TEST(selector_test::t_04_lua_ref::operator_equal_test);
		ADD_TEST(selector_test::t_04_lua_ref::typetest);
		ADD_TEST(selector_test::t_04_lua_ref::luafun_callback);

		ADD_TEST(selector_test::t_05_error_handler::set_error_function);
		ADD_TEST(selector_test::t_05_error_handler::function_call_error);

		ADD_TEST(selector_test::t_06_state::other_state);
		ADD_TEST(selector_test::t_06_state::load_string);

		test_result = execute_test(testmap);

		//		microbenchmark(testmap);
	}
	return test_result ? 0 : -1;
}
