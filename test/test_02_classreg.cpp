#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

namespace {
struct SelfRefCounted {
  static int all_object_count; // for simple leak check

  SelfRefCounted() : refcount(0) { all_object_count++; }
  virtual ~SelfRefCounted() { all_object_count--; }

  void AddRef() { refcount++; }
  void ReleaseRef() {
    refcount--;
    if (refcount == 0) {
      delete this;
    }
  }

  int get_ref_count() const { return refcount; }

  int refcount;

private:
  SelfRefCounted(const SelfRefCounted &);
  SelfRefCounted &operator=(const SelfRefCounted &);
};
int SelfRefCounted::all_object_count = 0;

struct RefObjectA : SelfRefCounted {};
struct RefObjectB : SelfRefCounted {};
}

namespace kaguya {
template <class T> struct SelfRefCountedPtrWrapper : ObjectPointerWrapper<T> {
  SelfRefCountedPtrWrapper(T *ptr) : ObjectPointerWrapper<T>(ptr) {
    if (ptr) {
      ptr->AddRef();
    }
  }
  virtual ~SelfRefCountedPtrWrapper() {
    if (ObjectPointerWrapper<T>::object_) {
      ObjectPointerWrapper<T>::object_->ReleaseRef();
    }
  }
};

template <typename T>
struct isSelfCountedObjectPointer
    : kaguya::standard::conditional<
          kaguya::standard::is_convertible<T *, SelfRefCounted *>::value,
          kaguya::standard::true_type, kaguya::standard::false_type>::type {};

template <class T>
struct ObjectPointerWrapperType<
    T, typename traits::enable_if<isSelfCountedObjectPointer<T>::value>::type> {
  typedef SelfRefCountedPtrWrapper<typename traits::decay<T>::type> type;
};
}

KAGUYA_TEST_GROUP_START(test_02_classreg)

using namespace kaguya_test_util;

struct ABC {
  int intmember;
  std::string stringmember;

  ABC() : intmember(0) {}
  ABC(int a) : intmember(a) {}
  ABC(const char *a) : intmember(0), stringmember(a) {}
  ABC(std::string a) : intmember(0), stringmember(a) {}
  ABC(int intmem, const std::string &strmem)
      : intmember(intmem), stringmember(strmem) {}
  ABC(const std::string &strmem, int intmem)
      : intmember(intmem), stringmember(strmem) {}
  ABC(const ABC &src)
      : intmember(src.intmember), stringmember(src.stringmember) {}

  bool operator==(const ABC &rhs) const {
    return intmember == rhs.intmember && stringmember == rhs.stringmember;
  }
  bool operator<(const ABC &rhs) const {
    return intmember < rhs.intmember ||
           (intmember == rhs.intmember && stringmember < rhs.stringmember);
  }
  bool operator!=(const ABC &rhs) const { return !(*this == rhs); }
  bool operator>(const ABC &rhs) const { return (rhs < *this); }
  bool operator>=(const ABC &rhs) const { return !(*this < rhs); }
  bool operator<=(const ABC &rhs) const { return !(*this > rhs); }

  int getInt() const { return intmember; }
  void setInt(const int &n) { intmember = n; }
  std::string getString() const { return stringmember; }
  void setString(std::string str) { stringmember = str; }

  ABC copy() const { return *this; }
  const ABC &references() const { return *this; }
  const ABC &const_references() const { return *this; }
  ABC &references() { return *this; }
  ABC *pointer() { return this; }
  const ABC *const_pointer() const { return this; }
  kaguya::standard::shared_ptr<ABC> shared_copy() {
    return kaguya::standard::shared_ptr<ABC>(new ABC(*this));
  }
};

KAGUYA_TEST_FUNCTION_DEF(default_constructor)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC()>());

  TEST_CHECK(state("value = assert(ABC.new())"));
}
KAGUYA_TEST_FUNCTION_DEF(int_constructor)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "getInt", &ABC::getInt));

  TEST_CHECK(state("value = assert(ABC.new(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));
  TEST_EQUAL(state["value"]["getInt"](state["value"]), 32);
  TEST_EQUAL((state["value"]->*"getInt")(), 32);
}
KAGUYA_TEST_FUNCTION_DEF(string_constructor)(kaguya::State &state) {
  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .setConstructors<ABC(const char *)>()
                            .addFunction("getString", &ABC::getString));

  TEST_CHECK(state("value = assert(ABC.new('string_value'))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value')"));
}
KAGUYA_TEST_FUNCTION_DEF(overloaded_constructor)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>()
          .setConstructors<void(const char *), ABC(int), ABC(std::string),
                           ABC(int, const std::string &),
                           ABC(const std::string &, int)>()
          .addFunction("getString", &ABC::getString)
          .addFunction("getInt", &ABC::getInt)
          .addFunction("setInt", &ABC::setInt));

  TEST_CHECK(state("value = assert(ABC.new(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));
  TEST_CHECK(state("value = assert(ABC.new('string_value'))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value')"));
  TEST_CHECK(state("value = assert(ABC.new('string_value2',54))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value2')"));
  TEST_CHECK(state("assert(value:getInt() == 54)"));
  TEST_CHECK(state("value = assert(ABC.new(64,'string_value3'))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value:getInt() == 64)"));

  TEST_CHECK(state("value:setInt(33)"));
  TEST_CHECK(state("assert(value:getInt() == 33)"));
  TEST_EQUAL(state["value"]["getInt"](state["value"]), 33);
  TEST_EQUAL((state["value"]->*"getInt")(), 33);
}

ABC createABC(int, int, int) { return ABC(324343); }
KAGUYA_TEST_FUNCTION_DEF(overloaded_constructor2)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>()
          .addOverloadedFunctions(
              "new", kaguya::ConstructorFunction<ABC()>::type(),
              kaguya::ConstructorFunction<ABC(int)>::type(),
              kaguya::ConstructorFunction<ABC(const char *)>::type(),
              kaguya::ConstructorFunction<ABC(std::string)>::type(),
              kaguya::ConstructorFunction<ABC(int,
                                              const std::string &)>::type(),
              kaguya::ConstructorFunction<ABC(const std::string &,
                                              int)>::type(),
              &createABC)
          .addFunction("getString", &ABC::getString)
          .addFunction("getInt", &ABC::getInt)
          .addFunction("setInt", &ABC::setInt));

  TEST_CHECK(state("value = assert(ABC.new(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));
  TEST_CHECK(state("value = assert(ABC.new('string_value'))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value')"));
  TEST_CHECK(state("value = assert(ABC.new('string_value2',54))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value2')"));
  TEST_CHECK(state("assert(value:getInt() == 54)"));
  TEST_CHECK(state("value = assert(ABC.new(64,'string_value3'))"));
  TEST_CHECK(state("assert(value:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value:getInt() == 64)"));

  TEST_CHECK(state("value = assert(ABC.new(1,2,3))")); // use createABC
  TEST_CHECK(state("assert(value:getInt() == 324343)"));

  TEST_CHECK(state("value:setInt(33)"));
  TEST_CHECK(state("assert(value:getInt() == 33)"));
}

KAGUYA_TEST_FUNCTION_DEF(copy_constructor)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>()
          .setConstructors<ABC(const char *), ABC(int), ABC(std::string),
                           ABC(int, const std::string &),
                           ABC(const std::string &, int)>()
          .addFunction("getString", &ABC::getString)
          .addFunction("setString", &ABC::setString)
          .addFunction("getInt", &ABC::getInt)
          .addOverloadedFunctions(
              "references", static_cast<ABC &(ABC::*)()>(&ABC::references),
              static_cast<const ABC &(ABC::*)() const>(&ABC::references))
          .addFunction("const_pointer", &ABC::const_pointer)
          .addFunction("pointer", &ABC::pointer)
          .addFunction("copy", &ABC::copy)
          .addFunction("shared_copy", &ABC::shared_copy));

  TEST_CHECK(state("value = assert(ABC.new(64,'string_value3'))"));
  TEST_CHECK(state("value2 = assert(value:copy())"));
  TEST_CHECK(state("assert(value2:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value2:getInt() == 64)"));
  TEST_CHECK(state("value3 = assert(value:references())"));
  TEST_CHECK(state("cvalue = assert(value:const_pointer())"));
  TEST_CHECK(state("assert(cvalue:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(cvalue:getInt() == 64)"));

  TEST_CHECK(state("assert(value3:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value3:getInt() == 64)"));
  TEST_CHECK(state("value4 = assert(value:pointer())"));
  TEST_CHECK(state("assert(value4:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value4:getInt() == 64)"));

  TEST_CHECK(state("value5 = assert(value:shared_copy())"));
  TEST_CHECK(state("value =1"));
  state.garbageCollect(); // warning!! value3 and value4 to dangling pointer
  TEST_CHECK(state("assert(value5:getString() == 'string_value3')"));
  TEST_CHECK(state("assert(value5:getInt() == 64)"));

  {
    kaguya::standard::shared_ptr<ABC> shared_ptr(new ABC("shared_object", 53));
    state["shared_object"] = shared_ptr;
  }
  TEST_CHECK(state("assert(shared_object:getInt() == 53)"));
  TEST_CHECK(state("assert(shared_object:getString() =='shared_object')"));

  TEST_CHECK(state["value5"].weakTypeTest<kaguya::ObjectWrapperBase *>());
  TEST_CHECK(state["value5"].typeTest<kaguya::ObjectWrapperBase *>());

  kaguya::ObjectWrapperBase *wrapper = state["value2"];
  TEST_CHECK(wrapper->get());
  TEST_CHECK(wrapper->cget() == wrapper->get());
  TEST_CHECK(wrapper->native_get() == wrapper->get());
  TEST_CHECK(wrapper->native_get() == wrapper->cget());

  kaguya::ObjectWrapperBase *shared_wrapper = state["value5"];
  TEST_CHECK(shared_wrapper->get());
  TEST_CHECK(shared_wrapper->cget() == shared_wrapper->get());
  TEST_CHECK(shared_wrapper->native_get() != shared_wrapper->get());
  TEST_CHECK(shared_wrapper->native_get() != shared_wrapper->cget());
}

