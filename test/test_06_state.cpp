#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_06_state)

using namespace kaguya_test_util;

int error_count = 0;
std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
  error_count++;
}

KAGUYA_TEST_FUNCTION_DEF(set_error_function)(kaguya::State &state) {
  error_count = 0;
  state.setErrorHandler(ignore_error_fun);
  TEST_CHECK(!state("awdorgkwl;gw"));

  TEST_CHECK(error_count == 1);
  state["yy"]["yy"]["yy"]();
  TEST_CHECK(error_count > 0);
}
KAGUYA_TEST_FUNCTION_DEF(function_call_error)(kaguya::State &state) {
  error_count = 0;
  state.setErrorHandler(ignore_error_fun);

  state["errofun"] = kaguya::function(ignore_error_fun);
  state["errofun"](33);

  TEST_CHECK(error_count == 1);

  kaguya::LuaRef f;
  f.resume<void>();
  f.call<void>();
  TEST_COMPARE_EQ(f.threadStatus(), LUA_ERRRUN);
  TEST_COMPARE_EQ(state.newRef(1).threadStatus(), LUA_ERRRUN);
}

KAGUYA_TEST_FUNCTION_DEF(other_state)(kaguya::State &) {
  lua_State *L = luaL_newstate();

  kaguya::State state(L);
  kaguya::State state2(L);
  kaguya::State state3(L);
  kaguya::State state4(L);
  kaguya::State state5(L);

  state5["value"] = 1;
  TEST_EQUAL(state["value"], 1);
  TEST_EQUAL(state5["value"], state2["value"]);

  lua_close(L);
}

KAGUYA_TEST_FUNCTION_DEF(load_string)(kaguya::State &state) {
  kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
  TEST_CHECK(luafun() != false);
  state.setErrorHandler(ignore_error_fun);
  kaguya::LuaRef errorref = state.loadstring("function() e"); // syntax error
  TEST_CHECK(!errorref);
}

KAGUYA_TEST_FUNCTION_DEF(load_with_other_env)(kaguya::State &state) {
  state.dostring("foo = 'bar'");

  state["otherEnv"] = kaguya::NewTable();

  state.dostring("foo = 'dar'", state["otherEnv"]);

  TEST_CHECK(state("assert(foo == 'bar')"));

  TEST_CHECK(state("assert(otherEnv.foo == 'dar')"));

  std::stringstream sstream;
  sstream << "foo = 'beer'";
  TEST_CHECK(state.dostream(sstream, "streamchunk", state["otherEnv"]));
  TEST_CHECK(state("assert(foo == 'bar')"));
  TEST_CHECK(state("assert(otherEnv.foo == 'beer')"));
}
KAGUYA_TEST_FUNCTION_DEF(no_standard_lib)(kaguya::State &) {
  kaguya::State state(kaguya::NoLoadLib());
  state.setErrorHandler(ignore_error_fun);
  TEST_CHECK(!state("assert(true)")); // can not call assert
}
KAGUYA_TEST_FUNCTION_DEF(load_all_library)(kaguya::State &) {
  kaguya::State state(kaguya::NoLoadLib());
  state.openlibs();

  TEST_CHECK(state("assert(true)")); //
  TEST_CHECK(state("assert(load)")); //
  TEST_CHECK(state("assert(coroutine)"));
  TEST_CHECK(state("assert(debug)"));   //
  TEST_CHECK(state("assert(io)"));      //
  TEST_CHECK(state("assert(math)"));    //
  TEST_CHECK(state("assert(os)"));      //
  TEST_CHECK(state("assert(package)")); //
  TEST_CHECK(state("assert(string)"));  //
  TEST_CHECK(state("assert(table)"));   //
}

int lua_mylibf(lua_State *L) {
  kaguya::State state(L);
  kaguya::LuaTable t = state.newTable();
  t["value"] = 111;
  return t.push();
}

KAGUYA_TEST_FUNCTION_DEF(load_my_library)(kaguya::State &state) {
  state.openlib(kaguya::LoadLib("mylib", lua_mylibf));
  TEST_CHECK(state("assert(mylib.value == 111)"));
}

