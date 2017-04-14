#include "kaguya/another_binding_api.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_13_predefined_binding_api)
using namespace kaguya_test_util;

using namespace kaguya;

int squared(int a) { return a * a; }

enum ENUM_TEST_TYPE { ENUM_TEST_A = 2, ENUM_TEST_B = 4 };

KAGUYA_BINDINGS(test_bind) {
  class_<TestClass>("TestClass")
      .constructor<int>()
      .def("getInt", &TestClass::getInt);

  {
    scope newscope("submodule");
    def("squared", &squared);
    {
      scope newscope("submodule");
      def("squared3", &squared);
    }
  }
  function("squared", &squared);
  scope().attr("x") = 1;

  enum_<ENUM_TEST_TYPE>("ENUM_TEST_TYPE")
      .value("ENUM_TEST_A", ENUM_TEST_A)
      .value("ENUM_TEST_B", ENUM_TEST_B);
}

KAGUYA_TEST_FUNCTION_DEF(int_constructor)(kaguya::State &state) {
  state.openlib("test_bind", &luaopen_test_bind);

  TEST_CHECK(state("value = assert(test_bind.TestClass.new(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));
  TEST_CHECK(state("assert(test_bind.squared(3) == 9)"));
  TEST_CHECK(state("assert(test_bind.submodule.squared(6) == 36)"));
  TEST_CHECK(state("assert(test_bind.submodule.submodule.squared3(6) == 36)"));

  TEST_CHECK(state("assert(test_bind.x == 1)"))
  TEST_CHECK(state("assert(test_bind.ENUM_TEST_TYPE.ENUM_TEST_A == 2)"));
  TEST_CHECK(state("assert(test_bind.ENUM_TEST_TYPE.ENUM_TEST_B == 4)"));
}

KAGUYA_TEST_GROUP_END(test_13_predefined_binding_api)
