#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

#if KAGUYA_USE_CPP11
#include <array>

KAGUYA_TEST_GROUP_START(test_11_cxx11_feature)

using namespace kaguya_test_util;

enum class testenumclass {
  Foo = 0,
  Bar = 1,
};

KAGUYA_TEST_FUNCTION_DEF(enum_class_set)(kaguya::State &state) {
  state["value"] = testenumclass::Foo;
  TEST_CHECK(state("assert(value == 0)"));
}
KAGUYA_TEST_FUNCTION_DEF(enum_class_get)(kaguya::State &state) {
  state("value = 1");
  TEST_CHECK(state["value"] == testenumclass::Bar);
}

struct MoveOnlyClass {
  MoveOnlyClass(int i) : member(i) {}
  int member;

  MoveOnlyClass(MoveOnlyClass &&src) : member(src.member) {}

private:
  MoveOnlyClass();
  MoveOnlyClass(const MoveOnlyClass &);
  MoveOnlyClass &operator=(const MoveOnlyClass &);
};
KAGUYA_TEST_FUNCTION_DEF(movable_class_test)(kaguya::State &state) {
  state["MoveOnlyClass"].setClass(
      kaguya::UserdataMetatable<MoveOnlyClass>()
          .setConstructors<MoveOnlyClass(int)>()
          .addProperty("member", &MoveOnlyClass::member));

  state["moveonly"] = MoveOnlyClass(2);

  const MoveOnlyClass *ref = state["moveonly"];
  TEST_CHECK(ref);
  TEST_CHECK(ref->member == 2);

  state("func =function(arg) return assert(arg.member == 5) end");
  state["func"](MoveOnlyClass(5));

  state.newRef(MoveOnlyClass(5));
}