KAGUYA_TEST_FUNCTION_DEF(load_lib_constructor)(kaguya::State &) {
  {
    kaguya::LoadLibs libs;
    libs.push_back(kaguya::LoadLib("_G", luaopen_base));
    libs.push_back(kaguya::LoadLib(LUA_MATHLIBNAME, luaopen_math));

    kaguya::State state(libs);
    TEST_CHECK(state("assert(true)"));
    TEST_CHECK(state("assert(math.abs(-2) == 2)"));
    TEST_CHECK(lua_gettop(state.state()) == 0); // leak check
  }
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
  { // use c++11 initializer list
    kaguya::State state(
        {{"_G", luaopen_base}, {LUA_MATHLIBNAME, luaopen_math}});
    TEST_CHECK(state("assert(true)"));
    TEST_CHECK(state("assert(math.abs(-2) == 2)"));

    TEST_CHECK(lua_gettop(state.state()) == 0); // leak check
  }
#endif
}

bool errorOccurred = false;
std::string lastMsg;
void registerError(int status, const char *message) {
  KAGUYA_UNUSED(status);
  if (errorOccurred)
    throw std::runtime_error("Should have reset errorOccurred");
  errorOccurred = true;
  lastMsg = message;
}

struct Foo {};

void foobar1(int) { throw std::runtime_error("MyRuntimeError"); }
void foobar2(int, int) {}
void foobar3(int, Foo, int) {}
void foobar4(std::string) { throw 3; }

KAGUYA_TEST_FUNCTION_DEF(errorThrowing)(kaguya::State &state) {
  using kaguya::util::pretty_name;
  state["foobar"] = kaguya::overload(foobar1, foobar2, foobar3, foobar4);
  state["Foo"].setClass(
      kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());
  state.setErrorHandler(registerError);
  TEST_CHECK(!state("foobar()"));
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);
  lastMsg = lastMsg.substr(lastMsg.find("candidate is:\n"));
  lastMsg = lastMsg.substr(0, lastMsg.find("stack "));
  std::vector<std::string> parts = remove_empty(split(lastMsg, '\n'));
  TEST_EQUAL(parts.size(), 5);
  std::string intName = pretty_name(typeid(int));
  TEST_CHECK(parts[1].find(intName) != std::string::npos);
  TEST_CHECK(parts[2].find(intName + "," + intName) != std::string::npos);
  TEST_CHECK(parts[3].find(intName + "," + pretty_name(typeid(Foo)) + "," +
                           intName) != std::string::npos);
  TEST_CHECK(parts[4].find(pretty_name(typeid(std::string))) !=
             std::string::npos);

  errorOccurred = false;
  TEST_CHECK(!state("foobar(Foo.new(), 1, 1)"));
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);

  errorOccurred = false;
  TEST_CHECK(!state("foobar(1)"));
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("MyRuntimeError") != std::string::npos);

  errorOccurred = false;
  TEST_CHECK(!state("foobar('test')"));
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("Unknown") != std::string::npos);

  {
    std::stringstream sstream;
    sstream << "foobar(1)";
    errorOccurred = false;
    TEST_CHECK(!state.dostream(sstream, "streamchunk"));
    TEST_EQUAL(errorOccurred, true);
    TEST_CHECK(lastMsg.find("MyRuntimeError") != std::string::npos);
  }

#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
  errorOccurred = false;
  state["noTable"] = 5;
  std::map<int, int> map = state["noTable"];
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("type mismatch") != std::string::npos);

  errorOccurred = false;
  state["noTable"] = 5;
  std::vector<int> vec = state["noTable"];
  TEST_EQUAL(errorOccurred, true);
  TEST_CHECK(lastMsg.find("type mismatch") != std::string::npos);
#endif
}

