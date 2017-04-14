#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

namespace {
struct ProxyClassTest {
  ProxyClassTest(const std::string &v)
      : str(v), num(0), object(0), vec_ptr(0) {}
  ProxyClassTest(const int &v) : str(), num(v), object(0), vec_ptr(0) {}

  ProxyClassTest(kaguya_test_util::TestClass *v)
      : str(), num(0), object(v), vec_ptr(0) {}

  ProxyClassTest(const std::vector<int> &v)
      : str(), num(0), object(0), vec_ptr(&v) {}

  std::string str;
  int num;
  kaguya_test_util::TestClass *object;
  const std::vector<int> *vec_ptr;
};
}

namespace kaguya {
template <>
struct lua_type_traits<ProxyClassTest>
    : util::ConvertibleRegisterHelper<
          util::ConvertibleRegisterHelperProxy<ProxyClassTest>, int,
          std::string, kaguya_test_util::TestClass *,
          const std::vector<int> &> {};
}

KAGUYA_TEST_GROUP_START(test_09_utility)

using namespace kaguya_test_util;

std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(lua_resume_test)(kaguya::State &s) {
  using namespace kaguya;

  LuaThread t = s.newThread(s.loadstring("v={...}"));

  lua_State *co = t;
  lua_pushnumber(co, 2);
  lua_pushnumber(co, 3);

  lua_resume(co, s.state(), 2);

  TEST_EQUAL(s["v"][1], 2);
  TEST_EQUAL(s["v"][2], 3);
}
KAGUYA_TEST_FUNCTION_DEF(lua_compare_test)(kaguya::State &s) {
  using namespace kaguya;

  s.pushToStack(2);
  s.pushToStack(2);
  TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPEQ));
  TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLE));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLT));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2)); // invalid option
  s.popFromStack();
  s.popFromStack();

  s.pushToStack(4);
  s.pushToStack(2);
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPEQ));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLT));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2)); // invalid option
  s.popFromStack();
  s.popFromStack();

  s.pushToStack(2);
  s.pushToStack(4);
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPEQ));
  TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLE));
  TEST_CHECK(lua_compare(s.state(), -2, -1, LUA_OPLT));
  TEST_CHECK(!lua_compare(s.state(), -2, -1, LUA_OPLE + 2)); // invalid option
  s.popFromStack();
  s.popFromStack();
}

int lua_mylibf(lua_State *L) {
  kaguya::State state(L);
  kaguya::LuaTable t = state.newTable();
  t["value"] = 111;
  return t.push();
}

KAGUYA_TEST_FUNCTION_DEF(luaL_requiref_test)(kaguya::State &s) {
  using namespace kaguya;

  luaL_requiref(s.state(), "mylib", lua_mylibf, false);
  kaguya::LuaStackRef ref(s.state(), -1, true);
  TEST_EQUAL(ref["value"], 111);
  TEST_CHECK(s("assert(mylib == nil)"));
}

KAGUYA_TEST_FUNCTION_DEF(stackValueDump)(kaguya::State &s) {
  using namespace kaguya;
  util::stackValueDump(std::cout, s.state(), 4, 2);
}

struct A {
  int a[4];
};
KAGUYA_TEST_FUNCTION_DEF(lua_rawlen_test)(kaguya::State &s) {
  using namespace kaguya;

  s.pushToStack("string");
  TEST_EQUAL(lua_rawlen(s.state(), -1), 6);

  LuaTable tbl = s.newTable();
  tbl["2"] = 3;
  tbl["1"] = 3;
  s.pushToStack(tbl);
  TEST_EQUAL(lua_rawlen(s.state(), -1), 0);

  s.pushToStack(A());
  TEST_COMPARE_GE(lua_rawlen(s.state(), -1), sizeof(A));

  A a;
  s.pushToStack(&a);
  TEST_EQUAL(lua_type(s.state(), -1), LUA_TLIGHTUSERDATA);
  TEST_COMPARE_EQ(lua_rawlen(s.state(), -1), 0);

  s.pushToStack(2);
  TEST_EQUAL(lua_rawlen(s.state(), -1), 0);

  lua_settop(s.state(), 0);
}

void checkf() {}

template <typename F> void function_argcount_check(F, size_t count) {
  TEST_EQUAL(kaguya::util::FunctionSignature<F>::type::argument_count, count);
}

