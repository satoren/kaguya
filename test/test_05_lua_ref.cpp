#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_05_lua_ref)
using namespace kaguya_test_util;

std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(access)(kaguya::State &state) {
  kaguya::LuaRef ref(state.state(), "abc");
  TEST_EQUAL(ref.get<std::string>(), "abc");
  ref = ref;

  state("abc={d =1,e=3,f=64,g='sss'}");
  kaguya::LuaRef abctable = state["abc"];

  TEST_EQUAL(abctable["d"], 1);
  TEST_EQUAL(abctable["e"], 3);
  TEST_EQUAL(abctable["f"], 64);
  TEST_EQUAL(abctable["g"], std::string("sss"));

  typedef std::map<kaguya::LuaRef, kaguya::LuaRef> maptype;
  const maptype &map = abctable.map();
  TEST_CHECK(map.size() == 4);

  std::map<std::string, std::string> strmap =
      abctable.map<std::string, std::string>();

  TEST_EQUAL(strmap["d"], "1");
  TEST_EQUAL(strmap["e"], "3");
  TEST_EQUAL(strmap["f"], "64");
  TEST_EQUAL(strmap["g"], "sss");
  TEST_EQUAL(strmap.size(), 4);

  std::vector<std::string> keys = abctable.keys<std::string>();

  TEST_EQUAL(keys.size(), 4);
  TEST_CHECK(std::find(keys.begin(), keys.end(), "d") != keys.end());
  TEST_CHECK(std::find(keys.begin(), keys.end(), "e") != keys.end());
  TEST_CHECK(std::find(keys.begin(), keys.end(), "f") != keys.end());
  TEST_CHECK(std::find(keys.begin(), keys.end(), "g") != keys.end());

  abctable.setField("a", "test");
  TEST_CHECK(abctable["a"] == std::string("test"));

  abctable.setField("a", 22);
  TEST_CHECK(abctable["a"] == 22);

  kaguya::LuaTable v = state.newTable();
  TEST_CHECK(v.setRawField("s", 2));
  TEST_CHECK(v.setRawField("s", "a"));
  TEST_CHECK(v.setRawField("s", std::string("a")));
  TEST_CHECK(v.setRawField("s", abctable));
  TEST_CHECK(v.setRawField("s", kaguya::NilValue()));

  kaguya::LuaStackRef a;
}
KAGUYA_TEST_FUNCTION_DEF(refcopy)(kaguya::State &state) {
  kaguya::LuaRef ref = state.newRef(2);
  kaguya::LuaRef ref2;
  kaguya::LuaRef ref3;
  TEST_CHECK(ref);
  TEST_CHECK(!ref2);

  ref2 = ref;
  TEST_CHECK(ref2);
  ref2 = ref3;
  TEST_CHECK(!ref2);
}
KAGUYA_TEST_FUNCTION_DEF(newtable)(kaguya::State &state) {
  kaguya::LuaTable ref = state.newTable();
  TEST_CHECK(kaguya::LuaRef(ref) == ref);
  TEST_CHECK(ref == kaguya::LuaRef(ref));

  ref["tbl"] = kaguya::NewTable();
  kaguya::LuaRef othertable = ref["tbl"];
  TEST_CHECK(othertable != ref);
  TEST_CHECK(ref != othertable);

  TEST_CHECK(othertable.map().size() == 0);
  TEST_CHECK(othertable.type() == kaguya::LuaRef::TYPE_TABLE);

  othertable["foo"] = 3;
  TEST_CHECK(othertable["foo"] == 3);
}

int free_standing_function() { return 12; }

