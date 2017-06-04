#include "kaguya/kaguya.hpp"
#include "../test_util.hpp"
#include "test_shared_class.hpp"

namespace {
int A_fn(const A &a) { return a.data; }
int B_fn(const B &) { return 22; }
int DerivedB_fn(const DerivedB &) { return 33; }
A create_A() { return A(1); }
}

#if defined(_WIN32) || defined(_WIN64)
extern "C" __declspec(dllexport)
#else
extern "C" __attribute__((visibility("default")))
#endif
    int luaopen_test_shared_class_use(lua_State *L) {
  using namespace kaguya;

  State state(L);
  LuaTable module = state.newTable();
  module["A_fn"] = &A_fn;
  module["create_A"] = &create_A;
  module["B_fn"] = &B_fn;
  module["DerivedB_fn"] = &DerivedB_fn;

  return module.push();
}