KAGUYA_TEST_FUNCTION_DEF(data_member_bind)(kaguya::State &state) {

  kaguya::LuaFunction f = state.loadstring("return 3,4");
  kaguya::LuaFunction f2 = state.loadstring("return 7,8,9");

  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .setConstructors<ABC(int), ABC(std::string)>()
                            .addFunction("intmember", &ABC::intmember)
                            .addFunction("stringmember", &ABC::stringmember)
                            .addStaticField("static_data", f())
                            .addStaticField("static_data2", f2()));

  TEST_CHECK(state("value = assert(ABC.new(64))"));
  TEST_CHECK(state("assert(value:intmember() == 64)"));

  TEST_CHECK(state("assert(value.static_data == 3)"));
  TEST_CHECK(state("assert(value.static_data2 == 7)"));

  TEST_CHECK(state("value:intmember(4)"));
  TEST_CHECK(state("assert(value:intmember() == 4)"));

  TEST_CHECK(state("value:stringmember('test')"));
  TEST_CHECK(state("assert(value:stringmember() == 'test')"));
}

KAGUYA_TEST_FUNCTION_DEF(codechunkregtest)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>()
          .addStaticField("luafunc", kaguya::LuaCodeChunkResult(
                                         "return function(a,b) return a*b end"))
          .addStaticField(
              "luafunc2",
              kaguya::LuaCodeChunk("local arg = {...}; return arg[1]*arg[2]")));
  ABC abc;
  state["abc"] = &abc;

  TEST_CHECK(state("assert(abc.luafunc(3,4) == 12)"));
  TEST_CHECK(state("assert(abc.luafunc2(3,4) == 12)"));
}

KAGUYA_TEST_FUNCTION_DEF(overload_member_function)(kaguya::State &state) {
  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .setConstructors<ABC(int)>()
                            .addOverloadedFunctions("setmember",
                                                    &ABC::intmember,
                                                    &ABC::stringmember)
                            .addFunction("intdata", &ABC::intmember)
                            .addFunction("stringdata", &ABC::stringmember));

  TEST_CHECK(state("value = assert(ABC.new(64))"));
  TEST_CHECK(state("assert(value:intdata() == 64)"));

  TEST_CHECK(state("value:setmember(4)"));
  TEST_CHECK(state("assert(value:intdata() == 4)"));

  TEST_CHECK(state("value:setmember('test')"));
  TEST_CHECK(state("assert(value:stringdata() == 'test')"));
}
struct Foo {
  int func() { return 1; }
  int func(int param) { return param; }
};
KAGUYA_TEST_FUNCTION_DEF(overload_member_function2)(kaguya::State &state) {
  state["Foo"].setClass(kaguya::UserdataMetatable<Foo>().addOverloadedFunctions(
      "func", static_cast<int (Foo::*)()>(&Foo::func),
      static_cast<int (Foo::*)(int)>(&Foo::func)));
  state["foo"] = Foo();
  state("assert(foo:func() == 1)");
  state("assert(foo:func(64) == 64)");
}
KAGUYA_TEST_FUNCTION_DEF(overload_member_function3)(kaguya::State &state) {
  state["Foo"].setClass(kaguya::UserdataMetatable<Foo>().addStaticField(
      "func", kaguya::overload(static_cast<int (Foo::*)()>(&Foo::func),
                               static_cast<int (Foo::*)(int)>(&Foo::func))));
  state["foo"] = Foo();
  state("assert(foo:func() == 1)");
  state("assert(foo:func(32) == 32)");
}

