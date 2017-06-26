#include "kaguya/kaguya.hpp"
#include "../test_util.hpp"
#include "test_shared_class.hpp"

KAGUYA_TEST_GROUP_START(test_shared_library)

KAGUYA_TEST_FUNCTION_DEF(multi_shared_lib)(kaguya::State &L) {
  bool ret =
      L.dostring("package.cpath = package.cpath .. ';?.dylib' "
                 "test_shared_class_reg = require('test_shared_class_reg') "
                 "test_shared_class_use = require('test_shared_class_use') "
                 "local a = test_shared_class_reg.A(32) "
                 "local b = test_shared_class_reg.B() "
                 "assert(a.data == 32) "
                 "assert(test_shared_class_use.A_fn(a) == 32) "
                 "assert(test_shared_class_use.B_fn(b) == 22) "
                 "assert(test_shared_class_use.create_A().data == 1) ");

  TEST_CHECK(ret);
}

void ignore_error_fun(int, const char *) {}

KAGUYA_TEST_FUNCTION_DEF(typemismatch_test)(kaguya::State &L) {

  L.setErrorHandler(ignore_error_fun);
  bool ret =
      L.dostring("package.cpath = package.cpath .. ';?.dylib' "
                 "test_shared_class_reg = require('test_shared_class_reg') "
                 "test_shared_class_use = require('test_shared_class_use') ");
  TEST_CHECK(ret);
  TEST_EQUAL(false,
             L.dostring("local b = test_shared_class_reg.B(); "
                        "test_shared_class_use.A_fn(b)"));
}

KAGUYA_TEST_FUNCTION_DEF(create_self)(kaguya::State &L) {

  bool ret =
      L.dostring("package.cpath = package.cpath .. ';?.dylib' "
                 "test_shared_class_reg = require('test_shared_class_reg') "
                 "test_shared_class_use = require('test_shared_class_use') ");
  TEST_CHECK(ret);

  L["a"] = A(43);
  TEST_CHECK(L.dostring("assert(a.data == 43)"));
  DerivedB derived_b;
  L["derived_b"] = derived_b;
  TEST_CHECK(L.dostring("assert(test_shared_class_use.B_fn(derived_b) == 22)"));
  TEST_CHECK(
      L.dostring("assert(test_shared_class_use.DerivedB_fn(derived_b) == 33)"));
}

KAGUYA_TEST_GROUP_END(test_shared_library)
#include "../test_main.cpp"
