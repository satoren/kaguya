#pragma once
#include <map>
#include <set>
#include <iostream>
#include <cassert>
#include <sstream>
#include "kaguya/kaguya.hpp"

namespace kaguya_test_util {
inline std::string to_string(int v) {
  char buffer[64] = {};
#if (defined(_MSC_VER) && _MSC_VER < 1900)
  sprintf_s(buffer, "%d", v);
#else
  snprintf(buffer, 64, "%d", v);
#endif
  return buffer;
}
inline std::string to_string(size_t v) { return to_string(int(v)); }

inline std::vector<std::string> split(std::string s, const char delim) {
  std::vector<std::string> result;
  std::size_t pos;
  while ((pos = s.find(delim)) != std::string::npos) {
    result.push_back(s.substr(0, pos));
    s = s.substr(pos + 1);
  }
  result.push_back(s);
  return result;
}
inline std::vector<std::string>
remove_empty(const std::vector<std::string> &src) {
  std::vector<std::string> result;
  result.reserve(src.size());
  for (std::vector<std::string>::const_iterator i = src.begin(); i != src.end();
       ++i) {
    if (!i->empty()) {
      result.push_back(*i);
    }
  }
  return result;
}

typedef void (*TestFunctionType)(kaguya::State &);

class TestRunner {
  TestRunner() {}
  TestRunner(const TestRunner &);
  TestRunner &operator=(const TestRunner &);

  typedef std::map<std::string, TestFunctionType> TestFunctionMapType;

  TestFunctionMapType test_functions_;

  std::vector<std::string> test_set_;

public:
  void set_test_set(std::vector<std::string> container) {
    test_set_ = container;
  }
  static TestRunner &instance() {
    static TestRunner ins;
    return ins;
  }
  static void test_error_handler(int, const char *message) {
    throw std::runtime_error(std::string(message));
  }
  bool is_name_execute(const std::string &name) const {
    if (test_set_.empty()) {
      return true;
    }
    for (std::vector<std::string>::const_iterator it = test_set_.begin();
         it != test_set_.end(); ++it) {
      if (name.find(*it) != std::string::npos) {
        return true;
      }
    }

    return false;
  }
  TestFunctionMapType tests() {
    TestFunctionMapType tests;
    for (TestFunctionMapType::const_iterator it = test_functions_.begin();
         it != test_functions_.end(); ++it) {
      if (!is_name_execute(it->first)) {
        continue;
      }
      tests.insert(*it);
    }
    return tests;
  }

  void addTest(const std::string &name, TestFunctionType function) {
    test_functions_[name] = function;
  }

  bool execute() {
    TestFunctionMapType test_function = tests();
    bool fail = false;
    size_t testcount = test_function.size();
    size_t testindex = 1;

    std::vector<std::string> pass_tests;
    std::vector<std::string> fail_tests;
    for (TestFunctionMapType::const_iterator it = test_function.begin();
         it != test_function.end(); ++it, ++testindex) {
      const std::string &test_name = it->first;

      kaguya::State state;

      state.setErrorHandler(test_error_handler);

      std::cout << test_name << "  (" << testindex << "/" << testcount
                << ") ...";

      std::string error_message = "";
      std::cout << std::flush;
      bool result = false;
      try {
        it->second(state);
        result = true;
      } catch (std::exception &e) {
        result = false;
        std::cout << e.what() << std::endl;
        error_message = e.what();
      } catch (...) {
        result = false;
        std::cout << "unknown exception" << std::endl;
        error_message = "unknown exception";
      }

      if (result) {
        // test pass
        int stack_leak = lua_gettop(state.state());
        if (stack_leak == 0) {
          std::cout << "pass" << std::endl;
          pass_tests.push_back(test_name);
        } else {
          std::cout << "pass. but stack leaked, count=" << stack_leak
                    << std::endl;
          fail = true;
          error_message = "stack leaked, count =" + to_string(stack_leak);
          fail_tests.push_back(test_name + " msg: " + error_message);
        }
      } else {
        // test failure
        std::cout << "Failed" << std::endl;
        fail = true;
        fail_tests.push_back(test_name + "\n\t error msg: " + error_message);
      }
    }
    std::cout << std::endl;
    int percent = int(pass_tests.size() * 100 / test_function.size());
    std::cout << percent << "% tests passed." << std::endl;
    if (!fail_tests.empty()) {
      std::cout << "The following tests FAILED:" << std::endl;

      for (size_t i = 0; i < fail_tests.size(); ++i) {
        std::cout << fail_tests[i] << std::endl;
      }

      std::cout << std::endl;
    }
    return !fail;
  }
};

// test target class
struct TestClass {
  int intmember;
  std::string stringmember;
  TestClass() : intmember(0) {}
  TestClass(int a) : intmember(a) {}
  TestClass(const char *a) : intmember(0), stringmember(a) {}
  TestClass(std::string a) : intmember(0), stringmember(a) {}
  TestClass(int intmem, const std::string &strmem)
      : intmember(intmem), stringmember(strmem) {}
  TestClass(const std::string &strmem, int intmem)
      : intmember(intmem), stringmember(strmem) {}
  TestClass(const TestClass &src)
      : intmember(src.intmember), stringmember(src.stringmember) {}

