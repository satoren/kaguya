#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_03_function)
using namespace kaguya_test_util;

int arg = 0;
void free_standing_function(int r) { arg = r; }
int free_standing_function2() { return 12; }
int free_standing_function3(const char *text) { return atoi(text); }

KAGUYA_TEST_FUNCTION_DEF(free_standing_function_test)(kaguya::State &state) {
  state["ABC"] = &free_standing_function;
  state["ABC"](54);
  state["free2"] = &free_standing_function2;
  TEST_EQUAL(arg, 54);
  TEST_CHECK(state["free2"]() == 12.0);

  state["free3"] = &free_standing_function3;
  TEST_CHECK(state["free3"]("54") == 54.0);
}

kaguya::standard::tuple<int, std::string> tuplefun() {
  return kaguya::standard::tuple<int, std::string>(12, "23");
}

KAGUYA_TEST_FUNCTION_DEF(multi_return_function_test)(kaguya::State &state) {
  state["multresfun2"] = kaguya::function(tuplefun);

  state("a,b = multresfun2()");
  TEST_CHECK(state("assert(a == 12  and b == '23')"));
}

void pointerfun(void *pointer) { TEST_EQUAL(pointer, 0); }
void const_pointerfun(const void *pointer) { TEST_EQUAL(pointer, 0); }
std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}
KAGUYA_TEST_FUNCTION_DEF(nil_to_nullpointer)(kaguya::State &state) {
  state["pointerfun"] = kaguya::function(pointerfun);
  TEST_CHECK(state("pointerfun(nil)"));
  state["const_pointerfun"] = kaguya::function(const_pointerfun);
  TEST_CHECK(state("const_pointerfun(nil)"));

  state.setErrorHandler(ignore_error_fun);
  TEST_CHECK(!state("pointerfun(32)"));    // is error
  TEST_CHECK(!state("pointerfun('232')")); // is error
}
KAGUYA_TEST_FUNCTION_DEF(noargs_to_nullpointer)(kaguya::State &state) {
  state["pointerfun"] = kaguya::function(pointerfun);
  TEST_CHECK(state("pointerfun()"));
  state["const_pointerfun"] = kaguya::function(const_pointerfun);
  TEST_CHECK(state("const_pointerfun()"));
}

struct Foo {
  std::string bar;
  void setBar(std::string b) { bar = b; }
};
struct Bar {
  std::string member;
  Bar() {}
  Bar(const std::string &data) : member(data) {}
};
KAGUYA_TEST_FUNCTION_DEF(native_function_call_test)(kaguya::State &state) {
  using namespace kaguya::nativefunction;
  Foo foo;
  state.newRef(6).push();
  state.newRef(9).push();
  state.newRef(2).push();
  int getarg = getArgument<0, void(int, int, int)>(state.state());
  TEST_EQUAL(getarg, 6);
  getarg = getArgument<1, void(int, int, int)>(state.state());
  TEST_EQUAL(getarg, 9);
  getarg = getArgument<2, void(int, int, int)>(state.state());
  TEST_EQUAL(getarg, 2);

  call(state.state(), &free_standing_function);

  lua_settop(state.state(), 0);
  state.newRef(&foo).push();
  state.newRef("Bar").push();
  call(state.state(), &Foo::setBar);

  lua_settop(state.state(), 0);
#if KAGUYA_USE_CPP11
  state.newRef(&foo).push();
  state.newRef(9).push();
  call(state.state(), [](Foo *foo, int b) {
    TEST_EQUAL(b, 9);
    foo->setBar("fromlambda");
  });
  TEST_EQUAL(foo.bar, "fromlambda");

  std::string capture("capture");
  call(state.state(), [=](Foo *foo, int b) {
    TEST_EQUAL(b, 9);
    foo->setBar(capture + "lambda");
  });
  TEST_EQUAL(foo.bar, "capturelambda");
#endif

  lua_settop(state.state(), 0);
}

int overload1() { return 1; }
int overload2(const std::string &) { return 2; }
int overload3(int) { return 3; }

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
int overload4(const std::vector<int> &) { return 4; }
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
int overload5(const std::map<std::string, std::string> &) { return 5; }
// This function after the first to trigger weak type test
int overload6(const std::map<std::string, std::string> &, int b) { return b; }
#endif
int overload7(void *) { return 7; }

int overload8(const Foo *) { return 8; }
int overload9(Bar *) { return 9; }
int overload10(const kaguya::standard::function<int()> fn) {
  TEST_EQUAL(fn(), 1);
  return 10;
}