KAGUYA_TEST_FUNCTION_DEF(operator_bind)(kaguya::State &state) {
  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .setConstructors<ABC(int)>()
                            .addFunction("__eq", &ABC::operator==)
                            .addFunction("__lt", &ABC::operator<)
                            .addFunction("__le", &ABC::operator<=));

  TEST_CHECK(state("value1 = assert(ABC.new(64))"));
  TEST_CHECK(state("value2 = assert(ABC.new(64))"));
  TEST_CHECK(state("assert(value1 == value2)"));
  TEST_CHECK(state("assert(value1 >= value2)"));

  TEST_CHECK(state("value1 = assert(ABC.new(64))"));
  TEST_CHECK(state("value2 = assert(ABC.new(61))"));
  TEST_CHECK(state("assert(value1 ~= value2)"));
  TEST_CHECK(state("assert(value1 > value2)"));
  TEST_CHECK(state("assert(value1 >= value2)"));
}
KAGUYA_TEST_FUNCTION_DEF(add_field)(kaguya::State &state) {
  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .setConstructors<ABC(int)>()
                            .addStaticField("TEST", 1)
                            .addStaticField("TEST2", "343")
                            .addStaticField("TEST3", 133.23)
                            .addStaticField("TEST4", std::string("test")));

  TEST_CHECK(state("assert(ABC.TEST == 1)"));
  TEST_CHECK(state("assert(ABC.TEST2 == '343')"));
  TEST_CHECK(state("assert(ABC.TEST3 == 133.23)"));
  TEST_CHECK(state("assert(ABC.TEST4 == 'test')"));
}

struct CopyableClass {
  CopyableClass(int i) : member(i) {}
  bool operator==(const CopyableClass &rhs) const {
    return member == rhs.member;
  }
  bool operator!=(const CopyableClass &rhs) const {
    return member != rhs.member;
  }
  int member;

private:
  CopyableClass();
};
KAGUYA_TEST_FUNCTION_DEF(copyable_class_test)(kaguya::State &state) {
  state["CopyableClass"].setClass(
      kaguya::UserdataMetatable<CopyableClass>()
          .setConstructors<CopyableClass(int)>()
          .addFunction("__eq", &CopyableClass::operator==));
  state["copy"] = CopyableClass(2);
  state["copy2"] = CopyableClass(2);
  TEST_CHECK(state("assert(copy2 == copy)"));
  TEST_CHECK(state["copy"] == CopyableClass(2));
  TEST_CHECK(CopyableClass(2) == state["copy"]);
  CopyableClass copy = state["copy"];
  TEST_CHECK(copy == CopyableClass(2));

  CopyableClass src(5);
  state["copied"] = src;
  src.member = 12;
  CopyableClass copied = state["copied"];
  TEST_CHECK(copied != src);
  TEST_CHECK(state("assert(copied ~= copy)"));
}

struct NoncopyableClass {
  NoncopyableClass(int i) : member(i) {}
  bool operator==(const NoncopyableClass &rhs) const {
    return member == rhs.member;
  }
  bool operator!=(const NoncopyableClass &rhs) const {
    return member != rhs.member;
  }

private:
  NoncopyableClass();
  NoncopyableClass(const NoncopyableClass &);
  NoncopyableClass &operator=(const NoncopyableClass &);
  int member;
};
KAGUYA_TEST_FUNCTION_DEF(noncopyable_class_test)(kaguya::State &state) {
  state["NoncopyableClass"].setClass(
      kaguya::UserdataMetatable<NoncopyableClass>()
          .setConstructors<NoncopyableClass(int)>()
          .addFunction("__eq", &NoncopyableClass::operator==));

  NoncopyableClass noncopy(2);
  state["noncopy"] = &noncopy;

  const NoncopyableClass *noncpyref = state["noncopy"];
  TEST_CHECK(noncpyref == &noncopy);
}

KAGUYA_TEST_FUNCTION_DEF(registering_object_instance)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "get_value", &ABC::getInt));

  ABC abc(43);
  // register object pointer
  state["abc"] = &abc;
  state("assert(43 == abc:get_value())");
  // or copy instance
  state["copy_abc"] = abc;
  state("assert(43 == copy_abc:get_value())");
  // or registering shared pointer
  state["shared_abc"] = kaguya::standard::shared_ptr<ABC>(
      new ABC(43)); // kaguya::standard::shared_ptr is std::shared_ptr or
                    // boost::shared_ptr.
  state("assert(43 == shared_abc:get_value())");
}

struct Member {
  Member() : a(0){};
  Member(int i) : a(i){};
  int a;
};
struct Base {
  Base() : a(0){};
  int a;
  Member member;

  void set(int d) { a = d; }
  int get() const { return a; }

  void member_set(const Member &d) { member = d; }
  const Member &member_get() const { return member; }
};
struct Derived : Base {
  Derived() : b(0){};
  int b;
};
struct Derived2 : Derived {
  Derived2() : c(0){};
  int c;
};

struct PureVirtualBase {
  PureVirtualBase(){};
  virtual ~PureVirtualBase(){};

  virtual void test() = 0;
  virtual void default_arg(int a = 0) = 0;
  virtual void const_test() const = 0;
  virtual void const_default_arg(int a = 0) const = 0;
};

int base_function(Base *b) {
  TEST_CHECK(b);
  b->a = 1;
  return b->a;
}
int derived_function(Derived *d) {
  TEST_CHECK(d);
  d->b = 2;
  return d->b;
}
KAGUYA_TEST_FUNCTION_DEF(registering_derived_class)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().addFunction("a", &Base::a));

  state["Derived"].setClass(kaguya::UserdataMetatable<Derived, Base>()
                                .addFunction("b", &Derived::b)
                                .addStaticFunction("test", derived_function));
  state["Derived2"].setClass(kaguya::UserdataMetatable<Derived2, Derived>()
                                 .addFunction("c", &Derived2::c)
                                 .addStaticFunction("test", derived_function));

  Derived2 derived2;
  Derived derived;
  Base base;
  state["base"] = &base;
  state["derived"] = kaguya::standard::ref(derived);
  state["derived2"] = kaguya::standard::ref(derived2);
  state["base_function"] = &base_function;
  state["derived_function"] = &derived_function;
  TEST_CHECK(state("assert(1 == base_function(base))"));
  TEST_CHECK(state("assert(1 == base_function(derived))"));
  TEST_CHECK(state("assert(1 == base_function(derived2))"));
  TEST_CHECK(state("assert(2 == derived_function(derived))"));
  TEST_CHECK(state("assert(2 == derived_function(derived2))"));
  TEST_CHECK(state("assert(1 == base:a())"));
  TEST_CHECK(state("assert(1 == derived:a())"));
  TEST_CHECK(state("assert(2 == derived:b())"));
  TEST_CHECK(state("assert(1 == derived2:a())"));
  TEST_CHECK(state("assert(2 == derived2:b())"));
  TEST_EQUAL(derived.b, 2);
  TEST_EQUAL(derived2.b, 2);
  TEST_CHECK(state("assert(2 == derived.test(derived))"));
}