KAGUYA_TEST_FUNCTION_DEF(callfunction)(kaguya::State &state) {

  int ret = state["math"]["abs"](-32);
  TEST_CHECK(ret == 32);

  kaguya::LuaTable globalTable = state.globalTable();
  globalTable["tbl"] = kaguya::NewTable();
  kaguya::LuaRef tbl = globalTable["tbl"];

  state("tbl.fun=function() return 1 end");
  TEST_CHECK(tbl.map().size() == 1);

  int result = tbl["fun"]();
  TEST_CHECK(result == 1);

  state("tbl.value=6");
  state("tbl.memfun=function(self) return self.value end");
  result = (tbl->*"memfun")(); //==tbl["memfun"](tbl) like tbl:memfun() in Lua

  TEST_CHECK(result == 6);

  globalTable["free2"] = kaguya::function(&free_standing_function);

  TEST_CHECK(state("assert(free2()==12)"));

  state("tbl.sum=function(...) local args = {...};"
        "local total=0;"
        "for i = 1, #args do "
        "  total = total + args[i];"
        " end "
        "return total;"
        "end");
  TEST_CHECK(tbl["sum"](1, 2, 3, 4, 5, 6, 7, 8) == 36);
  TEST_CHECK(tbl["sum"].call<int>(1, 2, 3, 4, 5, 6, 7, 8) == 36);

  // no return compile test
  tbl["sum"].call<void>(1, 2, 3, 4, 5, 6, 7, 8);

  state("tbl.retfun=function(tbl) return tbl.sum end");
  TEST_CHECK((tbl->*"retfun")()(1, 2, 3, 4, 5, 6, 7, 8) == 36)

  // callable check
  TEST_CHECK(
      (tbl->*"retfun").call<kaguya::LuaFunction>()(1, 2, 3, 4, 5, 6, 7, 8) ==
      36)
  TEST_CHECK((tbl->*"retfun")().call<int>(1, 2, 3, 4, 5, 6, 7, 8) == 36)
}

struct ob {
  operator bool() const { return true; }
};
KAGUYA_TEST_FUNCTION_DEF(test_operator_bool)(kaguya::State &state) {
  kaguya::LuaRef table1 = state.newTable();
  TEST_CHECK(ob() != table1);
  kaguya::LuaTable newTable2 = state.newTable();
  kaguya::LuaTable tabl1_ref_copy = table1;
  TEST_CHECK(newTable2 != table1);
  TEST_CHECK(ob() != newTable2);
  TEST_CHECK(!(newTable2 == table1));
  TEST_CHECK(table1 == tabl1_ref_copy);
}
KAGUYA_TEST_FUNCTION_DEF(typetest_function)(kaguya::State &state) {
  kaguya::LuaRef newTable = state.newTable();

  kaguya::LuaRef function =
      state.newRef(kaguya::function(&free_standing_function));
  kaguya::LuaRef function2 = function;
  TEST_CHECK(function != newTable);
  TEST_CHECK(function == function2);
}
KAGUYA_TEST_FUNCTION_DEF(operator_equal_test)(kaguya::State &state) {
  kaguya::LuaTable globalTable = state.globalTable();

  TEST_CHECK(!(globalTable != state.globalTable()));
  TEST_CHECK(globalTable == state.globalTable());

  kaguya::LuaRef luanum = state.newRef(422);
  kaguya::LuaRef luanum2 = state.newRef(42);
  kaguya::LuaRef luanum3 = state.newRef(422);

  TEST_COMPARE_EQ(luanum, luanum3);
  TEST_COMPARE_NE(luanum, luanum2);
  TEST_COMPARE_LT(luanum2, luanum);
  TEST_COMPARE_LE(luanum2, luanum);
  TEST_COMPARE_GT(luanum, luanum2);
  TEST_COMPARE_GE(luanum, luanum2);

  TEST_CHECK(!(luanum != luanum3));
  TEST_CHECK(!(luanum < luanum3));
  TEST_CHECK(luanum <= luanum3);
  TEST_CHECK(!(luanum > luanum3));
  TEST_CHECK(luanum >= luanum3);
}
KAGUYA_TEST_FUNCTION_DEF(typetest)(kaguya::State &state) {
  kaguya::LuaRef luanum = state.newRef(422);
  TEST_CHECK(luanum.typeTest<int>());
  TEST_CHECK(!luanum.typeTest<std::string>());
  TEST_CHECK(!luanum.typeTest<ob>());

  TEST_CHECK(luanum.weakTypeTest<std::string>());
  TEST_CHECK(!luanum.weakTypeTest<ob>());

  bool typevalid;
  TEST_EQUAL(luanum.get<std::string>(typevalid), "422");
  TEST_CHECK(typevalid);
  TEST_EQUAL(luanum.get<int>(typevalid), 422);
  TEST_CHECK(typevalid);
  luanum.get<std::string>(typevalid, false);
  TEST_CHECK(!typevalid);
}

struct CallbackTest {
  void callCallback(int x) {
    if (function_) {
      function_(x);
    }
  }
  void setCallback(kaguya::LuaRef fun) { function_ = fun; }
  kaguya::LuaRef function_;
};

