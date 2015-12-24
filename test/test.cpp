#include <iostream>
#include <cassert>

#include "kaguya/kaguya.hpp"



namespace selector_test
{
#define TEST_CHECK(B) if(!(B)) throw std::runtime_error( std::string("failed.\nfunction:") +__FUNCTION__  + std::string("\nline:") + kaguya::standard::to_string(__LINE__) + "\nCHECKCODE:" #B );
	namespace t_01_primitive
	{
		void bool_get(kaguya::State& state)
		{
			state("value = true");
			TEST_CHECK(state["value"] == true);
		};
		void int_get(kaguya::State& state)
		{
			state("value = 3");
			TEST_CHECK(state["value"] == int(3));
		};
		void string_get(kaguya::State& state)
		{
			state("value = \"test\"");
			TEST_CHECK(state["value"] == "test" &&  state["value"] == std::string("test"));
		};
		void table_get1(kaguya::State& state)
		{
			state("value = {1,32,\"teststr\"}");
			TEST_CHECK(state["value"][1] == 1 && state["value"][2] == 32 && state["value"][3] == "teststr");
		};
		void table_get2(kaguya::State& state)
		{
			state("value={out=32,str=\"gjgj\"}");
			state("value[\"in\"] = \"test\"");
			kaguya::LuaRef value = state["value"];
			TEST_CHECK(value["str"] == "gjgj" && value["in"] == "test" &&  value["out"] == 32);
		};

		void bool_set(kaguya::State& state)
		{
			state["value"] = true;
			assert(state["value"] == true);
			TEST_CHECK(state("assert(value == true)"));
		};
		void int_set(kaguya::State& state)
		{
			state["value"] = 3;
			TEST_CHECK(state("assert(value == 3)"));
		};
		void string_set(kaguya::State& state)
		{
			state["value"] = "test";
			TEST_CHECK(state("assert(value == \"test\")"));
		};
		void table_set(kaguya::State& state)
		{
			state["value"] = kaguya::NewTable();
			state["value"]["abc"] = kaguya::NewTable();
			state["value"]["abc"]["def"] = 7;
			state["value"]["abc"]["bbb"] = "test";
			TEST_CHECK(state("assert(value.abc.def == 7 and value.abc.bbb == \"test\")"));
		};

		enum testenum
		{
			Foo = 0,
			Bar = 1,
		};

		void enum_set(kaguya::State& state)
		{
			state["value"] = Foo;
			TEST_CHECK(state("assert(value == 0)"));
		};
		void enum_get(kaguya::State& state)
		{
			state("value = 1");
			TEST_CHECK(state["value"] == Bar);
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
			ABC(const char* a) :intmember(0), stringmember(a) { }
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
			void setInt(const int& n) {
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

		void default_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor()
				);

			TEST_CHECK(state("value = assert(ABC.new())"));
		};
		void int_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("getInt", &ABC::getInt)
				);

