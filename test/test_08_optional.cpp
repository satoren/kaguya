#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_08_optional)

using namespace kaguya_test_util;

KAGUYA_TEST_FUNCTION_DEF(optional_construct)(kaguya::State &) {
  kaguya::optional<int> opt1 = 1;
  TEST_EQUAL(1, *opt1);

  kaguya::optional<std::string> opt2 = std::string("abc");
  TEST_EQUAL("abc", *opt2);

  TEST_EQUAL(std::string("abc"), opt2.value_or("def"));

  const kaguya::optional<std::string> opt3 = opt2;
  TEST_EQUAL("abc", *opt2);
  TEST_EQUAL("abc", *opt3);
  std::string value("data");
  TEST_CHECK(opt2 = value);
}

KAGUYA_TEST_FUNCTION_DEF(optional_copy)(kaguya::State &) {
  kaguya::optional<const char *> s1 = "abc", s2; // constructor

  TEST_EQUAL(std::string("def"), s2.value_or("def"));
  TEST_CHECK((s2 = s1));
  TEST_CHECK((s1 = "def"));

  TEST_EQUAL(std::string("def"), s1.value_or("abc"));
  TEST_EQUAL(std::string("abc"), s2.value_or("def"));
  TEST_CHECK(s1);
  TEST_CHECK(s2);
  TEST_EQUAL(std::string("abc"), *s2);
  TEST_EQUAL(std::string("def"), *s1);

  TEST_CHECK(!(s2 = kaguya::optional<const char *>()));
  TEST_CHECK(!s2);

  TEST_CHECK(s2 = s1);
  TEST_EQUAL(std::string("def"), *s2);
  TEST_CHECK(s2 = "data");
  TEST_EQUAL(std::string("data"), *s2);

  kaguya::optional<const char *> csnullopt;
  TEST_CHECK(!(s2 = csnullopt));
  TEST_CHECK(!(s2 = csnullopt));
  TEST_CHECK(!s2);

  kaguya::nullopt_t nullopt;
  TEST_CHECK(!(s2 = nullopt));
  TEST_CHECK(!s2);
}

KAGUYA_TEST_FUNCTION_DEF(optional_ref)(kaguya::State &) {
  const std::string &str = "abc";
  kaguya::optional<const std::string &> s1 = str; // constructor
  TEST_EQUAL(std::string("abc"), *s1);

  TEST_EQUAL(std::string("abc"), s1.value_or("def"));

  const kaguya::optional<const std::string &> s2 = str; // constructor
  TEST_CHECK(s2);
  TEST_EQUAL(std::string("abc"), *s2);
  TEST_EQUAL(std::string("abc"), s1.value_or("def"));

  kaguya::optional<const std::string &> csnullopt;
  TEST_CHECK(!(s1 = csnullopt));
  TEST_CHECK(!s1);

  TEST_CHECK(s1 = str);
  TEST_CHECK(s1);

  kaguya::nullopt_t nullopt;
  TEST_CHECK(!(s1 = nullopt));
  TEST_CHECK(!s1);
}