int receive_shared_ptr_function(kaguya::standard::shared_ptr<Derived> d) {
  TEST_CHECK(d);
  d->b = 5;
  return d->b;
}
int receive_base_shared_ptr_function(kaguya::standard::shared_ptr<Base> d) {
  TEST_CHECK(d);
  d->a = 2;
  return d->a;
}
int receive_base_shared_ptr_function_null(
    kaguya::standard::shared_ptr<Base> d) {
  TEST_CHECK(!d);
  return 4;
}
int receive_void_shared_ptr_function(kaguya::standard::shared_ptr<void> d) {
  TEST_CHECK(d);
  return 6;
}
KAGUYA_TEST_FUNCTION_DEF(registering_shared_ptr)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().addFunction("a", &Base::a));

  state["Derived"].setClass(
      kaguya::UserdataMetatable<Derived, Base>().addFunction("b", &Derived::b));

  kaguya::standard::shared_ptr<Derived> derived(new Derived());
  kaguya::standard::shared_ptr<Base> base(new Base());
  kaguya::standard::shared_ptr<const Derived> const_derived(new Derived());
  kaguya::standard::shared_ptr<const Base> const_base(new Base());
  state["base"] = base;
  state["derived"] = derived;
  state["const_base"] = const_base;
  state["const_derived"] = const_derived;
  state["non_shared_base"] = Base();
  state["base_function"] = &base_function;
  state["derived_function"] = &derived_function;
  state["receive_shared_ptr_function"] = &receive_shared_ptr_function;
  state["receive_base_shared_ptr_function"] = &receive_base_shared_ptr_function;

  state["receive_shared_ptr_function_null"] =
      &receive_base_shared_ptr_function_null;
  state["receive_void_shared_ptr_function"] = &receive_void_shared_ptr_function;

  TEST_CHECK(state("assert(1 == base_function(base))"));
  TEST_EQUAL(base->a, 1);
  TEST_CHECK(state("assert(1 == base_function(derived))"));
  TEST_EQUAL(derived->a, 1);
  TEST_CHECK(state("assert(2 == derived_function(derived))"));
  TEST_CHECK(state("assert(1 == base:a())"));
  TEST_CHECK(state("assert(1 == derived:a())"));
  TEST_CHECK(state("assert(2 == derived:b())"));
  TEST_EQUAL(derived->b, 2);
  TEST_CHECK(state("assert(5 == receive_shared_ptr_function(derived))"));
  TEST_EQUAL(derived->b, 5);
  TEST_CHECK(state("assert(2 == receive_base_shared_ptr_function(derived))"));
  TEST_EQUAL(derived->a, 2);
  TEST_CHECK(
      state("assert(4 == receive_shared_ptr_function_null(non_shared_base))"));
  TEST_CHECK(state("assert(6 == receive_void_shared_ptr_function(base))"));
  TEST_CHECK(state("assert(6 == receive_void_shared_ptr_function(derived))"));

  TEST_EQUAL(derived->a, 2);

  {
    kaguya::standard::shared_ptr<Derived> d;
    kaguya::standard::shared_ptr<Base> b;
    kaguya::standard::shared_ptr<void> v;
    TEST_CHECK(!(d = state["base"]));
    TEST_CHECK(b = state["base"]);
    TEST_CHECK(v = state["base"]);
    TEST_CHECK(d = state["derived"]);
    TEST_CHECK(b = state["derived"]);
    TEST_CHECK(v = state["derived"]);

    TEST_CHECK(!(d = state["non_shared_base"]));
    TEST_CHECK(!(b = state["non_shared_base"]));
    TEST_CHECK(!(v = state["non_shared_base"]));

    TEST_CHECK(!(d = state["const_base"]));
    TEST_CHECK(!(b = state["const_base"]));
    TEST_CHECK(!(v = state["const_base"]));
    TEST_CHECK(!(d = state["const_derived"]));
    TEST_CHECK(!(b = state["const_derived"]));
    TEST_CHECK(!(v = state["const_derived"]));
  }
  {
    kaguya::standard::shared_ptr<const Derived> cd;
    kaguya::standard::shared_ptr<const Base> cb;
    kaguya::standard::shared_ptr<const void> cv;
    TEST_CHECK(!(cd = state["base"]));
    TEST_CHECK(cb = state["base"]);
    TEST_CHECK(cv = state["base"]);
    TEST_CHECK(cd = state["derived"]);
    TEST_CHECK(cb = state["derived"]);
    TEST_CHECK(cv = state["derived"]);

    TEST_CHECK(!(cd = state["non_shared_base"]));
    TEST_CHECK(!(cb = state["non_shared_base"]));
    TEST_CHECK(!(cv = state["non_shared_base"]));
    TEST_CHECK(!(cv = state["non_shared_base"]));

    TEST_CHECK(!(cd = state["const_base"]));
    TEST_CHECK((cb = state["const_base"]));
    TEST_CHECK((cv = state["const_base"]));
    TEST_CHECK((cd = state["const_derived"]));
    TEST_CHECK((cb = state["const_derived"]));
    TEST_CHECK((cv = state["const_derived"]));
  }

#if !KAGUYA_NO_USERDATA_TYPE_CHECK
  {
    lua_newuserdata(state.state(), 100);
    kaguya::LuaRef myuserdata(state.state(), kaguya::StackTop());
    kaguya::standard::shared_ptr<const Derived> cd;
    kaguya::standard::shared_ptr<const Base> cb;
    kaguya::standard::shared_ptr<const void> cv;
    state["nokaguya_udata"] = myuserdata;

    TEST_CHECK(!(cd = state["nokaguya_udata"]));
    TEST_CHECK(!(cb = state["nokaguya_udata"]));
    TEST_CHECK(!(cv = state["nokaguya_udata"]));
    TEST_CHECK(!(cv = state["nokaguya_udata"]));
  }
#endif

  state["shared_ptr_function"] = kaguya::overload(
      &receive_shared_ptr_function, &receive_base_shared_ptr_function);
  TEST_CHECK(state("assert(5 == shared_ptr_function(derived))"));
  TEST_EQUAL(derived->b, 5);
  TEST_CHECK(state("assert(2 == shared_ptr_function(base))"));
  TEST_EQUAL(base->a, 2);

  state["shared_ptr_function"] = kaguya::overload(
      &receive_base_shared_ptr_function, &receive_shared_ptr_function);
  TEST_CHECK(state("assert(5 == shared_ptr_function(derived))"));
  TEST_EQUAL(derived->b, 5);
  TEST_CHECK(state("assert(2 == shared_ptr_function(base))"));
  TEST_EQUAL(base->a, 2);

  // first function is always miss.second function typecheck
  state["shared_ptr_function"] = kaguya::overload(
      &registering_derived_class, &receive_base_shared_ptr_function);
  TEST_CHECK(state("assert(2 == shared_ptr_function(derived))"));
  TEST_EQUAL(derived->a, 2);
}

struct shared_ptr_fun {
  kaguya::standard::shared_ptr<int> &ptr;
  shared_ptr_fun(kaguya::standard::shared_ptr<int> &r) : ptr(r) {}
  void operator()(kaguya::standard::shared_ptr<int> p) { ptr = p; }
};