			TEST_CHECK(state("value = assert(ABC.new(32))")) ;
			TEST_CHECK(state("assert(value:getInt() == 32)")) ;
		};
		void string_constructor(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<const char*>()
				.addMember("getString", &ABC::getString)
				);

			TEST_CHECK(state("value = assert(ABC.new(\"string_value\"))")) ;
			TEST_CHECK(state("assert(value:getString() == \"string_value\")")) ;
		};
		void overloaded_constructor(kaguya::State& state)
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

			TEST_CHECK(state("value = assert(ABC.new(32))")) ;
			TEST_CHECK(state("assert(value:getInt() == 32)")) ;
			TEST_CHECK(state("value = assert(ABC.new(\"string_value\"))")) ;
			TEST_CHECK(state("assert(value:getString() == \"string_value\")")) ;
			TEST_CHECK(state("value = assert(ABC.new(\"string_value2\",54))")) ;
			TEST_CHECK(state("assert(value:getString() == \"string_value2\")")) ;
			TEST_CHECK(state("assert(value:getInt() == 54)")) ;
			TEST_CHECK(state("value = assert(ABC.new(64,\"string_value3\"))")) ;
			TEST_CHECK(state("assert(value:getString() == \"string_value3\")")) ;
			TEST_CHECK(state("assert(value:getInt() == 64)")) ;

			TEST_CHECK(state("value:setInt(33)")) ;
			TEST_CHECK(state("assert(value:getInt() == 33)")) ;
		};
		void copy_constructor(kaguya::State& state)
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


			TEST_CHECK(state("value = assert(ABC.new(64,\"string_value3\"))")) ;
			TEST_CHECK(state("value2 = assert(value:copy())")) ;
			TEST_CHECK(state("assert(value2:getString() == \"string_value3\")")) ;
			TEST_CHECK(state("assert(value2:getInt() == 64)")) ;
			TEST_CHECK(state("value3 = assert(value:references())")) ;
			TEST_CHECK(state("assert(value3:getString() == \"string_value3\")")) ;
			TEST_CHECK(state("assert(value3:getInt() == 64)")) ;
			TEST_CHECK(state("value4 = assert(value:pointer())")) ;
			TEST_CHECK(state("assert(value4:getString() == \"string_value3\")")) ;
			TEST_CHECK(state("assert(value4:getInt() == 64)")) ;

			TEST_CHECK(state("value5 = assert(value:shared_copy())")) ;
			TEST_CHECK(state("value =1")) ;
			state.garbageCollect();//warning!! value3 and value4 to dangling pointer
			TEST_CHECK(state("assert(value5:getString() == \"string_value3\")")) ;
			TEST_CHECK(state("assert(value5:getInt() == 64)")) ;


			{
				kaguya::standard::shared_ptr<ABC> shared_ptr(new ABC("shared_object", 53));
				state["shared_object"] = shared_ptr;
			}
			TEST_CHECK(state("assert(shared_object:getInt() == 53)")) ;
			TEST_CHECK(state("assert(shared_object:getString() =='shared_object')")) ;


		};

		void data_member_bind(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("intmember", &ABC::intmember)
				.addConstructor<std::string>()
				.addMember("stringmember", &ABC::stringmember)
				);

			TEST_CHECK(state("value = assert(ABC.new(64))")) ;
			TEST_CHECK(state("assert(value:intmember() == 64)")) ;


			TEST_CHECK(state("value:intmember(4)")) ;
			TEST_CHECK(state("assert(value:intmember() == 4)")) ;


			TEST_CHECK(state("value:stringmember(\"test\")")) ;
			TEST_CHECK(state("assert(value:stringmember() == \"test\")")) ;
		}

		void operator_bind(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("__eq", &ABC::operator==)
				.addMember("__lt", &ABC::operator<)
				.addMember("__le", &ABC::operator<=)
				);

			TEST_CHECK(state("value1 = assert(ABC.new(64))")) ;
			TEST_CHECK(state("value2 = assert(ABC.new(64))")) ;
			TEST_CHECK(state("assert(value1 == value2)")) ;
			TEST_CHECK(state("assert(value1 >= value2)")) ;

			TEST_CHECK(state("value1 = assert(ABC.new(64))")) ;
			TEST_CHECK(state("value2 = assert(ABC.new(61))")) ;
			TEST_CHECK(state("assert(value1 ~= value2)")) ;
			TEST_CHECK(state("assert(value1 > value2)")) ;
			TEST_CHECK(state("assert(value1 >= value2)")) ;
		}
		void add_field(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addStaticField("TEST", 1)
				.addStaticField("TEST2", "343")
				.addStaticField("TEST3", 133.23)
				);

			TEST_CHECK(state("assert(ABC.TEST == 1)")) ;
			TEST_CHECK(state("assert(ABC.TEST2 == \"343\")")) ;
			TEST_CHECK(state("assert(ABC.TEST3 == 133.23)")) ;
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
		void copyable_class_test(kaguya::State& state)
		{
			state["CopyableClass"].setClass(kaguya::ClassMetatable<CopyableClass>()
				.addConstructor<int>()
				.addMember("__eq", &CopyableClass::operator==)
				);
			state["copy"] = CopyableClass(2);
			TEST_CHECK (state["copy"] == CopyableClass(2)) ;
			TEST_CHECK(CopyableClass(2) == state["copy"]) ;
			CopyableClass copy = state["copy"];
			TEST_CHECK (copy == CopyableClass(2)) ;
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
			NoncopyableClass& operator=(const NoncopyableClass&);
			int member;
		};
		void noncopyable_class_test(kaguya::State& state)
		{
			state["NoncopyableClass"].setClass(kaguya::ClassMetatable<NoncopyableClass>()
				.addConstructor<int>()
				.addMember("__eq", &NoncopyableClass::operator==)
				);

			NoncopyableClass noncopy(2);
			state["noncopy"] = &noncopy;

			const NoncopyableClass* noncpyref = state["noncopy"];
			TEST_CHECK(noncpyref == &noncopy) ;
		}

		void registering_object_instance(kaguya::State& state)
		{
			state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
				.addConstructor<int>()
				.addMember("get_value", &ABC::getInt)
				);

			ABC abc(43);
			//register object pointer
			state["abc"] = &abc;
			state("assert(43 == abc:get_value())");
			//or copy instance
			state["copy_abc"] = abc;
			state("assert(43 == copy_abc:get_value())");
			//or registering shared pointer
			state["shared_abc"] = kaguya::standard::shared_ptr<ABC>(new ABC(43));//kaguya::standard::shared_ptr is std::shared_ptr or boost::shared_ptr.
			state("assert(43 == shared_abc:get_value())");
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

		void free_standing_function_test(kaguya::State& state)
		{
			state["ABC"] = kaguya::function(&free_standing_function);
			state["ABC"](54);
			state["free2"] = kaguya::function(&free_standing_function2);
			TEST_CHECK( result == 54 && state["free2"]() == 12.0);
		}

		struct Foo
		{
			std::string bar;
			void setBar(std::string b) { bar = b; }
		};

		void member_function_test(kaguya::State& state)
		{
			state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
				.addMember("setBar", &Foo::setBar)
				.addCodeChunkResult("luafunc", "return function(a,b) return a*b end")
				);
			Foo foo;
			kaguya::LuaRef lfoo = state["Foo"];
			lfoo["bar"] = kaguya::function(&Foo::setBar);
			lfoo["bar"](&foo, "bar");
			TEST_CHECK (foo.bar == "bar") ;

			state["foo"] = &foo;

			state("foo:setBar(\"test\")");
			TEST_CHECK (foo.bar == "test") ;

			TEST_CHECK(state("assert(foo.luafunc(3,4) == 12)")) ;


			(state["foo"]->*"setBar")("test2");
			TEST_CHECK (foo.bar == "test2") ;
