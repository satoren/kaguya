#include "kaguya/kaguya.hpp"
#include "test_shared_class.hpp"

#if defined(_WIN32) || defined(_WIN64)
extern "C" __declspec(dllexport)
#else
extern "C" __attribute__((visibility("default")))
#endif
    int luaopen_test_shared_class_reg(lua_State *L) {
  using namespace kaguya;

  State state(L);
  LuaTable module = state.newTable();

  module["A"].setClass(
      UserdataMetatable<A>().setConstructors<A(), A(int)>().addProperty(
          "data", &A::data));
  module["B"].setClass(UserdataMetatable<B>().setConstructors<B()>());
  module["DerivedA"].setClass(
      UserdataMetatable<DerivedA>().setConstructors<DerivedA()>());
  module["DerivedB"].setClass(
      UserdataMetatable<DerivedB, B>().setConstructors<DerivedB()>());

  return module.push();
}