KAGUYA_TEST_FUNCTION_DEF(load_from_stream)(kaguya::State &state) {
  state("value=nil");
  {
    std::stringstream sstream;
    sstream << "value=true";
    TEST_CHECK(state.dostream(sstream, "streamchunk"));
    TEST_EQUAL(state["value"], true);
  }

  state("value=nil");
  { // BOM
    std::stringstream sstream;
    sstream << "\xEF\xBB\xBFvalue=true";
    TEST_CHECK(state.dostream(sstream, "streamchunk"));
    TEST_EQUAL(state["value"], true);
  }

  state("value=nil");
  { // comment
    std::stringstream sstream;
    sstream << "#/bin/lua\n"
               "value=true";
    TEST_CHECK(state.dostream(sstream, "streamchunk"));
    TEST_EQUAL(state["value"], true);
  }

  state("value=nil");
  { // short comment
    std::stringstream sstream;
    sstream << "#\n"
               "value=true";
    TEST_CHECK(state.dostream(sstream, "streamchunk"));
    TEST_EQUAL(state["value"], true);
  }
  state("value=nil");
  {
    std::stringstream sstream;
    sstream << "value=true";
    TEST_CHECK(state.dostream(sstream));
    TEST_EQUAL(state["value"], true);
  }
  state("value=nil");
  {
    std::stringstream sstream;
    sstream << "value=true";
    state.loadstream(sstream)(); // loadstream and execute
    TEST_EQUAL(state["value"], true);
  }
  state("value=nil");
  {
    std::stringstream sstream;
    sstream << "value=true";
    state.loadstream(sstream,
                     "streamchunk")(); // loadstream and execute with chunkname
    TEST_EQUAL(state["value"], true);
  }
  state("value=nil");
  {
    kaguya::LuaTable env = state.newTable();
    std::stringstream sstream;
    sstream << "value=true";
    TEST_CHECK(state.dostream(sstream, "streamchunk",
                              env)); // dostream with chunkname and function env
    TEST_CHECK(!state["value"]);
    TEST_EQUAL(env["value"], true);
  }
}

struct CountLimitAllocator {
  typedef void *pointer;
  typedef size_t size_type;
  size_type allocated_count;
  size_type allocate_limit;
  size_type allocate_block_max_size;
  CountLimitAllocator()
      : allocated_count(0), allocate_limit(0), allocate_block_max_size(0) {}
  CountLimitAllocator(size_type limit)
      : allocated_count(0), allocate_limit(limit), allocate_block_max_size(0) {}
  CountLimitAllocator(size_type limit, size_type blockmax)
      : allocated_count(0), allocate_limit(limit),
        allocate_block_max_size(blockmax) {}
  pointer allocate(size_type n) {
    if (allocate_limit != 0 && allocate_limit < allocated_count) {
      return 0;
    }
    if (allocate_block_max_size != 0 && allocate_block_max_size < n) {
      return 0;
    }
    allocated_count++;
    return std::malloc(n);
  }
  pointer reallocate(pointer p, size_type n) {
    if (allocate_block_max_size != 0 && allocate_block_max_size < n) {
      return 0;
    }
    return std::realloc(p, n);
  }
  void deallocate(pointer p, size_type n) {
    KAGUYA_UNUSED(n);
    if (p) {
      allocated_count--;
    }
    std::free(p);
  }
};

struct alloctest {
  int data;
};

KAGUYA_TEST_FUNCTION_DEF(allocator_test)(kaguya::State &) {
  kaguya::standard::shared_ptr<CountLimitAllocator> allocator(
      new CountLimitAllocator);
  {
    kaguya::State state(allocator);
    if (!state.state()) { // can not use allocator e.g. using luajit
      return;
    }
    state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
    state("a='abc'");
    state["data"] = alloctest();
    TEST_CHECK(allocator->allocated_count > 0);
  }

  TEST_CHECK(allocator->allocated_count == 0);
}

KAGUYA_TEST_FUNCTION_DEF(allocation_error_test)(kaguya::State &) {
  for (size_t alloclimit = 32; alloclimit < 512; ++alloclimit) {
    kaguya::standard::shared_ptr<CountLimitAllocator> allocator(
        new CountLimitAllocator(alloclimit));
    kaguya::State state(allocator);
    if (!state.state()) { // can not use allocator e.g. using luajit
      continue;
    }
    try {
      state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

      state["Foo"].setClass(
          kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());

      state["data"] = kaguya::NewTable();
      Foo foodata;
      for (size_t i = 0; i < alloclimit; ++i) {
        state("data[" + to_string(i) + "] ='abc'");
        state["data"][i + alloclimit * 2] = alloctest();
        state["data"][i + alloclimit * 3] = 1;
        state["data"][i + alloclimit * 4] = "str";
        state["data"][i + alloclimit * 5] = foodata;  // copy
        state["data"][i + alloclimit * 6] = &foodata; // ptr
      }
    } catch (const kaguya::LuaMemoryError &) {
      continue;
    }
    TEST_CHECK(false);
  }
}