KAGUYA_TEST_FUNCTION_DEF(overload)(kaguya::State &state) {
  state["overloaded_function"] =
      kaguya::overload(overload1, overload2, overload3
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
                       ,
                       overload4
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
                       ,
                       overload5, overload6
#endif
                       ,
                       overload7, overload8, overload9);
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
  TEST_EQUAL(f((void *)0), 7);
  TEST_CHECK(state("assert(overloaded_function(nil) == 7)"));

  state["Foo"].setClass(kaguya::UserdataMetatable<Foo>());
  state["Bar"].setClass(kaguya::UserdataMetatable<Bar>());

  TEST_EQUAL(f(Foo()), 8);
  TEST_EQUAL(f(Bar()), 9);

  state["overloaded_function2"] =
      kaguya::overload(overload1, overload2, overload3, overload10);
  kaguya::LuaFunction f2 = state["overloaded_function2"];
  TEST_EQUAL(f2(kaguya::standard::function<int()>(overload1)), 10);
}

KAGUYA_TEST_FUNCTION_DEF(result_to_table)(kaguya::State &state) {
  state["result_to_table"] = kaguya::function(overload1);
  state["result"] = state["result_to_table"]();
  TEST_EQUAL(state["result"], 1);
}

void nested_function_call(kaguya::LuaStackRef function,
                          kaguya::LuaStackRef value,
                          kaguya::LuaStackRef value2) {
  function(value, value2);
}

KAGUYA_TEST_FUNCTION_DEF(nested_function_call_test)(kaguya::State &state) {
  state["nested_function_call"] = kaguya::function(nested_function_call);

  state("nested_function_call(function(value,value2) assert(value == "
        "32);assert(value2 == 'text') end,32,'text')");
}

void nested_function_call2(kaguya::LuaStackRef function,
                           const kaguya::LuaStackRef &table,
                           kaguya::LuaStackRef value2) {
  function(table["key"], value2);
}

KAGUYA_TEST_FUNCTION_DEF(nested_function_call_test2)(kaguya::State &state) {
  state["nested_function_call"] = kaguya::function(nested_function_call2);

  state("nested_function_call(function(value,value2) "
        "assert(value==32);assert(value2 == 'text') end,{key=32},'text')");
}

namespace functioncall_testfunction {
int called = 0;
void testcall() { called = 1; }
void testcall2(int) { called = 2; }
}

KAGUYA_TEST_FUNCTION_DEF(functioncall)(kaguya::State &state) {
  using namespace functioncall_testfunction;
  kaguya::standard::function<void()> f = testcall;
  kaguya::standard::function<void(int)> f2 = testcall2;
  state["testcall"] = &testcall;
  state["testcall"]();
  TEST_EQUAL(called, 1);
  called = 0;

  state["testcall"] = kaguya::function(f);
  state["testcall"]();
  TEST_EQUAL(called, 1);

  state["overloaded"] = kaguya::overload(testcall2, testcall, f, f2);
  state["overloaded"]();
  TEST_EQUAL(called, 1);
  state["overloaded"](1);
  TEST_EQUAL(called, 2);
  state["overloaded"](1, 2);
  TEST_EQUAL(called, 2);
  state["overloaded"]("test");
  TEST_EQUAL(called, 1);
}

int cfunction(lua_State *L) {
  TEST_EQUAL(lua_tointeger(L, 1), 3);
  return 0;
}

int coroutinefn(lua_State *cor) {
  using namespace kaguya;
  lua_resume(cor, 0, 0);
  return static_cast<int>(lua_tointeger(cor, 1));
}

KAGUYA_TEST_FUNCTION_DEF(luacfunction)(kaguya::State &state) {
  state["cfunction"] = kaguya::luacfunction(&cfunction);

  state.dostring("cfunction(3)");

  state["coroutinefn"] = kaguya::function(&coroutinefn);
  state.dostring(
      "assert(coroutinefn( coroutine.create(function () return 8 end) ) == 8)");

  state["coroutinefn"] = &coroutinefn;
  state.dostring(
      "assert(coroutinefn( coroutine.create(function () return 3 end) ) == 3)");
}

int defargfn(int a = 3, int b = 2, int c = 1) { return a * b * c; }

KAGUYA_FUNCTION_OVERLOADS(defargfn_wrapper, defargfn, 0, 3)

KAGUYA_TEST_FUNCTION_DEF(defaultarguments)(kaguya::State &state) {
  state["defargfn"] = kaguya::function(defargfn_wrapper());

  state.dostring("assert(defargfn() == 6)");
  state.dostring("assert(defargfn(6) == 12)");
  state.dostring("assert(defargfn(6,5) == 30)");
  state.dostring("assert(defargfn(2,2,2) == 8)");
}

KAGUYA_MEMBER_FUNCTION_OVERLOADS(mem_defargfn_wrapper, TestClass, default_arg,
                                 0, 3)