KAGUYA_TEST_FUNCTION_DEF(shared_ptr_null)(kaguya::State &state) {
  kaguya::standard::shared_ptr<int> ptr(new int(5));
  state["foo"] = kaguya::function<void(kaguya::standard::shared_ptr<int>)>(
      shared_ptr_fun(ptr));
  state("foo(nil)");
  TEST_EQUAL(ptr, 0);
}

struct Base2 {
  Base2() : b(0){};
  int b;
  int test2() { return 1192; }
  int test() { return 794; }
  int consttest() const { return 794; }
};

struct Base3 {};
struct MultipleInheritance : Base, Base2 {
  MultipleInheritance() : d(0){};
  int d;
  int test() { return 1192; }
  int test3() { return 710; }
  int consttest2() const { return 1560; }
};
struct MultipleInheritance2 : MultipleInheritance, Base3 {
  MultipleInheritance2() : e(0){};
  int e;
};

KAGUYA_TEST_FUNCTION_DEF(multiple_inheritance)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().addProperty("a", &Base::a));
  state["Base2"].setClass(kaguya::UserdataMetatable<Base2>()
                              .addProperty("b", &Base2::b)
                              .addFunction("test", &Base2::test)
                              .addFunction("test2", &Base2::test2)
                              .addFunction("consttest", &Base2::consttest));
  state["Base3"].setClass(kaguya::UserdataMetatable<Base3>());
  state["MultipleInheritance"].setClass(
      kaguya::UserdataMetatable<MultipleInheritance,
                                kaguya::MultipleBase<Base, Base2> >()
          .addFunction("d", &MultipleInheritance::d)
          .addProperty("propd", &MultipleInheritance::d)
          .addFunction("test", &MultipleInheritance::test)
          .addFunction("consttest2", &MultipleInheritance::consttest2)
          .addFunction("test3", &MultipleInheritance::test3));

  state["MultipleInheritance2"].setClass(
      kaguya::UserdataMetatable<
          MultipleInheritance2,
          kaguya::MultipleBase<MultipleInheritance, Base3> >()
          .addFunction("e", &MultipleInheritance2::e));

  MultipleInheritance data;

  state["testobj"] = &data;
  TEST_CHECK(state("testobj:d(3)"));
  TEST_CHECK(state("assert(testobj:d() == 3)"));
  TEST_EQUAL(data.d, 3);
  TEST_CHECK(state("testobj.propd= 4"));
  TEST_CHECK(state("assert(testobj.propd == 4)"));
  TEST_EQUAL(data.d, 4);
  TEST_CHECK(state("assert(testobj:test()==1192)"));
  TEST_CHECK(state("assert(testobj:test2()==1192)"));
  TEST_CHECK(state("assert(testobj:test3()==710)"));
  TEST_CHECK(state("assert(testobj:consttest()==794)"));
  TEST_CHECK(state("testobj.a= 1"));
  TEST_CHECK(state("assert(testobj.a == 1)"));
  TEST_EQUAL(data.a, 1);
  TEST_CHECK(state("testobj.a= 2"));
  TEST_CHECK(state("assert(testobj.a == 2)"));
  TEST_EQUAL(data.a, 2);
  TEST_CHECK(state("testobj.b= 2"));
  TEST_CHECK(state("assert(testobj.b == 2)"));
  TEST_EQUAL(data.b, 2);
  TEST_CHECK(state("testobj.b= 5"));
  TEST_CHECK(state("assert(testobj.b == 5)"));
  TEST_EQUAL(data.b, 5);

  {
    MultipleInheritance test;
    state["test"] = &test;
    TEST_CHECK(state("test.b= 2"));
    TEST_CHECK(state("assert(test.b == 2)"));
    TEST_EQUAL(test.b, 2);
  }
  {
    MultipleInheritance2 test;
    state["test"] = &test;
    TEST_CHECK(state("test.b= 4"));
    TEST_CHECK(state("assert(test.b == 4)"));
    TEST_EQUAL(test.b, 4);
  }

  state["constobj"] = static_cast<const MultipleInheritance *>(&data);
  TEST_CHECK(state("assert(constobj:consttest()==794)"));
  TEST_CHECK(state("assert(constobj:consttest2()==1560)"));
}

KAGUYA_TEST_FUNCTION_DEF(add_property)(kaguya::State &state) {
  state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                             .setConstructors<Base()>()
                             .addProperty("a", &Base::a)
                             .addProperty("member", &Base::member)
                             .addFunction("memberf", &Base::member));

  state["Member"].setClass(
      kaguya::UserdataMetatable<Member>().addProperty("a", &Member::a));

  state["Derived"].setClass(kaguya::UserdataMetatable<Derived, Base>()
                                .setConstructors<Derived()>()
                                .addProperty("b", &Derived::b));

  state["Derived"]["test_value"] = 55;

  Derived derived;
  Base base;
  const Base *constbase = &base;
  state["base"] = &base;
  state["derived"] = kaguya::standard::ref(derived);

  TEST_CHECK(state("assert(55 == derived.test_value)"));

  TEST_CHECK(state("base.a=1"));
  TEST_CHECK(state("derived.a = 2"));
  TEST_CHECK(state("derived.b=3"));
  TEST_CHECK(state("derived.member.a=5"));
  TEST_CHECK(state("assert(1 == base.a)"));
  TEST_CHECK(state("assert(2 == derived.a)"));
  TEST_CHECK(state("assert(3 == derived.b)"));
  TEST_CHECK(state("assert(5 == derived.member.a)"));
  TEST_EQUAL(base.a, 1);
  TEST_EQUAL(derived.a, 2);
  TEST_EQUAL(derived.b, 3);
  TEST_EQUAL(derived.member.a, 5);

  TEST_CHECK(state("base2 = Base.new()"));
  TEST_CHECK(state("base2.a=5"));
  TEST_CHECK(state("assert(5 == base2.a)"));

  base.a = 3;
  base.member.a = 6;
  state["constbase"] = constbase;
  TEST_CHECK(state("assert(3 == constbase.a)"));
  TEST_CHECK(state("assert(6 == constbase.member.a)"));
  TEST_CHECK(state("assert(6 == constbase.memberf(constbase).a)"));
}
KAGUYA_TEST_FUNCTION_DEF(add_property_case2)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));

  state["Derived"].setClass(
      kaguya::UserdataMetatable<Derived, Base>().setConstructors<Derived()>());

  Derived derived;
  Base base;
  state["base"] = &base;
  state["derived"] = kaguya::standard::ref(derived);
  TEST_CHECK(state("base.a=1"));
  TEST_CHECK(state("derived.a = 2"));
  TEST_CHECK(state("assert(1 == base.a)"));
  TEST_CHECK(state("assert(2 == derived.a)"));
  TEST_EQUAL(base.a, 1);
  TEST_EQUAL(derived.a, 2);
}