KAGUYA_TEST_FUNCTION_DEF(optional_value_error)(kaguya::State &) {
  kaguya::optional<std::string> s1;

  TEST_CHECK(!s1);
  TEST_EQUAL(std::string("def"), s1.value_or("def"));
  try {
    s1.value();
  } catch (const kaguya::bad_optional_access &) {
    return;
  }
  TEST_CHECK(false);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error2)(kaguya::State &) {
  kaguya::optional<const std::string &> s1;

  TEST_CHECK(!s1);
  TEST_EQUAL(std::string("def"), s1.value_or("def"));
  try {
    s1.value();
  } catch (const kaguya::bad_optional_access &) {
    return;
  }
  TEST_CHECK(false);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error3)(kaguya::State &) {
  const kaguya::optional<std::string> s1;

  TEST_CHECK(!s1);
  TEST_EQUAL(std::string("def"), s1.value_or("def"));

  bool catch_except = false;
  try {
    s1.value();
  } catch (const kaguya::bad_optional_access &) {
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error4)(kaguya::State &) {
  const kaguya::optional<const std::string &> s1;

  TEST_CHECK(!s1);
  TEST_EQUAL(std::string("def"), s1.value_or("def"));
  bool catch_except = false;
  try {
    s1.value();
  } catch (const kaguya::bad_optional_access &) {
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

#if KAGUYA_USE_CPP11
struct MoveOnlyClass {
  MoveOnlyClass(int i) : member(i) {}
  int member;

  MoveOnlyClass(MoveOnlyClass &&src) : member(src.member) {}
  MoveOnlyClass &operator=(MoveOnlyClass &&src) {
    member = src.member;
    return *this;
  }

private:
  MoveOnlyClass();
  MoveOnlyClass(const MoveOnlyClass &);
  MoveOnlyClass &operator=(const MoveOnlyClass &);
};

KAGUYA_TEST_FUNCTION_DEF(optional_move)(kaguya::State &) {
  kaguya::optional<MoveOnlyClass> s1 = MoveOnlyClass(43);
  kaguya::optional<MoveOnlyClass> s2; // constructor
  s2 = std::move(s1);
  s1 = MoveOnlyClass(12);
  TEST_EQUAL(43, s2->member);
  TEST_EQUAL(12, s1->member);
}
#endif

template <typename T>
void optional_type_test(
    const typename kaguya::traits::decay<T>::type &init_value,
    const typename kaguya::traits::decay<T>::type &other_value) {
  typedef kaguya::optional<T> test_type;

  test_type init_v_opt(init_value);
  test_type other_v_opt(other_value);
  test_type nullopt_v_opt;

  TEST_EQUAL(init_v_opt == other_v_opt, init_value == other_value);
  TEST_EQUAL(init_v_opt != other_v_opt, init_value != other_value);
  TEST_EQUAL(init_v_opt < other_v_opt, init_value < other_value);
  TEST_EQUAL(init_v_opt > other_v_opt, init_value > other_value);
  TEST_EQUAL(init_v_opt <= other_v_opt, init_value <= other_value);
  TEST_EQUAL(init_v_opt >= other_v_opt, init_value >= other_value);

  TEST_EQUAL(init_v_opt == nullopt_v_opt, false);
  TEST_EQUAL(init_v_opt != nullopt_v_opt, true);
  TEST_EQUAL(init_v_opt < nullopt_v_opt, false);
  TEST_EQUAL(init_v_opt > nullopt_v_opt, true);
  TEST_EQUAL(init_v_opt <= nullopt_v_opt, false);
  TEST_EQUAL(init_v_opt >= nullopt_v_opt, true);

  TEST_EQUAL(nullopt_v_opt == nullopt_v_opt, true);
  TEST_EQUAL(nullopt_v_opt != nullopt_v_opt, false);
  TEST_EQUAL(nullopt_v_opt < nullopt_v_opt, false);
  TEST_EQUAL(nullopt_v_opt > nullopt_v_opt, false);
  TEST_EQUAL(nullopt_v_opt <= nullopt_v_opt, true);
  TEST_EQUAL(nullopt_v_opt >= nullopt_v_opt, true);

  test_type opt1 = init_value;
  TEST_EQUAL(init_value, opt1.value());
  TEST_EQUAL(init_value, *opt1);
  TEST_EQUAL(init_value, opt1.value_or(other_value));
  TEST_CHECK(!(opt1 = nullopt_v_opt));
  TEST_EQUAL(other_value, opt1.value_or(other_value));
  TEST_CHECK(!(opt1 = nullopt_v_opt));

  opt1 = init_value;
  TEST_EQUAL(init_value, opt1.value());
  TEST_EQUAL(init_value, *opt1);
  TEST_EQUAL(init_value, opt1.value_or(T(other_value)));
  TEST_CHECK(!(opt1 = nullopt_v_opt));
  TEST_EQUAL(other_value, opt1.value_or(T(other_value)));
  TEST_CHECK(!(opt1 = nullopt_v_opt));

  bool except_catch = false;
  try {
    opt1.value();
  } catch (const kaguya::bad_optional_access &) {
    except_catch = true;
  }
  TEST_CHECK(except_catch);

  TEST_CHECK(opt1 = test_type(init_value));
  TEST_EQUAL(init_value, *opt1);
  TEST_CHECK(opt1 = test_type(other_value));
  TEST_EQUAL(other_value, *opt1);

  TEST_CHECK(opt1 = test_type(T(init_value)));
  TEST_EQUAL(init_value, *opt1);
  TEST_CHECK(opt1 = T(init_value));
  TEST_EQUAL(init_value, *opt1);

  TEST_CHECK(opt1 = init_v_opt);
  TEST_EQUAL(init_value, *opt1);
  TEST_CHECK(opt1 = other_v_opt);
  TEST_EQUAL(other_value, *opt1);

  TEST_CHECK(opt1 = init_value);
  TEST_EQUAL(init_value, *opt1);
  TEST_CHECK(opt1 = other_value);
  TEST_EQUAL(other_value, *opt1);

  const test_type copt1 = init_value;
  TEST_CHECK(copt1);
  TEST_EQUAL(init_value, *copt1);

  TEST_EQUAL(init_value, copt1.value_or(other_value));

  const test_type copt2;
  TEST_CHECK(!copt2);
  except_catch = false;
  try {
    copt2.value();
  } catch (const kaguya::bad_optional_access &) {
    except_catch = true;
  }
  TEST_CHECK(except_catch);
}

KAGUYA_TEST_FUNCTION_DEF(optional_type)(kaguya::State &) {
  optional_type_test<int>(1, 4);
  optional_type_test<const int &>(1, 4);
  optional_type_test<double>(1, 4);
  optional_type_test<const double &>(1, 4);
  optional_type_test<const char *>("abc", "def");
  optional_type_test<std::string>("abc", "def");
  optional_type_test<const std::string &>("abc", "def");
}

KAGUYA_TEST_GROUP_END(test_08_optional)