KAGUYA_TEST_FUNCTION_DEF(syntax_error_throw_test)(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
  bool catch_except = false;
  try {
    state("tes terror"); // syntax_error
  } catch (const kaguya::LuaSyntaxError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("terror") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  catch_except = false;
  try {
    std::stringstream sstream;
    sstream << "tes terror";
    state.loadstream(sstream)(); // syntax_error
  } catch (const kaguya::LuaSyntaxError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("terror") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

KAGUYA_TEST_FUNCTION_DEF(running_error_throw_test)(kaguya::State &state) {
  state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
  bool catch_except = false;
  try {
    state("error('error message')"); // error
  } catch (const kaguya::LuaRuntimeError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("error message") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

void throwUnknownError(int status, const char *message) {
  KAGUYA_UNUSED(status);
  KAGUYA_UNUSED(message);
  kaguya::ErrorHandler::throwDefaultError(32323232, "unknown error");
}
KAGUYA_TEST_FUNCTION_DEF(unknown_error_throw_test)(kaguya::State &state) {
  state.setErrorHandler(throwUnknownError);
  bool catch_except = false;
  try {
    state("error('')"); // error
  } catch (const kaguya::LuaUnknownError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("unknown error") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}

void throwErrorRunningError(int status, const char *message) {
  KAGUYA_UNUSED(status);
  KAGUYA_UNUSED(message);
  throw kaguya::LuaErrorRunningError(LUA_ERRERR, "error handler error");
}
void throwErrorRunningError2(int status, const char *message) {
  KAGUYA_UNUSED(status);
  KAGUYA_UNUSED(message);
  throw kaguya::LuaErrorRunningError(LUA_ERRERR,
                                     std::string("error handler error"));
}
KAGUYA_TEST_FUNCTION_DEF(errorrunning_error_throw_test)(kaguya::State &state) {
  state.setErrorHandler(throwErrorRunningError);
  bool catch_except = false;
  try {
    state("error('')"); // error
  } catch (const kaguya::LuaErrorRunningError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("error handler error") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);

  state.setErrorHandler(throwErrorRunningError2);
  catch_except = false;
  try {
    state("error('')"); // error
  } catch (const kaguya::LuaErrorRunningError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("error handler error") != std::string::npos);
    catch_except = true;
  }
}

KAGUYA_TEST_FUNCTION_DEF(this_typemismatch_error_test)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  {
    last_error_message = "";
    std::stringstream sstream;
    sstream << "=true";
    TEST_CHECK(!state.dostream(sstream, "streamchunk"));
    TEST_CHECK(last_error_message.find("streamchunk") != std::string::npos);
  }
}

#if LUA_VERSION_NUM >= 502
KAGUYA_TEST_FUNCTION_DEF(gc_error_throw_test)(kaguya::State &) {
  bool catch_except = false;
  try {
    kaguya::State state;
    state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

    state("testtable ={}"
          "meta ={__gc=function() error('gc error') end}"
          "setmetatable(testtable,meta)"
          "testtable={}");
    state.gc().collect();
  } catch (const kaguya::LuaGCError &e) {
    std::string errormessage(e.what());
    TEST_CHECK(errormessage.find("gc error") != std::string::npos);
    catch_except = true;
  }
  TEST_CHECK(catch_except);
}
#endif

KAGUYA_TEST_FUNCTION_DEF(full_gc_test)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  int used = state.gc().count();
  int first = used;
  state.gc().disable();
  for (int i = 0; i < 1000; i++) {
    state["a"] = kaguya::NewTable(i, i);
    TEST_COMPARE_LE(used, state.gc().count());
    used = state.gc().count();
  }
  state.gc().enable();
  state["a"] = kaguya::NilValue();
  state.gc().collect();
  int current = state.gc().count();
  TEST_COMPARE_GT(used, current);
  TEST_COMPARE_GE(first, current);
}
KAGUYA_TEST_FUNCTION_DEF(inc_gc_test)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  int used = state.gc().count();
  int peak = 0;
  for (int i = 0; i < 1000; i++) {
    state["a"] = kaguya::NewTable(i, i);
    used = state.gc().count();
    peak = std::max(peak, used);
  }
  TEST_COMPARE_LT(peak, 1500);
}

KAGUYA_TEST_FUNCTION_DEF(defailt_error_handler)(kaguya::State &) {
  kaguya::State state;
  state("a");
}
KAGUYA_TEST_GROUP_END(test_06_state)