KAGUYA_TEST_FUNCTION_DEF(add_property_with_setter_getter)
(kaguya::State &state) {
  state["Member"].setClass(kaguya::UserdataMetatable<Member>()
                               .setConstructors<Member(), Member(int)>()
                               .addProperty("a", &Member::a));
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>()
          .setConstructors<Base()>()
          .addProperty("prop", &Base::get, &Base::set)
          .addProperty("class_prop", &Base::member_get, &Base::member_set));
  Base base;
  state["obj"] = &base;
  TEST_CHECK(state("obj.prop=1"));
  TEST_CHECK(state("assert(obj.prop==1)"));
  TEST_EQUAL(base.a, 1);
  TEST_CHECK(state("obj.prop=22"));
  TEST_CHECK(state("assert(obj.prop==22)"));
  TEST_EQUAL(base.a, 22);

  TEST_CHECK(state("obj.class_prop=Member.new(5)")); // set
  TEST_CHECK(state("assert(obj.class_prop.a==5)"));  // get
  TEST_EQUAL(base.member.a, 5);

  TEST_CHECK(state("obj.class_prop=Member.new(33)")); // set
  TEST_CHECK(state("assert(obj.class_prop.a==33)"));  // get
  TEST_EQUAL(base.member.a, 33);
}

struct Prop {
  Prop() : a(0) {}
  int a;
};
struct PropHolder {
  Prop mem;
  ~PropHolder() {}
};
KAGUYA_TEST_FUNCTION_DEF(add_property_ref_check)(kaguya::State &state) {
  state["Prop"].setClass(
      kaguya::UserdataMetatable<Prop>().addProperty("a", &Prop::a));

  state["PropHolder"].setClass(
      kaguya::UserdataMetatable<PropHolder>().addProperty("mem",
                                                          &PropHolder::mem));

  PropHolder prop;
  state["prop"] = &prop;
  TEST_CHECK(state("prop.mem.a=455"));
  TEST_EQUAL(prop.mem.a, 455);

  { // member retain not supported now
    //			state["prop"] = PropHolder();
    //			TEST_CHECK(state("member = prop.mem"));
    //			TEST_CHECK(state("prop = nil"));
    //			state.garbageCollect();
    //			TEST_CHECK(state("member.a = 3232"));
  }
}

KAGUYA_TEST_FUNCTION_DEF(object_take_copy)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));

  TEST_CHECK(state("fn = function(object,value) object.a = value end"));
  {
    Base base;
    base.a = 232;
    state["obj"] = base;
    TEST_CHECK(state("assert(obj.a==232)"));
    TEST_CHECK(state("obj.a = 2"));
    TEST_CHECK(state("assert(obj.a==2)"));
    TEST_EQUAL(base.a, 232);

    state.garbageCollect();
    TEST_CHECK(state("assert(obj.a==2)"));

    state["fn"](base, 54);
    TEST_EQUAL(base.a, 232);
  }
  { //
    Base basesrc;
    Base &base = basesrc;
    base.a = 232;
    state["obj"] = base;
    TEST_CHECK(state("assert(obj.a==232)"));
    TEST_CHECK(state("obj.a = 2"));
    TEST_CHECK(state("assert(obj.a==2)"));
    TEST_EQUAL(base.a, 232);

    state.garbageCollect();
    TEST_CHECK(state("assert(obj.a==2)"));

    state["fn"](base, 54);
    TEST_EQUAL(base.a, 232);
  }
  { //
    Base basesrc;
    const Base &base = basesrc;
    basesrc.a = 232;
    state["obj"] = base;
    TEST_CHECK(state("assert(obj.a==232)"));
    TEST_CHECK(state("obj.a = 2"));
    TEST_CHECK(state("assert(obj.a==2)"));
    TEST_EQUAL(base.a, 232);

    state.garbageCollect();
    TEST_CHECK(state("assert(obj.a==2)"));

    state["fn"](base, 54);
    TEST_EQUAL(base.a, 232);
  }
}
KAGUYA_TEST_FUNCTION_DEF(object_take_pointer)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));
  TEST_CHECK(state("fn = function(object,value) object.a = value end"));

  Base base;
  base.a = 232;
  state["obj"] = &base;
  TEST_CHECK(state("assert(obj.a==232)"));
  TEST_CHECK(state("obj.a = 2"));
  TEST_CHECK(state("assert(obj.a==2)"));
  TEST_EQUAL(base.a, 2);

  state.garbageCollect();
  TEST_CHECK(state("assert(obj.a==2)"));

  state["fn"](&base, 54);
  TEST_EQUAL(base.a, 54);
}
KAGUYA_TEST_FUNCTION_DEF(object_take_reference)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));
  TEST_CHECK(state("fn = function(object,value) object.a = value end"));

  Base base;
  base.a = 232;
  state["obj"] = kaguya::standard::ref(base);
  TEST_CHECK(state("assert(obj.a==232)"));
  TEST_CHECK(state("obj.a = 2"));
  TEST_CHECK(state("assert(obj.a==2)"));
  TEST_EQUAL(base.a, 2);

  state.garbageCollect();
  TEST_CHECK(state("assert(obj.a==2)"));

  state["fn"](kaguya::standard::ref(base), 54);
  TEST_EQUAL(base.a, 54);
}
KAGUYA_TEST_FUNCTION_DEF(object_take_const_reference)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));
  TEST_CHECK(state("fn = function(object,value) object.a = value end"));

  Base basesrc;
  basesrc.a = 232;
  const Base *base = &basesrc;
  state["obj"] = base;
  TEST_CHECK(state("assert(obj.a==232)"));
  //		TEST_CHECK(!state("obj.a = 2"));//cannot assign
  TEST_CHECK(state("assert(obj.a==232)"));
  TEST_EQUAL(base->a, 232);

  basesrc.a = 22;
  TEST_CHECK(state("assert(obj.a==22)"));

  state.garbageCollect();
  TEST_CHECK(state("assert(obj.a==22)"));

  //		state["fn"](base, 54);//cannot assign
  TEST_EQUAL(base->a, 22);
}
KAGUYA_TEST_FUNCTION_DEF(object_take_const_pointer)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));
  TEST_CHECK(state("fn = function(object,value) object.a = value end"));

  Base basesrc;
  basesrc.a = 232;
  const Base &base = basesrc;
  state["obj"] = kaguya::standard::ref(base);
  TEST_CHECK(state("assert(obj.a==232)"));
  //		TEST_CHECK(!state("obj.a = 2"));//cannot assign
  TEST_CHECK(state("assert(obj.a==232)"));
  TEST_EQUAL(base.a, 232);

  basesrc.a = 22;
  TEST_CHECK(state("assert(obj.a==22)"));

  state.garbageCollect();
  TEST_CHECK(state("assert(obj.a==22)"));

  //		state["fn"](base, 54);//cannot assign
  TEST_EQUAL(base.a, 22);
}

KAGUYA_TEST_FUNCTION_DEF(null_shared_ptr)(kaguya::State &state) {
  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base::a));

  state["test"] = kaguya::standard::shared_ptr<Base>();
  TEST_CHECK(state("assert(test==nil)"));

  state["test"] = kaguya::standard::shared_ptr<Derived>();

  TEST_CHECK(state("assert(test==nil)"));

  state["test"] = kaguya::standard::shared_ptr<void>();
  TEST_CHECK(state("assert(test==nil)"));

  kaguya::standard::shared_ptr<void> sptr = state["test"];
  TEST_CHECK(!sptr);
}