KAGUYA_TEST_FUNCTION_DEF(lambdafun)(kaguya::State &state) {
  state["ABC"] = kaguya::function([](int a) { return a * 2; });
  int a = state["ABC"](54);
  TEST_EQUAL(a, 108);

  state["free2"] = kaguya::function([]() { return 12; });
  TEST_EQUAL(state["free2"](), 12.0);

  state["sum"] = kaguya::function([](kaguya::VariadicArgType args) {
    int sum = 0;
    for (int arg : args) {
      sum += arg;
    }

    TEST_EQUAL(args.size(), 10);
    TEST_EQUAL(args[0], 1);
    TEST_EQUAL(args[1], 2);
    TEST_EQUAL(args[2], 3);
    TEST_EQUAL(args[3], 4);
    TEST_EQUAL(args[4], 5);
    TEST_EQUAL(args[5], 6);
    TEST_EQUAL(args[6], 7);
    TEST_EQUAL(args[7], 8);
    TEST_EQUAL(args[8], 9);
    TEST_EQUAL(args[9], 10);
    return sum;
  });
  TEST_EQUAL(state["sum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 55);

  state["ssum"] = kaguya::function([](kaguya::VariadicArgType args) {
    std::string sum;
    for (std::string arg : args) {
      sum += arg;
    }
    return sum;
  });
  TEST_EQUAL(state["ssum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), "12345678910");

  state["overload"] = kaguya::overload([](int) { return 1; },
                                       [](const std::string &) { return 2; },
                                       []() { return 3; });
  state("assert(overload(2) == 1)");   // int version
  state("assert(overload('2') == 2)"); // string version
  state("assert(overload() == 3)");    // no argument version

  state["lamda2"] =
      kaguya::function([]() mutable { return 12; }); // mutable functor
  state("assert(lamda2() == 12)");
}

KAGUYA_TEST_FUNCTION_DEF(put_unique_ptr)(kaguya::State &state) {
  state["MoveOnlyClass"].setClass(
      kaguya::UserdataMetatable<MoveOnlyClass>()
          .setConstructors<MoveOnlyClass(int)>()
          .addProperty("member", &MoveOnlyClass::member));

  state["uniqueptr"] = std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(2));

  const MoveOnlyClass *ref = state["uniqueptr"];
  TEST_CHECK(ref);
  TEST_EQUAL(ref->member, 2);

  state["uniqueptr"].typeTest<const std::unique_ptr<MoveOnlyClass> &>();
  const std::unique_ptr<MoveOnlyClass> &cuptr =
      state["uniqueptr"].get<const std::unique_ptr<MoveOnlyClass> &>();
  TEST_CHECK(cuptr);
  TEST_EQUAL(cuptr->member, 2);

  std::unique_ptr<MoveOnlyClass> &uptr =
      state["uniqueptr"].get<std::unique_ptr<MoveOnlyClass> &>();
  TEST_CHECK(uptr);
  TEST_EQUAL(uptr->member, 2);
  uptr.reset();

  state("func =function(arg) return assert(arg.member == 5) end");
  TEST_CHECK(state["func"](
                 std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(5))) == true);

  bool catch_except = false;
  try {
    std::unique_ptr<MoveOnlyClass> &uptr =
        state["nil"].get<std::unique_ptr<MoveOnlyClass> &>();
    TEST_CHECK(uptr);
    TEST_EQUAL(uptr->member, 2);
  } catch (const kaguya::LuaTypeMismatch &) {
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(compare_null_ptr)(kaguya::State &state) {
  {
    kaguya::LuaRef nullref = state.newRef(nullptr);
    TEST_CHECK(nullref.typeTest<std::nullptr_t>());
    TEST_CHECK(nullref.weakTypeTest<std::nullptr_t>());
    TEST_CHECK(nullref == nullptr);
    void *ptr = nullref.get<std::nullptr_t>();
    TEST_CHECK(!ptr);
  }
  {
    kaguya::LuaRef ref = state.newRef(1);
    TEST_CHECK(!(ref.typeTest<std::nullptr_t>()));
    TEST_CHECK(!(ref.weakTypeTest<std::nullptr_t>()));
    TEST_CHECK(ref != nullptr);
    bool catch_except = false;
    try {
      ref.get<std::nullptr_t>();
    } catch (const kaguya::LuaTypeMismatch &) {
      catch_except = true;
    }
    TEST_CHECK(catch_except);
  }
}

KAGUYA_TEST_FUNCTION_DEF(nullptr_set)(kaguya::State &state) {
  state["value"] = nullptr;
  TEST_CHECK(state["value"] == nullptr);
  TEST_CHECK(!state["value"]);
  TEST_CHECK(state("assert(value == nil)"));
}

KAGUYA_TEST_FUNCTION_DEF(null_unique_ptr)(kaguya::State &state) {
  state["Base"].setClass(kaguya::UserdataMetatable<MoveOnlyClass>());

  state["test"] = std::unique_ptr<MoveOnlyClass>();
  TEST_CHECK(state("assert(test==nil)"));
}

KAGUYA_TEST_FUNCTION_DEF(result_range_based_for)(kaguya::State &state) {
  state("fn =function() return 1,2,4,8,16 end");
  {
    std::vector<int> res;
    kaguya::LuaFunction fn = state["fn"];
    auto result = fn();
    for (int v : result) {
      res.push_back(v);
    }
    TEST_EQUAL(res.size(), 5);
    TEST_EQUAL(res[0], 1);
    TEST_EQUAL(res[1], 2);
    TEST_EQUAL(res[2], 4);
    TEST_EQUAL(res[3], 8);
    TEST_EQUAL(res[4], 16);
  }
  {
    std::vector<int> res;
    kaguya::LuaFunction fn = state["fn"];
    const auto result = fn();
    for (int v : result) {
      res.push_back(v);
    }
    TEST_EQUAL(res.size(), 5);
    TEST_EQUAL(res[0], 1);
    TEST_EQUAL(res[1], 2);
    TEST_EQUAL(res[2], 4);
    TEST_EQUAL(res[3], 8);
    TEST_EQUAL(res[4], 16);
  }
}

KAGUYA_TEST_FUNCTION_DEF(initializer_list)(kaguya::State &state) {
  state["table"] = kaguya::TableData{
      23, "test", {"key", "value"}, {"childtable", kaguya::TableData{3}}};
  kaguya::LuaTable tbl = state["table"];

  state("assert(table[1] == 23)");
  state("assert(table[2] == 'test')");
  state("assert(table['key'] == 'value')");

  TEST_EQUAL(tbl[1], 23);
  TEST_EQUAL(tbl[2], "test");
  TEST_EQUAL(tbl["key"], "value");
  TEST_EQUAL(tbl["childtable"][1], 3);
}

KAGUYA_TEST_FUNCTION_DEF(std_array)(kaguya::State &state) {
  std::array<int, 3> arr{{2, 3, 4}};
  state["value"] = arr;

  state("assert(value[1] == 2)");
  state("assert(value[2] == 3)");
  state("assert(value[3] == 4)");
  std::array<int, 3> get = state["value"];
  TEST_EQUAL(get[0], 2);
  TEST_EQUAL(get[1], 3);
  TEST_EQUAL(get[2], 4);
  bool v = state["value"].weakTypeTest<std::array<int, 3> >();
  TEST_CHECK(v);
}

int self_refcounted_object_count = 0;
struct self_refcounted_object {
  self_refcounted_object() : refcount(0) { self_refcounted_object_count++; }
  virtual ~self_refcounted_object() { self_refcounted_object_count--; }

  int get_ref_count() const { return refcount; }

  int refcount;

private:
  self_refcounted_object(const self_refcounted_object &) = delete;
  self_refcounted_object &operator=(const self_refcounted_object &) = delete;
};
struct self_refcounted_object_deleter {
  void operator()(self_refcounted_object *d) {
    d->refcount--;
    if (d->refcount <= 0) {
      delete d;
    }
  }
};

std::unique_ptr<self_refcounted_object, self_refcounted_object_deleter>
self_refcounted_object_construct() {
  self_refcounted_object *ptr = new self_refcounted_object();
  ptr->refcount++;
  return std::unique_ptr<self_refcounted_object,
                         self_refcounted_object_deleter>(ptr);
}

KAGUYA_TEST_FUNCTION_DEF(self_refcount_object)(kaguya::State &) {
  TEST_EQUAL(self_refcounted_object_count, 0);
  {
    kaguya::State state;
    state["self_refcount_object"].setClass(
        kaguya::UserdataMetatable<self_refcounted_object>()
            .addStaticFunction("new", &self_refcounted_object_construct)
            .addProperty("ref_count", &self_refcounted_object::get_ref_count));

    TEST_CHECK(state.dostring(
        "a = self_refcount_object.new();assert(a.ref_count == 1)"));

    TEST_EQUAL(self_refcounted_object_count, 1); // available 1 object
  }

  TEST_EQUAL(self_refcounted_object_count, 0); // destructed
}

KAGUYA_TEST_FUNCTION_DEF(invoke_test)(kaguya::State &) {
  TestClass c(2);
  TEST_EQUAL(c.getInt(), 2);
  kaguya::util::invoke(&TestClass::setInt, c, 4);
  TEST_EQUAL(c.getInt(), 4);
  int a = kaguya::util::invoke(&TestClass::getInt, c);

  TEST_EQUAL(a, 4);
}

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE

KAGUYA_TEST_FUNCTION_DEF(vector_to_table_with_move)(kaguya::State &state) {
  std::vector<double> v;
  v.push_back(3);
  v.push_back(13);
  v.push_back(2);
  v.push_back(99);
  state["v"] = std::move(v);
  TEST_CHECK(
      state("assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)"));
}
#endif

KAGUYA_TEST_FUNCTION_DEF(register_luafunction)(kaguya::State &state) {
  std::function<int(void)> func;
  state["reg_lua_func"] =
      kaguya::function([&](const std::function<int(void)> &f) { func = f; });

  state("reg_lua_func(function() return 123 end) "
        "");

  TEST_CHECK(func() == 123);
}

KAGUYA_TEST_FUNCTION_DEF(register_luafunction_from_coroutine)
(kaguya::State &state) {
  std::function<int(void)> func;
  state["reg_lua_func"] =
      kaguya::function([&](const std::function<int(void)> &f) { func = f; });

  state("local corf = coroutine.wrap( function() "
        "reg_lua_func(function() return 123 end) "
        "end) "
        "corf()");

  TEST_CHECK(func() == 123);
}

KAGUYA_TEST_GROUP_END(test_11_cxx11_feature)

#endif
