#include "kaguya/another_binding_api.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_14_error_message)
using namespace kaguya_test_util;
using namespace kaguya;

std::string last_error_message;
void error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "unknown error";
}

void defargfn(int a = 3, int b = 2, int c = 1) {
  TEST_EQUAL(a, 3);
  TEST_EQUAL(b, 2);
  TEST_EQUAL(c, 1);
}

void defargfn2(int a, int b, int c = 1) {
  TEST_EQUAL(a, 3);
  TEST_EQUAL(b, 2);
  TEST_EQUAL(c, 1);
}

KAGUYA_FUNCTION_OVERLOADS(defargfn_wrapper, defargfn, 0, 3)
KAGUYA_FUNCTION_OVERLOADS(defargfn_wrapper2, defargfn2, 2, 3)

KAGUYA_TEST_FUNCTION_DEF(defaultarguments)(kaguya::State &state) {
  state.setErrorHandler(error_fun);
  state["defargfn"] = kaguya::function(defargfn_wrapper());
  state["defargfn2"] = kaguya::function(defargfn_wrapper2());

  std::string intname = kaguya::util::pretty_name(typeid(int));

  if (intname == "int") {
    state.dostring("defargfn('abc')");
    TEST_CHECK_M(last_error_message.find("[OPT]int,[OPT]int,[OPT]int") !=
                     std::string::npos,
                 last_error_message);
    state.dostring("defargfn2('abc',3)");
    TEST_CHECK_M(last_error_message.find("int,int,[OPT]int") !=
                     std::string::npos,
                 last_error_message);
  }
}

KAGUYA_TEST_GROUP_END(test_14_error_message)