int TestClass2_objectcount = 0;
struct TestClass2 {
  TestClass2() { TestClass2_objectcount++; }
  TestClass2(const TestClass2 &src) : data(src.data) {
    TestClass2_objectcount++;
  }
  ~TestClass2() { TestClass2_objectcount--; }
  std::vector<int> data;
};

KAGUYA_TEST_FUNCTION_DEF(not_registered_object)(kaguya::State &) {
  TEST_CHECK(TestClass2_objectcount == 0);
  {
    kaguya::State state;
    state["data"] = TestClass2();
  }
  TEST_CHECK(TestClass2_objectcount == 0);
}

std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(error_check)(kaguya::State &state) {
  state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                             .setConstructors<Base()>()
                             .addProperty("a", &Base::a)
                             .addFunction("fa", &Base::a)
                             .addProperty("member", &Base::member)
                             .addFunction("memberf", &Base::member));

  state["test"] = Base();
  TEST_CHECK(state("assert(test~=nil)"));

  TEST_CHECK(state("test.fa(test)"));
  TEST_CHECK(state("test:fa()"));

  state.setErrorHandler(ignore_error_fun);
  last_error_message = "";

  state("test.fa()"); // error
  TEST_COMPARE_NE(last_error_message, "");

  last_error_message = "";
  state("assert(6 == test.memberf(nil).a)");
  TEST_COMPARE_NE(last_error_message, "");
}

KAGUYA_TEST_FUNCTION_DEF(duplicate_register_member_error_throw_test)
(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
  bool catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .setConstructors<Base()>()
                               .addFunction("a", &Base::a)
                               .addProperty("a", &Base::a));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
  catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addProperty("a", &Base::a)
                               .addStaticFunction("a", &Base::a));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
  catch_except = false;
  try {
    std::string data_value("value");
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addProperty("a", &Base::a)
                               .addStaticField("a", data_value));
    TEST_EQUAL(data_value, "value");
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
  catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addProperty("a", &Base::a)
                               .addOverloadedFunctions("a", &Base::a));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addOverloadedFunctions("a", &Base::a)
                               .addProperty("a", &Base::get));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addOverloadedFunctions("a", &Base::a)
                               .addProperty("a", &Base::get, &Base::set));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .addFunction("a", &Base::a)
                               .addFunction("a", &Base::get));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  catch_except = false;
  try {
    CopyableClass copy = state["a"];
    TEST_CHECK(false); // unreachable
    TEST_CHECK(copy.member);
  } catch (const kaguya::LuaTypeMismatch &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("mismatch") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(duplicate_register_member_error_throw_test2)
(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

  bool catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .setConstructors<Base()>()
                               .addProperty("a", &Base::a)
                               .addProperty("a", &Base::a));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(duplicate_register_member_error_throw_test3)
(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

  bool catch_except = false;
  try {
    state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                               .setConstructors<Base()>()
                               .addProperty("a", &Base::a)
                               .addStaticField("a", 3));
  } catch (const kaguya::KaguyaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("already registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(duplicate_register_class_error_throw_test)
(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

  bool catch_except = false;
  try {
    state["Base"].setClass(
        kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
            "a", &Base::a));
    state["Base3"].setClass(
        kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
            "a", &Base::a));
  } catch (const kaguya::LuaException &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("registered") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

KAGUYA_TEST_FUNCTION_DEF(this_typemismatch_error_test)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .addOverloadedFunctions("setmember",
                                                    &ABC::intmember,
                                                    &ABC::stringmember)
                            .addFunction("intdata", &ABC::intmember)
                            .addFunction("stringdata", &ABC::stringmember)
                            .addFunction("getInt", &ABC::getInt)
                            .addFunction("setInt", &ABC::setInt)
                            .addFunction("shared_copy", &ABC::shared_copy)
                            .addFunction("const_pointer", &ABC::const_pointer));

  state["test"] = ABC();
  state["other"] = Base();

  TEST_CHECK(state("assert(test~=nil)"));
  TEST_CHECK(state("test.setmember(test,'')"));

  last_error_message = "";
  state("test.setmember()");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);

  last_error_message = "";
  state("test.intdata()");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);

  last_error_message = "";
  state("test.getInt()");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);

  last_error_message = "";
  state("test.setInt()");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);

  last_error_message = "";
  state("test.shared_copy()");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);

  last_error_message = "";
  state("test.const_pointer(other)");
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);
}

KAGUYA_TEST_FUNCTION_DEF(self_refcounted_object)(kaguya::State &) {
  // test for self reference counted class
  // see SelfRefCountedPtrWrapper and specialized ObjectPointerWrapperType for
  // SelfRefCounted
  TEST_EQUAL(SelfRefCounted::all_object_count, 0);

  {
    kaguya::State state;
    state["RefObjectA"].setClass(kaguya::UserdataMetatable<RefObjectA>());
    state["RefObjectA"].setClass(kaguya::UserdataMetatable<RefObjectB>());

    state["a"] = new RefObjectA;
    state["b"] = new RefObjectB;
    TEST_EQUAL(SelfRefCounted::all_object_count, 2);

    RefObjectA *ptr = state["a"];
    state["c"] = ptr;
    TEST_EQUAL(SelfRefCounted::all_object_count, 2);
  }
  TEST_EQUAL(SelfRefCounted::all_object_count, 0);
}

int testindexfn(ABC *self, int key) { return self->intmember * key; }
void testnewindexfn(ABC *self, int key, int value) {
  TEST_CHECK(self);
  TEST_EQUAL(key, 1);
  TEST_EQUAL(value, 3);
}

KAGUYA_TEST_FUNCTION_DEF(self_register_index_object)(kaguya::State &state) {

  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
                            .addStaticFunction("__index", &testindexfn)
                            .addStaticFunction("__newindex", &testnewindexfn));
  ABC obj;
  state["obj"] = ABC(3);
  state("assert(obj[3]==9);");
  state("obj[1]=3;");

  { // other way
    kaguya::State state;
    state["ABC"].setClass(kaguya::UserdataMetatable<ABC>());

    state["ABC"]["__index"] = kaguya::function(&testindexfn);
    state["ABC"]["__newindex"] = kaguya::function(&testindexfn);

    ABC obj;
    state["obj"] = ABC(3);
    state("assert(obj[3]==9);");
    state("obj[1]=3;");
  }
}