  bool operator==(const TestClass &rhs) const {
    return intmember == rhs.intmember && stringmember == rhs.stringmember;
  }
  bool operator<(const TestClass &rhs) const {
    return intmember < rhs.intmember ||
           (intmember == rhs.intmember && stringmember < rhs.stringmember);
  }
  bool operator!=(const TestClass &rhs) const { return !(*this == rhs); }
  bool operator>(const TestClass &rhs) const { return (rhs < *this); }
  bool operator>=(const TestClass &rhs) const { return !(*this < rhs); }
  bool operator<=(const TestClass &rhs) const { return !(*this > rhs); }
  int getInt() const { return intmember; }
  void setInt(const int &n) { intmember = n; }
  std::string getString() const { return stringmember; }
  void setString(std::string str) { stringmember = str; }

  int default_arg(int a = 3, int b = 2, int c = 1) { return a * b * c; }
  void default_set(int a = 3, int b = 2, int c = 1) { setInt(a * b * c); }
  void default_set_overload(int a, int b = 2, int c = 1) { setInt(a * b * c); }
  void default_set_overload(std::string a, std::string b = "b",
                            std::string c = "c") {
    setString(a + b + c);
  }

  TestClass copy() const { return *this; }
  const TestClass &references() const { return *this; }
  const TestClass &const_references() const { return *this; }
  TestClass &references() { return *this; }
  TestClass *pointer() { return this; }
  const TestClass *const_pointer() const { return this; }
  kaguya::standard::shared_ptr<TestClass> shared_copy() {
    return kaguya::standard::shared_ptr<TestClass>(new TestClass(*this));
  }
};
}

#if KAGUYA_USE_CPP11
inline std::ostream &operator<<(std::ostream &os, std::nullptr_t) {
  return os << "nullptr";
}
#endif

#define TEST_CHECK(B)                                                          \
  if (!(B))                                                                    \
    throw std::runtime_error(std::string("failed.\nfunction:") +               \
                             __FUNCTION__ + std::string("\nline:") +           \
                             kaguya_test_util::to_string(__LINE__) +           \
                             "\nCHECKCODE:" #B);

#define TEST_CHECK_M(B, M)                                                     \
  if (!(B))                                                                    \
    throw std::runtime_error(M + std::string("\nfunction:") + __FUNCTION__ +   \
                             std::string("\nline:") +                          \
                             kaguya_test_util::to_string(__LINE__) +           \
                             "\nCHECKCODE:" #B);

#define TEST_COMPARE(A, B, OP)                                                 \
  if (!((A)OP(B))) \
{                                                        \
    \
std::stringstream ss;                                                          \
    \
ss << "failed.\nfunction:"                                                     \
   << __FUNCTION__ << std::endl                                                \
   \
<< " line:" << __LINE__ << std::endl                                        \
   \
<< " CHECKCODE:" #A #OP #B << std::endl                                     \
   \
<< " left:" << (A) << std::endl                                             \
   \
<< " right:" << (B) << std::endl;                                           \
    throw std::runtime_error(ss.str());                                        \
  \
}
#define TEST_EQUAL(A, B) TEST_COMPARE(A, B, ==)
#define TEST_COMPARE_EQ(A, B) TEST_COMPARE(A, B, ==)
#define TEST_COMPARE_NE(A, B) TEST_COMPARE(A, B, !=)
#define TEST_COMPARE_LT(A, B) TEST_COMPARE(A, B, <)
#define TEST_COMPARE_LE(A, B) TEST_COMPARE(A, B, <=)
#define TEST_COMPARE_GT(A, B) TEST_COMPARE(A, B, >)
#define TEST_COMPARE_GE(A, B) TEST_COMPARE(A, B, >=)

#define KAGUYA_TEST_GROUP_START(TEST_GROUP)                                    \
  namespace TEST_GROUP {                                                       \
  static const char *kaguya_test_group_name = #TEST_GROUP;
#define KAGUYA_TEST_GROUP_END(TEST_GROUP) }

#define KAGUYA_TEST_FUNCTION_DEF(TEST_NAME)                                    \
  void TEST_NAME(kaguya::State &);                                             \
  struct TEST_NAME##_register {                                                \
    TEST_NAME##_register() {                                                   \
      kaguya_test_util::TestRunner::instance().addTest(                        \
          std::string(kaguya_test_group_name) + "::" + #TEST_NAME,             \
          &TEST_NAME);                                                         \
    }                                                                          \
  } TEST_NAME##_register;                                                      \
  void TEST_NAME