#if __cplusplus >= 201103L || defined(__cpp_lambdas)
			lfoo["bar"] = kaguya::function(kaguya::standard::function<void(std::string)>([&foo](std::string str) { foo.bar = str; }));
			TEST_CHECK(state("Foo.bar(\"test\")")) ;
			TEST_CHECK(foo.bar == "test");
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
		void variadic_function_test(kaguya::State& state)
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
			TEST_CHECK(res1 && res2);
		}



		kaguya::standard::tuple<int, std::string> tuplefun()
		{
			return kaguya::standard::tuple<int, std::string>(12, "23");
		}

		void multi_return_function_test(kaguya::State& state)
		{
			state("multresfun =function() return 1,2,4,8,16 end");
			int a, b, c, d, e;
			a = b = c = d = e = 0;
			kaguya::tie(a, b, c, d, e) = state["multresfun"]();
			TEST_CHECK((a == 1 && b == 2 && c == 4 && d == 8 && e == 16)) ;

			state["multresfun"] = kaguya::function(tuplefun);

			state("a,b = multresfun()");
			TEST_CHECK(state("assert(a == 12  and b == \"23\")"));
		}

		void vector_and_map_from_table_mapping(kaguya::State& state)
		{
			state("arraytablefn =function() return {32,1,2,4,8,16} end");
			std::vector<int> b = state["arraytablefn"]();

			TEST_CHECK((b.size() == 6 && b[0] == 32 && b[1] == 1 && b[2] == 2 && b[3] == 4 && b[4] == 8 && b[5] == 16));


			state("tablefn =function() return {a=32,b=1,c=2} end");
			std::map<std::string, int> m = state["tablefn"]();
			TEST_CHECK((m["a"] == 32 && m["b"] == 1 && m["c"] == 2)) ;
		}

		void vector_and_map_to_table_mapping(kaguya::State& state)
		{
			std::vector<double> v; v.push_back(3); v.push_back(13); v.push_back(2); v.push_back(99);
			state["v"] = v;
			TEST_CHECK(state("assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)")) ;


			std::map<std::string, double> m; m["a"] = 4; m["b"] = 32; m["c"] = 24;
			state["m"] = m;
			TEST_CHECK(state("assert(m['a'] == 4 and m['b'] == 32 and m['c'] == 24)")) ;
		}



		void coroutine(kaguya::State& state)
		{
			{
				TEST_CHECK(state("cor = coroutine.create( function()"
					"coroutine.yield(32) "
					"coroutine.yield(53) "
					"return 2 "
					" end)")) ;

				kaguya::LuaRef cor = state["cor"];
				int r1 = cor();
				int r2 = cor();
				int r3 = cor();
				TEST_CHECK((r1 == 32 && r2 == 53 && r3 == 2)) ;
			}
			{
				state["cor2"] = kaguya::NewThread();
				kaguya::LuaRef cor2 = state["cor2"];
				TEST_CHECK(state("corfun = function(arg)"
					"coroutine.yield(arg) "
					"coroutine.yield(arg*2) "
					"coroutine.yield(arg*3) "
					"return arg*4 "
					" end")) ;

				kaguya::LuaRef corfun = state["corfun"];
				int r1 = cor2(corfun, 3);
				int r2 = cor2();
				int r3 = cor2();
				int r4 = cor2();

				TEST_CHECK((r1 == 3 && r2 == 6 && r3 == 9 && r4 == 12)) ;
			}
			{

				state["cor2"] = kaguya::NewThread();
				kaguya::LuaRef cor2 = state["cor2"];
				TEST_CHECK(state("corfun = function(arg)"
					"for i = 1,arg do "
					"coroutine.yield() "
					"end "
					"end")) ;
				kaguya::LuaRef corfun = state["corfun"];
				cor2(corfun, 10);
				int yieldnum = 0;
				while (cor2.thread_status() == LUA_YIELD)
				{
					cor2();
					yieldnum++;
				}

				TEST_CHECK((yieldnum == 10)) ;
			}
		}

		void pointerfun(VariFoo* pointer)
		{
			TEST_CHECK(pointer == 0);
		}
		void const_pointerfun(VariFoo* pointer)
		{
			TEST_CHECK(pointer == 0);
		}
		void ignore_error_fun(int status, const char* message)
		{
		}
		void zero_to_nullpointer(kaguya::State& state)
		{
			state["pointerfun"] = kaguya::function(pointerfun);
			TEST_CHECK(state("pointerfun(0)"));
			state["const_pointerfun"] = kaguya::function(const_pointerfun);
			TEST_CHECK(state("const_pointerfun(0)"));

			state.setErrorHandler(ignore_error_fun);
			TEST_CHECK(!state("pointerfun(32)"));// is error
			TEST_CHECK(!state("pointerfun('232')"));// is error

		}

