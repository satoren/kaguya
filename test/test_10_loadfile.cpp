#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_10_loadfile)
using namespace kaguya_test_util;

std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(load_return_number)(kaguya::State &state) {
  TEST_CHECK(state.dofile("lua/return_number.lua"));
  TEST_CHECK(state.loadfile("lua/return_number.lua"));
}

KAGUYA_TEST_FUNCTION_DEF(load_assign_value)(kaguya::State &state) {
  TEST_CHECK(state.dofile("lua/assign_value.lua"));
  TEST_EQUAL(state["value"], 1);
  state["value"] = 5;
  kaguya::LuaTable env = state.newTable();
  TEST_CHECK(state.dofile("lua/assign_value.lua", env));
  TEST_CHECK(state.loadfile("lua/assign_value.lua"));
  TEST_EQUAL(env["value"], 1);
}

KAGUYA_TEST_FUNCTION_DEF(load_syntax_error_script)(kaguya::State &state) {
  last_error_message = "";
  state.setErrorHandler(ignore_error_fun);

  TEST_CHECK(!state.loadfile("lua/syntax_error.lua"));
  TEST_CHECK(!state.dofile("lua/syntax_error.lua"));
  TEST_COMPARE_NE(last_error_message, "");
}

KAGUYA_TEST_FUNCTION_DEF(runtime_error_script)(kaguya::State &state) {
  last_error_message = "";
  state.setErrorHandler(ignore_error_fun);

  TEST_CHECK(!state.dofile("lua/runtime_error.lua"));
  TEST_COMPARE_NE(last_error_message, "");
}

KAGUYA_TEST_GROUP_END(test_10_loadfile)
