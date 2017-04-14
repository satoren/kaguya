#define KAGUYA_FUNCTION_MAX_ARGS 20
#define KAGUYA_FUNCTION_MAX_OVERLOADS 25
#include "kaguya/another_binding_api.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_20_max_arg_20)
using namespace kaguya_test_util;
using namespace kaguya;

void defargfn(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
              int a9, int a10 = 10, int a11 = 11, int a12 = 12, int a13 = 13,
              int a14 = 14, int a15 = 15, int a16 = 16, int a17 = 17,
              int a18 = 18, int a19 = 19, int a20 = 20) {
  TEST_EQUAL(a1, 1);
  TEST_EQUAL(a2, 2);
  TEST_EQUAL(a3, 3);
  TEST_EQUAL(a4, 4);
  TEST_EQUAL(a5, 5);
  TEST_EQUAL(a6, 6);
  TEST_EQUAL(a7, 7);
  TEST_EQUAL(a8, 8);
  TEST_EQUAL(a9, 9);
  TEST_EQUAL(a10, 10);
  TEST_EQUAL(a11, 11);
  TEST_EQUAL(a12, 12);
  TEST_EQUAL(a13, 13);
  TEST_EQUAL(a14, 14);
  TEST_EQUAL(a15, 15);
  TEST_EQUAL(a16, 16);
  TEST_EQUAL(a17, 17);
  TEST_EQUAL(a18, 18);
  TEST_EQUAL(a19, 19);
  TEST_EQUAL(a20, 20);
}

KAGUYA_TEST_FUNCTION_DEF(many_arg_fn)(kaguya::State &state) {
  state["f"] = kaguya::function(&defargfn);
  state.dostring("f(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)");
}
KAGUYA_FUNCTION_OVERLOADS(defargfn_wrapper, defargfn, 10, 20)

KAGUYA_TEST_FUNCTION_DEF(defaultarguments)(kaguya::State &state) {
  state["defargfn"] = kaguya::function(defargfn_wrapper());
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18)");
  state.dostring("defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19)");
  state.dostring(
      "defargfn(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)");
}

KAGUYA_TEST_FUNCTION_DEF(many_overloads)(kaguya::State &state) {
  state["defargfn"] =
      kaguya::UserdataMetatable<TestClass>()
          .setConstructors<TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass(), TestClass(), TestClass(), TestClass(),
                           TestClass()>();
}

KAGUYA_TEST_GROUP_END(test_20_max_arg_20)