KAGUYA_TEST_FUNCTION_DEF(call_constructor)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "getInt", &ABC::getInt));

  TEST_CHECK(state("value = assert(ABC(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));

  state["Foo"].setClass(
      kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());
  TEST_CHECK(state("value = assert(Foo())"));

  state["Base"].setClass(kaguya::UserdataMetatable<Base>()
                             .addProperty("a", &Base::a)
                             .addFunction("__call", &Base::get)
                             .setConstructors<Base()>());
  state["Base2"].setClass(kaguya::UserdataMetatable<Base2>()
                              .addProperty("b", &Base2::b)
                              .addFunction("__call", &Base2::test)
                              .setConstructors<Base2()>());
  state["MultipleInheritance"].setClass(
      kaguya::UserdataMetatable<MultipleInheritance,
                                kaguya::MultipleBase<Base, Base2> >()
          .setConstructors<MultipleInheritance()>()
          .addFunction("__call", &MultipleInheritance::test)
          .addFunction("d", &MultipleInheritance::d));

  TEST_CHECK(state("multiinheritance = assert(MultipleInheritance())"));
  TEST_CHECK(state("assert(multiinheritance:d() == 0)"));
  TEST_CHECK(state("assert(multiinheritance.a == 0)"));
  TEST_CHECK(state("assert(multiinheritance.b == 0)"));
  TEST_CHECK(state("assert(multiinheritance() == 1192)"));

  TEST_CHECK(state("base2 = assert(Base2())"));
  TEST_CHECK(state("assert(base2.b == 0)"));
  TEST_CHECK(state("assert(base2() == 794)"));
  TEST_CHECK(state("base = assert(Base())"));
  TEST_CHECK(state("assert(base.a == 0)"));
  TEST_CHECK(state("base.a = 4 assert(base.a == 4) assert(base() == 4)"));
}
KAGUYA_TEST_FUNCTION_DEF(int_call_constructor_manual)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "getInt", &ABC::getInt));
  kaguya::LuaTable constructor_table = state.newTable();
  constructor_table["__call"] = kaguya::LuaCodeChunkResult(
      "return function(t,...) return t.new(...) end");
  state["ABC"].setMetatable(constructor_table);

  TEST_CHECK(state("value = assert(ABC(32))"));
  TEST_CHECK(state("assert(value:getInt() == 32)"));
}

KAGUYA_TEST_FUNCTION_DEF(constructor_arg_type_mismatch_error)
(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "getInt", &ABC::getInt));
  state.setErrorHandler(ignore_error_fun);

  last_error_message = "";
  bool res = state("v = ABC.new('abc')");
  TEST_CHECK(!res);
  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);
}

KAGUYA_TEST_FUNCTION_DEF(arg_type_mismatch_error)(kaguya::State &state) {
  state["ABC"].setClass(
      kaguya::UserdataMetatable<ABC>().setConstructors<ABC(int)>().addFunction(
          "setInt", &ABC::setInt));
  state.setErrorHandler(ignore_error_fun);

  last_error_message = "";
  TEST_CHECK(state("value = assert(ABC.new(32))"));

  TEST_CHECK(!state("value:setInt('abc')"));

  TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);
}
int Base_a_getter(const Base *self) { return self->a; }
void Base_a_setter(Base *self, int v) { self->a = v; }
KAGUYA_TEST_FUNCTION_DEF(add_property_external)(kaguya::State &state) {

  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>().setConstructors<Base()>().addProperty(
          "a", &Base_a_getter, &Base_a_setter));
  Base base;
  state["base"] = &base;
  TEST_CHECK(state("base.a=1"));
  TEST_CHECK(state("assert(1 == base.a)"));
  TEST_EQUAL(base.a, 1);
}
KAGUYA_TEST_FUNCTION_DEF(add_property_any)(kaguya::State &state) {

  state["Base"].setClass(
      kaguya::UserdataMetatable<Base>()
          .setConstructors<Base()>()
          .addPropertyAny("a", &Base_a_getter, &Base_a_setter));
  Base base;
  state["base"] = &base;
  TEST_CHECK(state("base.a=1"));
  TEST_CHECK(state("assert(1 == base.a)"));
  TEST_EQUAL(base.a, 1);
}

KAGUYA_MEMBER_FUNCTION_OVERLOADS(PureVirtualBase_defarg_test, PureVirtualBase,
                                 default_arg, 0, 1)
KAGUYA_MEMBER_FUNCTION_OVERLOADS_WITH_SIGNATURE(
    PureVirtualBase_const_defarg_test, PureVirtualBase, const_default_arg, 0, 1,
    void (PureVirtualBase::*)(int) const)

struct PureVirtualDerived : PureVirtualBase {
  PureVirtualDerived(int &t) : v(t){};
  virtual ~PureVirtualDerived(){};

  virtual void test() { v = 1; };
  virtual void default_arg(int a = 0) { v = a; }
  virtual void const_test() const { v = 2; };
  virtual void const_default_arg(int a = 0) const { v = a; }

  int &v;
};
KAGUYA_TEST_FUNCTION_DEF(pure_virtual_test)(kaguya::State &state) {

  state["PureVirtualBase"].setClass(
      kaguya::UserdataMetatable<PureVirtualBase>()
          .addFunction("test", &PureVirtualBase::test)
          .addFunction("const_test", &PureVirtualBase::const_test)
          .addFunction("default_arg", PureVirtualBase_defarg_test())
          .addFunction("const_default_arg",
                       PureVirtualBase_const_defarg_test()));

  state["PureVirtualDerived"].setClass(
      kaguya::UserdataMetatable<PureVirtualDerived, PureVirtualBase>()
          .addFunction("test", &PureVirtualDerived::test));
  int test = 0;
  PureVirtualDerived virtual_test(test);
  state["test"] = &virtual_test;
  TEST_CHECK(state("test:test()"));
  TEST_EQUAL(test, 1);
  TEST_CHECK(state("test:const_test()"));
  TEST_EQUAL(test, 2);
  TEST_CHECK(state("test:default_arg()"));
  TEST_EQUAL(test, 0);
  TEST_CHECK(state("test:default_arg(6)"));
  TEST_EQUAL(test, 6);
  TEST_CHECK(state("test:const_default_arg()"));
  TEST_EQUAL(test, 0);
  TEST_CHECK(state("test:const_default_arg(6)"));
  TEST_EQUAL(test, 6);

  test = 0;
  PureVirtualBase *pure_virtual_test = &virtual_test;
  state["test"] = pure_virtual_test;
  TEST_CHECK(state("test:test()"));
  TEST_EQUAL(test, 1);
  TEST_CHECK(state("test:const_test()"));
  TEST_EQUAL(test, 2);
  TEST_CHECK(state("test:default_arg()"));
  TEST_EQUAL(test, 0);
  TEST_CHECK(state("test:default_arg(6)"));
  TEST_EQUAL(test, 6);
  TEST_CHECK(state("test:const_default_arg()"));
  TEST_EQUAL(test, 0);
  TEST_CHECK(state("test:const_default_arg(6)"));
  TEST_EQUAL(test, 6);
}


KAGUYA_TEST_FUNCTION_DEF(unregistered_property)(kaguya::State &state) {

	state["Base"].setClass(
		kaguya::UserdataMetatable<Base>()
		.setConstructors<Base()>()
		.addProperty("a", &Base::a));

	state.setErrorHandler(ignore_error_fun);
	last_error_message = "";

	Base base;
	state["base"] = &base;
	TEST_CHECK(state("assert(base.c==nil)"));
	TEST_CHECK(!state("base.c=1"));
}


KAGUYA_TEST_GROUP_END(test_02_classreg)