KAGUYA_TEST_FUNCTION_DEF(member_function_defaultarguments)
(kaguya::State &state) {

  state["TestClass"].setClass(
      kaguya::UserdataMetatable<TestClass>()
          .setConstructors<TestClass()>()
          .addFunction("defargfn", mem_defargfn_wrapper()));

  state.dostring("test = TestClass.new()");

  state.dostring("assert(test:defargfn() == 6)");
  state.dostring("assert(test:defargfn(6) == 12)");
  state.dostring("assert(test:defargfn(6,5) == 30)");
  state.dostring("assert(test:defargfn(2,2,2) == 8)");
}

void void_defargfn(int a = 3, int b = 2, int c = 1) {
  TEST_EQUAL(a, 3);
  TEST_EQUAL(b, 2);
  TEST_EQUAL(c, 1);
}

KAGUYA_FUNCTION_OVERLOADS(void_defargfn_wrapper, void_defargfn, 0, 3)

KAGUYA_TEST_FUNCTION_DEF(void_defaultarguments)(kaguya::State &state) {
  state["defargfn"] = kaguya::function(void_defargfn_wrapper());

  state.dostring("defargfn()");
  state.dostring("defargfn(3)");
  state.dostring("defargfn(3,2)");
  state.dostring("defargfn(3,2,1)");
}

KAGUYA_MEMBER_FUNCTION_OVERLOADS(default_set_wrapper, TestClass, default_set, 0,
                                 3)

KAGUYA_TEST_FUNCTION_DEF(member_void_function_defaultarguments)
(kaguya::State &state) {

  state["TestClass"].setClass(
      kaguya::UserdataMetatable<TestClass>()
          .setConstructors<TestClass()>()
          .addFunction("defargfn", default_set_wrapper())
          .addFunction("getInt", &TestClass::getInt));

  state.dostring("test = TestClass.new()");

  state.dostring("test:defargfn() assert(test:getInt() == 6)");
  state.dostring("test:defargfn(6) assert(test:getInt() == 12)");
  state.dostring("test:defargfn(6,5) assert(test:getInt() == 30)");
  state.dostring("test:defargfn(2,2,2) assert(test:getInt() == 8)");
}

std::string defargfn2(std::string a, std::string b = "b", std::string c = "c") {
  return a + b + c;
}

KAGUYA_FUNCTION_OVERLOADS(defargfn2_wrapper, defargfn2, 1, 3)

KAGUYA_TEST_FUNCTION_DEF(defaultarguments_overload)(kaguya::State &state) {
  state["defargfn"] = kaguya::overload(defargfn_wrapper(), defargfn2_wrapper());

  state.dostring("assert(defargfn() == 6)");
  state.dostring("assert(defargfn(6) == 12)");
  state.dostring("assert(defargfn(6,5) == 30)");
  state.dostring("assert(defargfn(2,2,2) == 8)");

  state.dostring("assert(defargfn('a') == 'abc')");
  state.dostring("assert(defargfn('a','b') == 'abc')");
  state.dostring("assert(defargfn('a','b','c') == 'abc')");
  state.dostring("assert(defargfn('d','e','f') == 'def')");
}

KAGUYA_FUNCTION_OVERLOADS_WITH_SIGNATURE(defargfn_wrapper_with_sig, defargfn, 0,
                                         3, int(int, int, int))

KAGUYA_TEST_FUNCTION_DEF(defaultarguments_overload_with_sig)
(kaguya::State &state) {
  state["defargfn"] = kaguya::function(defargfn_wrapper_with_sig());

  state.dostring("assert(defargfn() == 6)");
  state.dostring("assert(defargfn(6) == 12)");
  state.dostring("assert(defargfn(6,5) == 30)");
  state.dostring("assert(defargfn(2,2,2) == 8)");
}

KAGUYA_MEMBER_FUNCTION_OVERLOADS_WITH_SIGNATURE(mem_defargfn_wrapper_with_sig,
                                                TestClass, default_arg, 0, 3,
                                                int (TestClass::*)(int, int,
                                                                   int))

KAGUYA_TEST_FUNCTION_DEF(member_function_defaultarguments_with_sig)
(kaguya::State &state) {

  state["TestClass"].setClass(
      kaguya::UserdataMetatable<TestClass>()
          .setConstructors<TestClass()>()
          .addFunction("defargfn", mem_defargfn_wrapper_with_sig()));

  state.dostring("test = TestClass.new()");

  state.dostring("assert(test:defargfn() == 6)");
  state.dostring("assert(test:defargfn(6) == 12)");
  state.dostring("assert(test:defargfn(6,5) == 30)");
  state.dostring("assert(test:defargfn(2,2,2) == 8)");
}

KAGUYA_MEMBER_FUNCTION_OVERLOADS_WITH_SIGNATURE(
    TestClass_default_set_overload_int, TestClass, default_set_overload, 1, 3,
    void (TestClass::*)(int, int, int))