#if KAGUYA_USE_DECLTYPE
		void lambdafun(kaguya::State& state)
		{
			state["ABC"] = kaguya::function([](int a) {return a * 2; });
			int a = state["ABC"](54);
			TEST_CHECK (a == 108) ;

			state["free2"] = kaguya::function([]() {return 12; });
			TEST_CHECK(state["free2"]() == 12.0);
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
		void access(kaguya::State& state)
		{
			kaguya::LuaRef ref(state.state(), "abc");
			TEST_CHECK(ref.get<std::string>() == "abc") ;

			state("abc={d =1,e=3,f=64,g=\"sss\"}");
			kaguya::LuaRef abctable = state["abc"];

			TEST_CHECK(abctable["d"] == 1) ;
			TEST_CHECK(abctable["e"] == 3) ;
			TEST_CHECK(abctable["f"] == 64) ;
			TEST_CHECK(abctable["g"] == std::string("sss")) ;

			typedef std::map<kaguya::LuaRef, kaguya::LuaRef> maptype;
			const maptype& map = abctable.map();

			TEST_CHECK (map.size() == 4) ;

			abctable.setField("a", "test");
			TEST_CHECK(abctable["a"] == std::string("test")) ;

			abctable.setField("a", 22);
			TEST_CHECK(abctable["a"] == 22) ;
		}
		void newtable(kaguya::State& state)
		{
			kaguya::LuaRef ref(state.state(), kaguya::NewTable());

			ref["tbl"] = kaguya::NewTable();
			kaguya::LuaRef othertable = ref["tbl"];

			TEST_CHECK(othertable.map().size() == 0) ;
			TEST_CHECK(othertable.type() == kaguya::LuaRef::TYPE_TABLE) ;

			othertable["foo"] = 3;
			TEST_CHECK (othertable["foo"] == 3) ;
		}

		int free_standing_function()
		{
			return 12;
		}

		void callfunction(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
			globalTable["tbl"] = kaguya::NewTable();
			kaguya::LuaRef tbl = globalTable["tbl"];

			state("tbl.fun=function() return 1 end");
			TEST_CHECK(tbl.map().size() == 1) ;

			int result = tbl["fun"]();
			TEST_CHECK(result == 1) ;

			state("tbl.value=6");
			state("tbl.memfun=function(self) return self.value end");
			result = (tbl->*"memfun")();//==tbl["memfun"](tbl) like tbl:memfun() in Lua

			TEST_CHECK(result == 6) ;

			globalTable["free2"] = kaguya::function(&free_standing_function);

			TEST_CHECK(state("assert(free2()==12)")) ;
		}

		struct ob
		{
			operator bool()const { return true; }
		};
		void test_operator_bool(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
			TEST_CHECK(ob() != globalTable);
		}
		void typetest_function(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();
		}
		void operator_equal_test(kaguya::State& state)
		{
			kaguya::LuaRef globalTable = state.globalTable();

			TEST_CHECK(!(globalTable != state.globalTable())) ;
			TEST_CHECK(globalTable == state.globalTable()) ;

			kaguya::LuaRef luanum = state.newRef(422);
			kaguya::LuaRef luanum2 = state.newRef(42);
			kaguya::LuaRef luanum3 = state.newRef(422);

			TEST_CHECK(!(luanum == luanum2));
			TEST_CHECK(luanum != luanum2);
			TEST_CHECK(!(luanum < luanum2)) ;
			TEST_CHECK(!(luanum <= luanum2)) ;
			TEST_CHECK(luanum > luanum2) ;
			TEST_CHECK(luanum >= luanum2) ;

			TEST_CHECK(luanum == luanum3) ;
			TEST_CHECK(!(luanum != luanum3)) ;
			TEST_CHECK(!(luanum < luanum3)) ;
			TEST_CHECK(luanum <= luanum3) ;
			TEST_CHECK(!(luanum > luanum3)) ;
			TEST_CHECK(luanum >= luanum3) ;
		}
		void typetest(kaguya::State& state)
		{
			kaguya::LuaRef luanum = state.newRef(422);
			TEST_CHECK(luanum.typeTest<int>()) ;
			TEST_CHECK (!luanum.typeTest<std::string>()) ;
			TEST_CHECK (!luanum.typeTest<ob>()) ;
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

		void luafun_callback(kaguya::State& state)
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

			TEST_CHECK((callback_data.size() == 3 && callback_data[0] == 54
				&& callback_data[1] == 24 && callback_data[2] == 454)) 

		}

		void function_env(kaguya::State& state)
		{
			kaguya::LuaRef fun = state.loadstring("foo='bar'");

			fun.setFunctionEnv(kaguya::NewTable());
			fun();

			TEST_CHECK("assert(foo == nil)");
			state["functionEnv"] = fun.getFunctionEnv();
			TEST_CHECK("assert(functionEnv.foo == 'bar')");
		}

	}

	namespace t_05_error_handler
	{
		int error_count = 0;
		void error_fun(int status, const char* message)
		{
			error_count++;
		}
		void set_error_function(kaguya::State& state)
		{
			error_count = 0;
			state.setErrorHandler(error_fun);
			TEST_CHECK (!state("awdorgkwl;gw")) ;

			TEST_CHECK (error_count == 1) ;
			state["yy"]["yy"]["yy"]();
			TEST_CHECK( error_count > 0);
		}
		void function_call_error(kaguya::State& state)
		{
			error_count = 0;
			state.setErrorHandler(error_fun);

			state["errofun"] = kaguya::function(error_fun);
			state["errofun"](33);

			TEST_CHECK( error_count == 1);
		}
	}

	namespace t_06_state
	{
		void other_state(kaguya::State& unused)
		{
			lua_State* L = luaL_newstate();

			kaguya::State state(L);
			kaguya::State state2(L);
			kaguya::State state3(L);
			kaguya::State state4(L);
			kaguya::State state5(L);

			lua_close(L);
		}

		void ignore_error_fun(int status, const char* message)
		{
		}
		void load_string(kaguya::State& state)
		{
			kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
			TEST_CHECK (luafun() != false) ;
			state.setErrorHandler(ignore_error_fun);
			kaguya::LuaRef errorref = state.loadstring("function() e");//syntax error
			TEST_CHECK (!errorref) ;
		}


		void load_with_other_env(kaguya::State& state)
		{
			state.dostring("foo = 'bar'");

			state["otherEnv"] = state.newRef(kaguya::NewTable());
			state.dostring("foo = 'dar'", state["otherEnv"]);

			TEST_CHECK(state("assert(foo == 'bar')"));

			TEST_CHECK(state("assert(otherEnv.foo == 'dar')"));
		}
	}

	namespace t_07_any_type_test
	{
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
			kaguya::standard::function<bool(T2)>f = fun;
			state["testfn"] = kaguya::function(f);
			state["value"] = setvalue;
			return state("assert(testfn(value))");
		}

		template<class T, class T2>
		bool test(kaguya::State& state, T value1, T2 value2)
		{
			TEST_CHECK(setget_eq(state, value1, value2)) ;
			TEST_CHECK(function_call(state, value1, value2)) ;
			return true;
		}
		void any_type_test(kaguya::State& state)
		{
			TEST_CHECK(test(state, true, true)) ;
			TEST_CHECK(test(state, 0, 0)) ;
			TEST_CHECK(test(state, "", std::string(""))) ;
			TEST_CHECK(test(state, "false", std::string("false"))) ;
			TEST_CHECK(test(state, std::string(""), std::string(""))) ;
			TEST_CHECK(test(state, 1, 1)) ;
			TEST_CHECK(test(state, kaguya::LuaRef(), kaguya::LuaRef())) ;
			TEST_CHECK(test(state, Foo, Foo)) ;
			TEST_CHECK(test(state, Bar, Bar)) ;

			TEST_CHECK(test(state, kaguya::LuaRef(), false)) ;
			TEST_CHECK(test(state, (void*)0, false)) ;
			TEST_CHECK(test(state, 0, true)) ;
			TEST_CHECK(test(state, "", true)) ;
			TEST_CHECK(test(state, "false", true)) ;
			TEST_CHECK(test(state, std::string(""), true)) ;
			TEST_CHECK(test(state, 1, true)) ;
			TEST_CHECK(test(state, std::numeric_limits<float>::min(), std::numeric_limits<float>::min())) ;
			TEST_CHECK(test(state, std::numeric_limits<double>::min(), std::numeric_limits<double>::min())) ;
			TEST_CHECK(test(state, std::numeric_limits<float>::max(), std::numeric_limits<float>::max())) ;
			TEST_CHECK(test(state, std::numeric_limits<double>::max(), std::numeric_limits<double>::max())) ;
#if LUA_VERSION_NUM > 502
			TEST_CHECK(test(state, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min())) ;
			TEST_CHECK(test(state, std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::min())) ;
			TEST_CHECK(test(state, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max())) ;
			TEST_CHECK(test(state, std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max())) ;
#endif
			TEST_CHECK(test(state, short(5), short(5))) ;
			TEST_CHECK(test(state, 1, state.newRef(1))) ;
			TEST_CHECK(test(state, "test", state.newRef("test"))) ;
			TEST_CHECK(test(state, false, state.newRef(false))) ;
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
				TEST_CHECK(test(state, setmap, copy)) ;
			}
			{
				std::vector<std::string> setvec;
				setvec.push_back("342");
				setvec.push_back("32");
				std::vector<std::string> copy = setvec;
				TEST_CHECK(test(state, setvec, copy)) ;
			}
			{
				std::vector<std::vector<std::string> > setvec;
				setvec.push_back(std::vector<std::string>(12, "332"));
				setvec.push_back(std::vector<std::string>(42, "3232"));
				std::vector<std::vector<std::string> > copy = setvec;
				TEST_CHECK(test(state, setvec, copy)) ;
			}
		}
	}
}