KAGUYA_TEST_FUNCTION_DEF(luafun_callback)(kaguya::State &state) {

  state["CallbackTest"].setClass(
      kaguya::UserdataMetatable<CallbackTest>()
          .setConstructors<CallbackTest()>()
          .addFunction("setCallback", &CallbackTest::setCallback)
          .addFunction("callCallback", &CallbackTest::callCallback));

  state("callback = CallbackTest.new();"
        "receive_data ={};"
        "callback:setCallback(function(x) table.insert(receive_data,x) end);"
        "callback:callCallback(54)"
        "callback:callCallback(24)");

  (state["callback"]->*"callCallback")(454);
  kaguya::LuaTable callback_data = state["receive_data"];

  TEST_CHECK(callback_data[1] == 54 && callback_data[2] == 24 &&
             callback_data[3] == 454)
}
struct size_check_struct {
  char data[22];
};

KAGUYA_TEST_FUNCTION_DEF(lua_ref_size)(kaguya::State &state) {
#define TEST_SIZE(IDENTIFIER, VALUE, LEN)                                      \
  state(#IDENTIFIER " = " #VALUE);                                             \
  kaguya::LuaRef IDENTIFIER = state[#IDENTIFIER];                              \
  TEST_EQUAL(IDENTIFIER.size(), LEN);

  TEST_SIZE(x, 17, 0);
  TEST_SIZE(s, 'hello world!', 12);
  TEST_SIZE(b, true, 0);
  TEST_SIZE(n, nil, 0);
  TEST_SIZE(f, function() return 3 end, 0);

#undef TEST_SIZE

  state["d"] = size_check_struct();
  kaguya::LuaRef d = state["d"];
  TEST_CHECK(d.size() >= sizeof(size_check_struct)); // fixme: determine exact
                                                     // size of userdata if
                                                     // possible
}

KAGUYA_TEST_FUNCTION_DEF(function_env)(kaguya::State &state) {
  kaguya::LuaFunction fun = state.loadstring("foo='bar'");

  fun.setFunctionEnv(kaguya::NewTable());
  fun();

  TEST_CHECK(state("assert(foo == nil)"));
  state["functionEnv"] = fun.getFunctionEnv();
  TEST_CHECK(state("assert(functionEnv.foo == 'bar')"));
}

KAGUYA_TEST_FUNCTION_DEF(lua_table_get)(kaguya::State &state) {
  state("value={out=32,str='gjgj'}");
  state("value['in'] = 'test'");
  kaguya::LuaTable table = state["value"];

  TEST_CHECK(table["str"] == "gjgj" && table["in"] == "test" &&
             table["out"] == 32);

  TEST_CHECK(!table.isNilref());
}

KAGUYA_TEST_FUNCTION_DEF(lua_table_set)(kaguya::State &state) {
  kaguya::LuaTable globalTable = state.globalTable();
  state["value"] = kaguya::NewTable();
  state["value"]["abc"] = kaguya::NewTable();
  state["value"]["abc"]["def"] = 7;
  state["value"]["abc"]["bbb"] = "test";
  TEST_CHECK(state("assert(value.abc.def == 7 and value.abc.bbb == 'test')"));
}

KAGUYA_TEST_FUNCTION_DEF(lua_table_size)(kaguya::State &state) {
  kaguya::LuaTable table = state.newTable();
  TEST_CHECK(table.size() == 0);

  state("value = {1, 2, 'foo'}");
  kaguya::LuaTable table2 = state["value"];
  TEST_CHECK(table2.size() == 3);

  state("value[3] = nil");
  kaguya::LuaTable table3 = state["value"];
  TEST_CHECK(table3.size() == 2);

  state["value"] = kaguya::NewTable();
  state["value"][1] = "foo";
  state["value"][2] = "bar";
  state["value"][99] = "baz";
  state["value"]["index"] = "foo";
  kaguya::LuaTable table4 = state["value"];
  TEST_CHECK(table4.size() == 2);
}

KAGUYA_TEST_FUNCTION_DEF(lua_table_reference)(kaguya::State &state) {
  kaguya::LuaTable globalTable = state.globalTable();
  state["value"] = kaguya::NewTable();
  state["value"]["abc"] = kaguya::NewTable();
  state["value"]["abc"]["def"] = 7;
  TEST_CHECK(state("assert(value.abc.def == 7)"));
  state["value"]["abc"]["bbb"] = "test";
  TEST_CHECK(state("assert(value.abc.bbb == 'test')"));
  state["value"]["abc"]["ddd"] = state["value"]["abc"]["ccc"] =
      state["value"]["abc"]["bbb"];
  TEST_CHECK(state("assert(value.abc.ccc == 'test')"));
  TEST_CHECK(state("assert(value.abc.ddd == 'test')"));

  TEST_COMPARE_EQ(state["value"]["abc"]["ccc"], state["value"]["abc"]["bbb"]);
  TEST_COMPARE_EQ(state["value"]["abc"]["ccc"], "test");
  TEST_CHECK(!(state["value"]["abc"]["ccc"] != state["value"]["abc"]["bbb"]));
  TEST_CHECK(!(state["value"]["abc"]["ccc"] != "test"));
  TEST_COMPARE_NE(state["value"]["abc"]["ccc"], state["value"]["abc"]["def"]);
  TEST_COMPARE_NE(state["value"]["abc"]["ccc"], "tes");
}

KAGUYA_TEST_FUNCTION_DEF(metatable)(kaguya::State &state) {
  kaguya::LuaTable table = state.newTable();
  kaguya::LuaTable metatable = state.newTable();
  kaguya::LuaTable metatable_index = state.newTable();
  metatable_index["hana"] = "uta";
  metatable["__index"] = metatable_index;
  table.setMetatable(metatable);
  TEST_EQUAL(table["hana"], "uta");
  TEST_EQUAL(table.getMetatable(), metatable);
}
KAGUYA_TEST_FUNCTION_DEF(stream_out_test)(kaguya::State &state) {
  kaguya::LuaTable table = state.newTable();
  table[1] = 231;
  table[2] = 21;
  table[3] = 2;
  std::stringstream ss;
  std::string text;
  ss << table;
  text = ss.str();
  TEST_EQUAL(text, "{1=231,2=21,3=2}");

  ss.str("");
  ss << table[1];
  text = ss.str();
  TEST_EQUAL(text, "231");

  ss.str("");
  ss << state.newRef(323);
  text = ss.str();
  TEST_EQUAL(text, "323");

  ss.str("");
  ss << state.newRef("test_text");
  text = ss.str();
  TEST_EQUAL(text, "'test_text'");

  ss.str("");
  ss << state.newRef(true);
  text = ss.str();
  TEST_EQUAL(text, "true");
  ss.str("");
  ss << state.newRef(false);
  text = ss.str();
  TEST_EQUAL(text, "false");
  ss.str("");
  ss << state.newRef((void *)(0));
  text = ss.str();
  TEST_EQUAL(text, "nil");

  ss.str("");
  ss << state.newRef(&stream_out_test);
  text = ss.str();
  TEST_EQUAL(text, "function");

  ss.str("");
  ss << state.loadstring("return {1,2,3,4,5}")();
  text = ss.str();
  TEST_EQUAL(text, "{1=1,2=2,3=3,4=4,5=5}");

  ss.str("");
  ss << state.loadstring("return {table={1,2,3,4,5}}")();
  text = ss.str();
  TEST_EQUAL(text, "{'table'={...}}");

  ss.str("");
  ss << state.loadstring("return 2,3")();
  text = ss.str();
  TEST_EQUAL(text, "2,3");

  ss.str("");
  ss << state.loadstring("return 2,3")().result_at(0);
  text = ss.str();
  TEST_EQUAL(text, "2");

  ss.str("");
  ss << state.newRef(&ss);
  text = ss.str();
  TEST_CHECK(text.compare(0, strlen("userdata"), "userdata") == 0);

  bool catch_except = false;
  try {
    ss << state.loadstring("return 2,3")().result_at(2);
  } catch (const std::out_of_range &) {
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

KAGUYA_TEST_FUNCTION_DEF(get_field)(kaguya::State &state) {
  kaguya::LuaRef table = state.newTable();
  state["testtable"] = table;
  table.setField("3", 4);
  TEST_CHECK(state("assert(testtable['3'] == 4)"));
  table.setField(1, 32);
  TEST_CHECK(state("assert(testtable[1] == 32)"));

  TEST_EQUAL(table.getField("3"), 4);
  TEST_EQUAL(table[1], 32);
  TEST_EQUAL(table.getField(1), 32);
  const kaguya::LuaRef &const_table = table;
  TEST_EQUAL(const_table.getField("3"), 4);
  TEST_EQUAL(const_table[1], 32);
  TEST_EQUAL(const_table.getField(1), 32);
  TEST_EQUAL(const_table.getField<int>(state.newRef(1)), 32);
  TEST_EQUAL(const_table.getRawField(1), 32);
  TEST_EQUAL(const_table.getRawField<int>(state.newRef(1)), 32);
}

KAGUYA_TEST_FUNCTION_DEF(luafun_loadstring)(kaguya::State &state) {
  {
    kaguya::LuaRef f = kaguya::LuaFunction::loadstring(
        state.state(),
        "return function(table, key) return table['other_'..key] end")();
    kaguya::LuaTable table = state.newTable();
    table["x"] = 5;
    table["other_x"] = 55;
    int v = f(table, "x");
    TEST_EQUAL(v, 55);
  }

  {
    kaguya::LuaRef f = kaguya::LuaFunction::loadstring(
        state.state(), "return function(a) return 22,66 end")();

    kaguya::LuaRef forwardf = kaguya::LuaFunction::loadstring(
        state.state(), "return function(...) return ... end")();
    int a = 0;
    int b = 0;
    kaguya::tie(a, b) = f();
    TEST_EQUAL(a, 22);
    TEST_EQUAL(b, 66);
    a = 0;
    b = 0;
    kaguya::tie(a, b) = forwardf(f());
    TEST_EQUAL(a, 22);
    TEST_EQUAL(b, 66);
  }
}

KAGUYA_TEST_FUNCTION_DEF(put_multiple)(kaguya::State &state) {
  kaguya::LuaRef v = state.newRef(kaguya::standard::tuple<int, int>(32, 22));
  TEST_EQUAL(v, 32);
  TEST_CHECK(v);
  state["value"] = v;
  TEST_EQUAL(state["value"], 32);
  TEST_CHECK(state["value"]);
  TEST_CHECK(state("assert(value == 32)"));

  kaguya::LuaRef f = state.loadstring("return 22,66");
  state["value"] = f();
  TEST_EQUAL(state["value"], 22);
  TEST_CHECK(state["value"]);
  TEST_CHECK(state("assert(value == 22)"));
}

struct table_functor {
  void operator()(int, int) {}
};
struct breakable_table_functor {
  bool operator()(int, int) { return true; }
};

KAGUYA_TEST_FUNCTION_DEF(nostate_ref_error)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  kaguya::LuaRef v;
  kaguya::LuaRef thread = state.newThread();
  kaguya::LuaRef table = state.newTable();

  TEST_CHECK(!v.setMetatable(state.newTable()));
  TEST_CHECK(!v.getMetatable());

  TEST_CHECK(!v.setFunctionEnv(state.newTable()));
  TEST_CHECK(!v.getFunctionEnv());
  TEST_CHECK(!v.getField("s"));
  TEST_CHECK(!v.setField("s", 2));
  TEST_CHECK(!v.setField("s", "a"));
  TEST_CHECK(!v.setField("s", std::string("a")));
  TEST_CHECK(!v.setField("s", table));
  TEST_CHECK(!v.setField("s", kaguya::NilValue()));
  TEST_CHECK(!v.setField(kaguya::NilValue(), kaguya::NilValue()));

  TEST_CHECK(!v.setRawField("s", 2));
  TEST_CHECK(!v.setRawField("s", "a"));
  TEST_CHECK(!v.setRawField("s", std::string("a")));
  TEST_CHECK(!v.setRawField("s", table));
  TEST_CHECK(!v.setRawField("s", kaguya::NilValue()));
  TEST_CHECK(!v.setRawField(kaguya::NilValue(), kaguya::NilValue()));

  TEST_CHECK(!table.getFunctionEnv());
  TEST_CHECK(!thread.getMetatable());

  TEST_CHECK(!table.setFunctionEnv(kaguya::NewTable()));
  TEST_CHECK(!thread.setMetatable(state.newTable()));

  const kaguya::LuaRef cv;
  const kaguya::LuaRef cthread = state.newThread();
  const kaguya::LuaRef ctable = state.newTable();

  TEST_CHECK(!cv.getMetatable());

  TEST_CHECK(!cv.getFunctionEnv());
  TEST_CHECK(!cv.getField("s"));
  TEST_CHECK(!cv.getField<int>("s"));
  TEST_CHECK(!cv.getField<int>(kaguya::NewTable()));

  TEST_CHECK(!ctable.getFunctionEnv());
  TEST_CHECK(!cthread.getMetatable());

  ctable.foreach_table<int, int>(table_functor());
  ctable.foreach_table_breakable<int, int>(breakable_table_functor());
  cv.foreach_table<int, int>(table_functor());
  cv.foreach_table_breakable<int, int>(breakable_table_functor());

  last_error_message = "";
  kaguya::LuaRef ref = state.newTable();
  ref.costatus();
  TEST_CHECK(!last_error_message.empty());
}

struct UserDataTest {
  UserDataTest(int v) : m(v) {}
  int m;
};
struct UserDataTest2 {
  UserDataTest2(int v) : m(v) {}
  double m;
};

UserDataTest &UserDataTestFunction(UserDataTest &r) {
  r.m = 5;
  return r;
}
void UserDataTestFunction2(int) {}

KAGUYA_TEST_FUNCTION_DEF(typecheck_ref_error)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  {
    last_error_message = "";
    kaguya::LuaTable nottable = kaguya::LuaRef(state.newThread());
    TEST_COMPARE_NE(last_error_message, "");
  }
  {
    last_error_message = "";
    kaguya::LuaUserData notud = kaguya::LuaRef(state.newThread());
    TEST_COMPARE_NE(last_error_message, "");
  }
  {
    last_error_message = "";
    kaguya::LuaThread notud = kaguya::LuaRef(state.newTable());
    TEST_COMPARE_NE(last_error_message, "");
  }

  {
    last_error_message = "";
    kaguya::LuaRef udataref = state.newRef(UserDataTest(2));
    TEST_CHECK(udataref.weakTypeTest<kaguya::LuaUserData>());
    TEST_CHECK(udataref.typeTest<kaguya::LuaUserData>());
    TEST_CHECK(!udataref.weakTypeTest<kaguya::LuaTable>());
    TEST_CHECK(!udataref.typeTest<kaguya::LuaTable>());
    TEST_CHECK(udataref.typeTest<UserDataTest>());
    TEST_CHECK(!udataref.typeTest<UserDataTest2>());
    TEST_CHECK(!udataref.typeTest<bool>());
    kaguya::LuaUserData udata = udataref;
    TEST_EQUAL(last_error_message, "");
    UserDataTest d = udata;
    TEST_EQUAL(d.m, 2);
  }
  {
    last_error_message = "";
    kaguya::LuaUserData udata;
    TEST_CHECK(!udata);
    TEST_EQUAL(last_error_message, "");
  }
  {
    last_error_message = "";
    kaguya::LuaUserData udata(state.state());
    TEST_CHECK(!udata);
    TEST_EQUAL(last_error_message, "");
  }
  {
    last_error_message = "";
    kaguya::LuaUserData udata(state.state(), UserDataTest(2));
    TEST_CHECK(udata);
    TEST_CHECK(udata.typeTest<UserDataTest>());
    TEST_CHECK(!udata.typeTest<UserDataTest2>());
    UserDataTest d = udata;
    TEST_EQUAL(d.m, 2);
    TEST_EQUAL(last_error_message, "");
  }
  {
    last_error_message = "";
    UserDataTest udatan(2);
    kaguya::LuaUserData udata(state.state(), &udatan);
    TEST_CHECK(udata);
    UserDataTest *d = udata; // unsafe
    TEST_EQUAL(d->m, 2);

    const UserDataTest &dr = udata; // unsafe
    TEST_EQUAL(dr.m, 2);
    TEST_EQUAL(last_error_message, "");

    kaguya::LuaFunction f(
        state.state(),
        kaguya::overload(UserDataTestFunction, UserDataTestFunction2));
    f(udata);
    TEST_EQUAL(udatan.m, 5);
  }
  {
    last_error_message = "";
    UserDataTest udatan(2);
    kaguya::LuaUserData udata(state.state(), kaguya::standard::ref(udatan));
    TEST_CHECK(udata);
    UserDataTest *d = udata; // unsafe
    TEST_EQUAL(d->m, 2);

    const UserDataTest &dr = udata; // unsafe
    TEST_EQUAL(dr.m, 2);
    TEST_EQUAL(last_error_message, "");

    kaguya::LuaFunction f(
        state.state(),
        kaguya::overload(UserDataTestFunction, UserDataTestFunction2));
    f(udata);
    TEST_EQUAL(udatan.m, 5);
  }
}

KAGUYA_TEST_GROUP_END(test_05_lua_ref)
