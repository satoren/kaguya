#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_12_push_any)
using namespace kaguya_test_util;

KAGUYA_TEST_FUNCTION_DEF(push_any_type)(kaguya::State &state) {
  using namespace kaguya;

  std::vector<AnyDataPusher> data;
  data.push_back(3);
  data.push_back("data");
  data.push_back("abc");
  data.push_back(std::string("abc"));

  TEST_COMPARE_EQ(state.newRef(AnyDataPusher(3)), 3);
  TEST_COMPARE_EQ(state.newRef(AnyDataPusher("data")), "data");
  TEST_COMPARE_EQ(state.newRef(AnyDataPusher(std::string("abc"))),
                  std::string("abc"));

  AnyDataPusher a("d");
  AnyDataPusher b(4);
  a = b;
  a = a;
  AnyDataPusher c;
  TEST_COMPARE_EQ(state.newRef(a), 4);
  TEST_COMPARE_EQ(state.newRef(b), 4);
  TEST_CHECK(!state.newRef(c));
}

KAGUYA_TEST_FUNCTION_DEF(push_const_shared_ptr_type)(kaguya::State &state) {
  using namespace kaguya;
  using namespace kaguya::standard;
  shared_ptr<const int> s(new int(4));
  LuaRef ref = state.newRef(s);
  shared_ptr<int> other = ref;

  TEST_CHECK(!other);
}
KAGUYA_TEST_GROUP_END(test_12_push_any)