template <typename RET, typename F> void function_return_type_check(F) {
  bool ret = kaguya::standard::is_same<
      typename kaguya::util::FunctionSignature<F>::type::result_type,
      RET>::value;
  TEST_CHECK(ret);
}
KAGUYA_TEST_FUNCTION_DEF(check_function_signature)(kaguya::State &s) {
  using namespace kaguya;

  standard::function<void()> stdfn(checkf);

  TEST_CHECK(nativefunction::checkArgTypes(s.state(), checkf));
  TEST_CHECK(nativefunction::strictCheckArgTypes(s.state(), checkf));
  TEST_EQUAL(nativefunction::argTypesName(checkf), std::string(""));
  TEST_EQUAL(nativefunction::minArgCount(checkf), 0);
  TEST_EQUAL(nativefunction::maxArgCount(checkf), 0);

  TEST_CHECK(nativefunction::checkArgTypes(s.state(), &checkf));
  TEST_CHECK(nativefunction::strictCheckArgTypes(s.state(), &checkf));
  TEST_EQUAL(nativefunction::argTypesName(&checkf), std::string(""));
  TEST_EQUAL(nativefunction::minArgCount(&checkf), 0);
  TEST_EQUAL(nativefunction::maxArgCount(&checkf), 0);

  TEST_CHECK(nativefunction::checkArgTypes(s.state(), stdfn));
  TEST_CHECK(nativefunction::strictCheckArgTypes(s.state(), stdfn));
  TEST_EQUAL(nativefunction::argTypesName(stdfn), std::string(""));
  TEST_EQUAL(nativefunction::minArgCount(stdfn), 0);
  TEST_EQUAL(nativefunction::maxArgCount(stdfn), 0);

  bool ret = traits::is_same<int, int>::value;
  TEST_CHECK(ret);
  ret =
      traits::is_same<int, util::ArgumentType<0, void(int, int)>::type>::value;
  TEST_CHECK(ret);
  ret = traits::is_same<
      std::string, util::ArgumentType<1, void(int, std::string)>::type>::value;
  TEST_CHECK(ret);

  function_argcount_check(checkf, 0);
  function_argcount_check(&TestClass::default_arg, 4);
  function_argcount_check(stdfn, 0);

  function_return_type_check<void>(checkf);
  function_return_type_check<int>(&TestClass::default_arg);
  function_return_type_check<void>(stdfn);
}

KAGUYA_TEST_FUNCTION_DEF(preprocessor)(kaguya::State &) {
  TEST_EQUAL(KAGUYA_PP_INC(1), 2);
  TEST_EQUAL(KAGUYA_PP_INC(2), 3);
  TEST_EQUAL(KAGUYA_PP_ADD(1, 2), 3);
  TEST_EQUAL(KAGUYA_PP_SUB(3, 2), 1);
  TEST_EQUAL(KAGUYA_PP_SUB(3, 3), 0);
  TEST_EQUAL(KAGUYA_PP_SUB(3, 0), 3);
}
KAGUYA_TEST_FUNCTION_DEF(pretty_type_name)(kaguya::State &) {
  using kaguya::util::pretty_name;

  TEST_EQUAL(pretty_name(typeid(int)), "int");
  TEST_EQUAL(pretty_name(typeid(long)), "long");
}

void proxy_class_test_func(ProxyClassTest t) { TEST_EQUAL(t.str, "test"); }
void proxy_class_test_func2(const ProxyClassTest &t) { TEST_EQUAL(t.num, 5); }
void proxy_class_test_func3(const ProxyClassTest &t) {
  TEST_EQUAL(t.object->stringmember, "test");
}
void proxy_class_test_func_array(const ProxyClassTest &t) {
  std::vector<int> v;
  v.push_back(0);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  TEST_CHECK(*t.vec_ptr == v);
}

KAGUYA_TEST_FUNCTION_DEF(proxy_class_test)(kaguya::State &state) {
  state["testf"] = &proxy_class_test_func;
  TEST_CHECK(state("testf('test')"));
  state["testf2"] = &proxy_class_test_func2;
  TEST_CHECK(state("testf2(5)"));

  state.setErrorHandler(ignore_error_fun);
  TEST_CHECK(!state("testf2()"));

  state["TestClass"].setClass(kaguya::UserdataMetatable<TestClass>()
                                  .setConstructors<TestClass(std::string)>());
  state["testf3"] = &proxy_class_test_func3;

  TEST_CHECK(state("testf3(TestClass.new('test'))"));

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
  state["testf4"] = &proxy_class_test_func_array;
  TEST_CHECK(state("testf4({0,1,2,3})"));
#endif
}

KAGUYA_TEST_GROUP_END(test_09_utility)