void test_error_handler(int status, const char* message)
{
	throw std::runtime_error(std::string(message));
}

typedef void(*test_function_t)(kaguya::State&);
typedef std::map<std::string, test_function_t> test_function_map_t;

bool execute_test(const test_function_map_t& testmap)
{
	bool fail = false;
	size_t testcount = testmap.size();
	size_t testindex = 1;

	std::vector<std::string> pass_tests;
	std::vector<std::string> fail_tests;
	for (test_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it, ++testindex)
	{
		kaguya::State state; state.openlibs();

		state.setErrorHandler(test_error_handler);

		const std::string& test_name = it->first;

		std::cout << test_name << "  (" << testindex << "/" << testcount << ") ...";

		bool result = false;
		try
		{
			it->second(state);
			result = true;
		}
		catch (std::exception& e)
		{
			result = false;
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
		std::cout << "all pass." << std::endl;
	}

	return !fail;
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
		ADD_TEST(selector_test::t_02_classreg::registering_object_instance);




		ADD_TEST(selector_test::t_03_function::free_standing_function_test);
		ADD_TEST(selector_test::t_03_function::member_function_test);
		ADD_TEST(selector_test::t_03_function::variadic_function_test);
		ADD_TEST(selector_test::t_03_function::multi_return_function_test);
		ADD_TEST(selector_test::t_03_function::vector_and_map_from_table_mapping);
		ADD_TEST(selector_test::t_03_function::vector_and_map_to_table_mapping);
		ADD_TEST(selector_test::t_03_function::coroutine);
		ADD_TEST(selector_test::t_03_function::zero_to_nullpointer);
#if KAGUYA_USE_DECLTYPE
		ADD_TEST(selector_test::t_03_function::lambdafun);

#endif

		ADD_TEST(selector_test::t_04_lua_ref::access);
		ADD_TEST(selector_test::t_04_lua_ref::newtable);
		ADD_TEST(selector_test::t_04_lua_ref::callfunction);
		ADD_TEST(selector_test::t_04_lua_ref::test_operator_bool);
		ADD_TEST(selector_test::t_04_lua_ref::typetest_function);
		ADD_TEST(selector_test::t_04_lua_ref::operator_equal_test);
		ADD_TEST(selector_test::t_04_lua_ref::typetest);
		ADD_TEST(selector_test::t_04_lua_ref::luafun_callback);
		ADD_TEST(selector_test::t_04_lua_ref::function_env);

		ADD_TEST(selector_test::t_05_error_handler::set_error_function);
		ADD_TEST(selector_test::t_05_error_handler::function_call_error);

		ADD_TEST(selector_test::t_06_state::other_state);
		ADD_TEST(selector_test::t_06_state::load_string);

		ADD_TEST(selector_test::t_06_state::load_with_other_env);

		

		ADD_TEST(selector_test::t_07_any_type_test::any_type_test);

		test_result = execute_test(testmap);
	}
	return test_result ? 0 : -1;
}