KAGUYA_MEMBER_FUNCTION_OVERLOADS_WITH_SIGNATURE(
    TestClass_default_set_overload_str, TestClass, default_set_overload, 1, 3,
    void (TestClass::*)(std::string, std::string, std::string))

KAGUYA_TEST_FUNCTION_DEF(member_function_different_type_overload)
(kaguya::State &state) {

  state["TestClass"].setClass(
      kaguya::UserdataMetatable<TestClass>()
          .setConstructors<TestClass()>()
          .addOverloadedFunctions("defargfn",
                                  TestClass_default_set_overload_int(),
                                  TestClass_default_set_overload_str())
          .addFunction("getInt", &TestClass::getInt)
          .addFunction("getString", &TestClass::getString));

  state["defargfn"] = kaguya::overload(defargfn_wrapper(), defargfn2_wrapper());

  state.dostring("test = TestClass.new()");

  state.dostring("test:defargfn(6) assert(test:getInt() == 12)");
  state.dostring("test:defargfn(6,5) assert(test:getInt() == 30)");
  state.dostring("test:defargfn(2,2,2) assert(test:getInt() == 8)");

  state.dostring("test:defargfn('a') assert(test:getString() == 'abc')");
  state.dostring("test:defargfn('a','a') assert(test:getString() == 'aac')");
  state.dostring(
      "test:defargfn('a','a','a') assert(test:getString() == 'aaa')");
}

std::string defargfn3(const std::string &a, const std::string &b = "b",
                      const std::string &c = "c") {
  return a + b + c;
}
KAGUYA_FUNCTION_OVERLOADS(defargfn3_wrapper, defargfn3, 1, 3)

KAGUYA_TEST_FUNCTION_DEF(defaultarguments_const_reference)
(kaguya::State &state) {
  state["defargfn"] = kaguya::function(defargfn3_wrapper());

  state.dostring("assert(defargfn('a') == 'abc')");
  state.dostring("assert(defargfn('a','b') == 'abc')");
  state.dostring("assert(defargfn('a','b','c') == 'abc')");
  state.dostring("assert(defargfn('d','e','f') == 'def')");
}

std::string defargfn4(const std::string &a, const char *b = "b",
                      const char *c = "c") {
  return a + b + c;
}

KAGUYA_FUNCTION_OVERLOADS(defargfn4_wrapper, defargfn4, 1, 3)
KAGUYA_TEST_FUNCTION_DEF(defaultarguments_const_char)(kaguya::State &state) {
  state["defargfn"] = kaguya::function(defargfn4_wrapper());

  state.dostring("assert(defargfn('a') == 'abc')");
  state.dostring("assert(defargfn('a','b') == 'abc')");
  state.dostring("assert(defargfn('a','b','c') == 'abc')");
  state.dostring("assert(defargfn('d','e','f') == 'def')");
}

kaguya::TableData ret_table() {
  using namespace kaguya;
  kaguya::TableData ret;

  ret.elements.push_back(TableDataElement("data", "value"));
  ret.elements.push_back(TableDataElement("num", 32));

  return ret;
}

kaguya::LuaCodeChunkResult ret_from_luacode() {
  return kaguya::LuaCodeChunkResult("return 1");
}

KAGUYA_TEST_FUNCTION_DEF(return_value_type)(kaguya::State &state) {
  state["ret_table"] = &ret_table;
  state("assert(ret_table().data == 'value')");
  state("assert(ret_table().num == 32)");

  state["ret_from_luacode"] = &ret_from_luacode;
  state("assert(ret_from_luacode() == 1)");
}

void two_string_fn(std::string a, std::string b) {
  TEST_EQUAL(a, "test1");
  TEST_EQUAL(b, "test2");
}
void two_cr_string_fn(const std::string &a, const std::string &b) {
  TEST_EQUAL(a, "test1");
  TEST_EQUAL(b, "test2");
}

KAGUYA_TEST_FUNCTION_DEF(none_to_string)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);
  state["two_string_fn"] = kaguya::function(two_string_fn);
  state["two_cr_string_fn"] = kaguya::function(two_cr_string_fn);

  last_error_message = "";
  TEST_CHECK(state.dostring("two_string_fn('test1','test2')"));
  TEST_CHECK(last_error_message == "");

  last_error_message = "";
  TEST_CHECK(!state.dostring("two_string_fn('error_test')"));
  TEST_CHECK(last_error_message != "");

  last_error_message = "";
  TEST_CHECK(state.dostring("two_cr_string_fn('test1','test2')"));
  TEST_CHECK(last_error_message == "");

  last_error_message = "";
  TEST_CHECK(!state.dostring("two_cr_string_fn('error_test')"));
  TEST_CHECK(last_error_message != "");
}

KAGUYA_TEST_GROUP_END(test_03_function)
