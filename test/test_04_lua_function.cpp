#include "test_util.hpp"
#include "kaguya/kaguya.hpp"

KAGUYA_TEST_GROUP_START(test_04_lua_function)
using namespace kaguya_test_util;

std::string last_error_message;
void ignore_error_fun(int status, const char *message) {
  KAGUYA_UNUSED(status);
  last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(function_call)(kaguya::State &state) {
  state("testfun = function(a,b,c,d,e) assert(a == 1,a) assert(b == 2,b) "
        "assert(c == 4,c) assert(d == 8,d) assert(e == 16,e) end");
  state["testfun"](1, 2, 4, 8, 16);
  state("testfun2 = function() return 3232 end");

  TEST_EQUAL(state["testfun2"](), 3232);
  state("testfun3 = function() return 'text' end");
  TEST_EQUAL(state["testfun3"](), "text");
}

KAGUYA_TEST_FUNCTION_DEF(multi_return_function_test)(kaguya::State &state) {
  state("multresfun =function() return 1,2,4,8,16 end");
  int a, b, c, d, e;
  a = b = c = d = e = 0;
  kaguya::tie(a, b, c, d, e) = state["multresfun"]();
  TEST_EQUAL(a, 1);
  TEST_EQUAL(b, 2);
  TEST_EQUAL(c, 4);
  TEST_EQUAL(d, 8);
  TEST_EQUAL(e, 16);

  a = b = c = d = e = 0;
  kaguya::tie(a, b, c, d, e) =
      state["multresfun"]
          .call<kaguya::standard::tuple<int, int, int, int, int> >();
  TEST_EQUAL(a, 1);
  TEST_EQUAL(b, 2);
  TEST_EQUAL(c, 4);
  TEST_EQUAL(d, 8);
  TEST_EQUAL(e, 16);

  using kaguya::standard::get;
  kaguya::standard::tuple<int, int, int, int, int> tuple_res =
      state["multresfun"]
          .call<kaguya::standard::tuple<int, int, int, int, int> >();
  TEST_EQUAL(get<0>(tuple_res), 1);
  TEST_EQUAL(get<1>(tuple_res), 2);
  TEST_EQUAL(get<2>(tuple_res), 4);
  TEST_EQUAL(get<3>(tuple_res), 8);
  TEST_EQUAL(get<4>(tuple_res), 16);

  state("multireturn_pass_through_to_arg = function(a,b,c,d,e) assert(a == "
        "1,a) assert(b == 2,b) assert(c == 4,c) assert(d == 8,d) assert(e == "
        "16,e) end");
  state["multireturn_pass_through_to_arg"](
      state["multresfun"]
          .call<kaguya::standard::tuple<int, int, int, int, int> >());
  state["multireturn_pass_through_to_arg"](state["multresfun"]());
}

KAGUYA_TEST_FUNCTION_DEF(coroutine)(kaguya::State &state) {
  {
    TEST_CHECK(state("cor = coroutine.create( function()"
                     "coroutine.yield(32) "
                     "coroutine.yield(53) "
                     "return 2 "
                     " end)"));

    kaguya::LuaRef cor = state["cor"];
    int r1 = cor();
    int r2 = cor();
    int r3 = cor();

    TEST_EQUAL(r1, 32);
    TEST_EQUAL(r2, 53);
    TEST_EQUAL(r3, 2);

    TEST_CHECK(state("cor3 = coroutine.create( function(arg)"
                     "coroutine.yield(arg) "
                     "coroutine.yield(arg*2) "
                     "coroutine.yield(arg*3) "
                     "return arg*4 "
                     " end)"));

    kaguya::LuaThread cor3 = state["cor3"];
    std::vector<int> results;
    while (!cor3.isThreadDead()) {
      results.push_back(cor3(3));
    }

    TEST_EQUAL(cor3.costatus(cor3), kaguya::COSTAT_RUNNING);
    TEST_EQUAL(results.size(), 4);
    TEST_EQUAL(results[0], 3);
    TEST_EQUAL(results[1], 6);
    TEST_EQUAL(results[2], 9);
    TEST_EQUAL(results[3], 12);
  }
  {
    kaguya::LuaThread cor2 = state.newRef(kaguya::NewThread());
    TEST_CHECK(state("corfun = function(arg)"
                     "coroutine.yield(arg) "
                     "coroutine.yield(arg*2) "
                     "coroutine.yield(arg*3) "
                     "return arg*4 "
                     " end"));

    kaguya::LuaFunction corfun = state["corfun"];
    int r1 = cor2(corfun, 3);
    int r2 = cor2.resume<int>();
    int r3 = cor2.resume<int>();
    int r4 = cor2.resume<int>();

    TEST_EQUAL(r1, 3);
    TEST_EQUAL(r2, 6);
    TEST_EQUAL(r3, 9);
    TEST_EQUAL(r4, 12);

    cor2.resume<void>(corfun, 3);
  }
  {

    state["cor2"] = kaguya::NewThread();
    kaguya::LuaRef cor2 = state["cor2"];
    TEST_CHECK(state("corfun = function(arg)"
                     "for i = 1,arg do "
                     "coroutine.yield() "
                     "end "
                     "end"));
    kaguya::LuaRef corfun = state["corfun"];
    cor2(corfun, 10);
    int yieldnum = 0;
    while (cor2.threadStatus() == LUA_YIELD) {
      cor2();
      yieldnum++;
    }

    TEST_EQUAL(yieldnum, 10);
  }
}

KAGUYA_TEST_FUNCTION_DEF(coroutine_yield_return)(kaguya::State &state) {
  {
    kaguya::LuaThread cor2 = state.newRef(kaguya::NewThread());
    TEST_CHECK(state("corfun = function(arg)"
                     "local r = coroutine.yield(arg) "
                     "r = coroutine.yield(r) "
                     "r = coroutine.yield(r) "
                     "return r "
                     " end"));

    kaguya::LuaFunction corfun = state["corfun"];
    int r1 = cor2(corfun, 3);
    int r2 = cor2.resume<int>(4);
    int r3 = cor2.resume<int>(5);
    int r4 = cor2.resume<int>(6);

    TEST_EQUAL(r1, 3);
    TEST_EQUAL(r2, 4);
    TEST_EQUAL(r3, 5);
    TEST_EQUAL(r4, 6);
  }
}

KAGUYA_TEST_FUNCTION_DEF(coroutine_dead)(kaguya::State &state) {
  kaguya::LuaThread emptycoroutine;
  TEST_CHECK(emptycoroutine.isThreadDead());

  TEST_CHECK(state("corfun = function(arg)"
                   "return 1 "
                   " end"));
  kaguya::LuaThread cor = state.newThread(state["corfun"]);

  TEST_CHECK(!cor.isThreadDead());
  cor();
  TEST_CHECK(cor.isThreadDead());
}

void corresult_to_main(kaguya::VariadicArgType args) {
  TEST_EQUAL(args.size(), 9);
  TEST_EQUAL(args[0], 1);
  TEST_EQUAL(args[1], 2);
  TEST_EQUAL(args[2], 3);
  TEST_EQUAL(args[3], 4);
  TEST_EQUAL(args[4], 5);
  TEST_EQUAL(args[5], 6);
  TEST_EQUAL(args[6], 7);
  TEST_EQUAL(args[7], 8);
  TEST_EQUAL(args[8], 9);
  int s = std::distance(args.begin(), args.end());
  TEST_EQUAL(s, 9);

  // iterator requirements test
  // Forward iterator
  kaguya::VariadicArgType::iterator it;       // default constructible
  it = args.begin();                          // copy -assignable
  kaguya::VariadicArgType::iterator it2 = it; // copy-constructible
  kaguya::VariadicArgType::iterator empty_it;
  TEST_CHECK(it2 == it);          // compare
  TEST_CHECK(empty_it != it);     // compare
  TEST_EQUAL(it->get<int>(), 1)   // dereferenced
  TEST_EQUAL((*it).get<int>(), 1) // dereferenced
  TEST_EQUAL(*it++, 1);           // incremented
  TEST_EQUAL(*(++it), 3);         // incremented
  it++;
  TEST_EQUAL(*it, 4); // incremented

  // Bidirectional	iterator
  it--;
  TEST_EQUAL(*it--, 3);   // decremented
  TEST_EQUAL(*(--it), 1); // decremented

  // Random access iterator
  it = args.begin();

  TEST_EQUAL(*(it += 5), 6); // compound assignment operations

  TEST_EQUAL(*it, 6);
  TEST_EQUAL(*(it - 5), 1);
  TEST_EQUAL(*(it - 3), 3);

  TEST_EQUAL(*(it -= 5), 1); // compound assignment operations

  TEST_EQUAL(*it, 1);
  TEST_EQUAL(*(it + 5), 6);
  TEST_EQUAL(*(it + 3), 4);
  TEST_EQUAL(*(5 + it), 6);
  TEST_CHECK(!((5 + it) < (4 + it)));
  TEST_CHECK(!((5 + it) < (5 + it)));
  TEST_CHECK(((5 + it) < (6 + it)));
  TEST_CHECK(((5 + it) > (4 + it)));
  TEST_CHECK(!((5 + it) > (5 + it)));
  TEST_CHECK(!((5 + it) > (6 + it)));
  TEST_CHECK(!((5 + it) <= (4 + it)));
  TEST_CHECK(((5 + it) <= (5 + it)));
  TEST_CHECK(((5 + it) <= (6 + it)));
  TEST_CHECK(((5 + it) >= (4 + it)));
  TEST_CHECK(((5 + it) >= (5 + it)));
  TEST_CHECK(!((5 + it) >= (6 + it)));
  TEST_CHECK(!((5 + it) <= (4 + it)));
  TEST_CHECK(((5 + it) <= (5 + it)));
  TEST_CHECK(((5 + it) <= (6 + it)));

  TEST_EQUAL(it[0], 1); // offset dereference operator
  TEST_EQUAL(it[6], 7); // offset dereference operator
  it += 5;
  TEST_EQUAL(it[0], 6);  // offset dereference operator
  TEST_EQUAL(it[-3], 3); // offset dereference operator

  it = args.begin();
  // use std::advance
  std::advance(it, 5);
  TEST_EQUAL(*it, 6);
  std::advance(it, -2);
  TEST_EQUAL(*it, 4);
}
void corresult_to_main2(kaguya::VariadicArgType args) {
  TEST_EQUAL(args.size(), 1);
  TEST_EQUAL(args[0], 6);
  int s = std::distance(args.begin(), args.end());
  TEST_EQUAL(s, 1);
}
void corresult_to_main3(kaguya::VariadicArgType args) {
  TEST_EQUAL(args.size(), 0);
  int s = std::distance(args.begin(), args.end());
  TEST_EQUAL(s, 0);
}

int coroutine_exec(kaguya::LuaThread cor) { return cor.resume<int>() * 2; }

KAGUYA_TEST_FUNCTION_DEF(coroutine_on_the_coroutine)(kaguya::State &state) {
  TEST_CHECK(state("cor1 = coroutine.create( function()"
                   "coroutine.yield(32) "
                   "coroutine.yield(53) "
                   "return 2 "
                   " end)"));

  state["coroutine_exec"] = &coroutine_exec;

  TEST_CHECK(state("cor2 = coroutine.wrap( function()"
                   "coroutine.yield(coroutine_exec(cor1)) "
                   "coroutine.yield(coroutine_exec(cor1)) "
                   "return 4 "
                   " end)"));

  kaguya::LuaRef cor = state["cor2"];
  int r1 = cor();
  int r2 = cor();
  int r3 = cor();

  TEST_EQUAL(r1, 32 * 2);
  TEST_EQUAL(r2, 53 * 2);
  TEST_EQUAL(r3, 4);

  kaguya::VariadicArgType vargtest(state.state(), 5);
  TEST_EQUAL(vargtest.size(), 0);
}

int function_exec(kaguya::LuaFunction f, kaguya::LuaStackRef v) {
  return f.call<int>(v) * 2;
}
KAGUYA_TEST_FUNCTION_DEF(function_on_the_coroutine)(kaguya::State &state) {
  TEST_CHECK(state("fn = function(a) return a end"));

  state["function_exec"] = &function_exec;

  TEST_CHECK(state("cor2 = coroutine.wrap( function()"
                   "coroutine.yield(function_exec(fn,3)) "
                   "coroutine.yield(function_exec(fn,4)) "
                   "return 4 "
                   " end)"));

  kaguya::LuaRef cor = state["cor2"];
  int r1 = cor();
  int r2 = cor();
  int r3 = cor();

  TEST_EQUAL(r1, 3 * 2);
  TEST_EQUAL(r2, 4 * 2);
  TEST_EQUAL(r3, 4);

  kaguya::VariadicArgType vargtest(state.state(), 5);
  TEST_EQUAL(vargtest.size(), 0);

  TEST_CHECK(cor.typeTest<kaguya::VariadicArgType>());
  TEST_CHECK(cor.weakTypeTest<kaguya::VariadicArgType>());
}

KAGUYA_TEST_FUNCTION_DEF(move_stack)(kaguya::State &state) {
  state.pushToStack(1);
  kaguya::LuaStackRef ref1(state.state(), -1);
  TEST_EQUAL(ref1, 1);
  state.pushToStack(2);
  kaguya::LuaStackRef ref2(state.state(), -1);
  TEST_EQUAL(ref2, 2);
  TEST_EQUAL(ref1, 1);

  kaguya::LuaThread cor = state.newThread();

  TEST_EQUAL(ref1, 1);
  ref1.pushStackIndex(cor);
  kaguya::LuaStackRef coref1(cor, -1);
  TEST_EQUAL(coref1, 1);
  ref2.pushStackIndex(cor);
  kaguya::LuaStackRef coref2(cor, -1);
  TEST_EQUAL(coref2, 2);

  state.popFromStack();
  state.popFromStack();
}

KAGUYA_TEST_FUNCTION_DEF(coroutine_stack)(kaguya::State &state) {
  state["corresult_to_main"] = &corresult_to_main;
  state["cor2"] = kaguya::NewThread();
  kaguya::LuaRef cor2 = state["cor2"];
  TEST_CHECK(state("corfun = function(arg)"
                   "for i = 1,arg do "
                   "coroutine.yield(1,2,3,4,5,6,7,8,9) "
                   "end "
                   "end"));
  state["corresult_to_main"](cor2(state["corfun"], 10));

  state["corresult_to_main2"] = &corresult_to_main2;
  state["corresult_to_main2"](cor2(state["corfun"], 10).result_at(5));

  state["corresult_to_main3"] = &corresult_to_main3;
  state["corresult_to_main3"]();

  TEST_CHECK(state("corfun = function(arg)"
                   "for i = 1,arg do "
                   "coroutine.yield({value=323}) "
                   "end "
                   "end"));

  state["cor3"] = kaguya::NewThread();
  state["value"] = state["cor3"](state["corfun"], 10).getField("value");
  TEST_EQUAL(state["value"], 323);
  TEST_CHECK(state["value"]);
  TEST_CHECK(state("assert(value == 323)"));
  TEST_EQUAL(state["value"],
             state["cor3"](state["corfun"], 10).getField<int>("value"));
}

void luacallback(const kaguya::standard::function<int(float)> &callback) {
  int ret = callback(32.f);
  TEST_EQUAL(ret, 1);
}

KAGUYA_TEST_FUNCTION_DEF(bind_to_std_function)(kaguya::State &state) {
  state["luacallback"] = &luacallback;
  state("luacallback(function(v) assert(32 == v) return 1 end)");
}

KAGUYA_TEST_FUNCTION_DEF(call_error)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  {
    last_error_message = "";
    kaguya::LuaFunction nilfn = state["a"];
    nilfn.call<void>();
    TEST_CHECK(last_error_message.find("attempt to call a nil value") !=
               std::string::npos);
  }

  {
    last_error_message = "";
    kaguya::LuaFunction nilfn;
    nilfn.call<void>();
  }
}

KAGUYA_TEST_FUNCTION_DEF(coroutine_error)(kaguya::State &state) {
  state.setErrorHandler(ignore_error_fun);

  {
    last_error_message = "";
    kaguya::LuaFunction nilfn = state["a"];
    nilfn.call<void>();
    TEST_CHECK(last_error_message.find("attempt to call a nil value") !=
               std::string::npos);
  }
  {

    TEST_CHECK(state("corfun = function(arg)"
                     "assert(false) "
                     " end"));
    kaguya::LuaThread cor = state.newThread(state["corfun"]);

    TEST_CHECK(!cor.isThreadDead());
    cor();
    TEST_CHECK(cor.isThreadDead());
  }
}

KAGUYA_TEST_FUNCTION_DEF(function_result_for)(kaguya::State &state) {
  {
    const kaguya::FunctionResults empty;
    TEST_EQUAL(empty.result_size(), 0);
    std::vector<int> res;
    for (kaguya::FunctionResults::iterator it = empty.begin();
         it != empty.end(); ++it) {
      res.push_back(*it);
    }
    TEST_EQUAL(res.size(), 0);
  }

  state("fn =function() return 1,2,4,8,16 end");
  {
    std::vector<int> res;
    kaguya::LuaFunction fn = state["fn"];
    kaguya::FunctionResults result = fn();
    for (kaguya::FunctionResults::iterator it = result.begin();
         it != result.end(); ++it) {
      res.push_back(*it);
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
    const kaguya::FunctionResults &result = fn();
    for (kaguya::FunctionResults::const_iterator it = result.begin();
         it != result.end(); ++it) {
      res.push_back(*it);
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
    kaguya::FunctionResults result = fn();
    TEST_EQUAL(result.result_size(), 5);
    TEST_EQUAL(result.resultStatus(), 0);
    for (kaguya::FunctionResults::const_iterator it = result.begin();
         it != result.end(); ++it) {
      res.push_back(*it);
    }
    TEST_EQUAL(res.size(), 5);
    TEST_EQUAL(res[0], 1);
    TEST_EQUAL(res[1], 2);
    TEST_EQUAL(res[2], 4);
    TEST_EQUAL(res[3], 8);
    TEST_EQUAL(res[4], 16);
  }

  state("a={} b={} c={}");
  state("fn =function() return a,b,c end");
  {
    kaguya::LuaFunction fn = state["fn"];
    kaguya::FunctionResults result = fn();
    for (kaguya::FunctionResults::iterator it = result.begin();
         it != result.end(); ++it) {
      it->setField("value", 5);
    }
    TEST_EQUAL(state["a"]["value"], 5);
    TEST_EQUAL(state["b"]["value"], 5);
    TEST_EQUAL(state["c"]["value"], 5);
    for (kaguya::FunctionResults::iterator it = result.begin();
         it != result.end(); ++it) {
      TEST_EQUAL(it->getField("value"), 5);
    }
  }
}

KAGUYA_TEST_FUNCTION_DEF(to_standard_function)(kaguya::State &state) {
  state("testfun = function() return 3232 end");
  kaguya::LuaRef testfunref = state["testfun"];

  TEST_CHECK(testfunref);
  TEST_CHECK(testfunref.typeTest<kaguya::standard::function<int()> >());
  TEST_CHECK(testfunref.weakTypeTest<kaguya::standard::function<int()> >());
  kaguya::standard::function<int()> testfun;

  testfun = kaguya::LuaRef().get<kaguya::standard::function<int()> >();
  TEST_CHECK(!testfun);

  testfun = testfunref.get<kaguya::standard::function<int()> >();
  TEST_EQUAL(testfun(), 3232);
  state("testfun3 = function() return 'text' end");
  TEST_EQUAL(state["testfun3"](), "text");
}

KAGUYA_TEST_FUNCTION_DEF(return_luastackref)(kaguya::State &state) {
  state("testfun = function() return 3232 end");
  kaguya::LuaRef testfunref = state["testfun"];

  {
    kaguya::LuaStackRef ret = testfunref.call<kaguya::LuaStackRef>();
    TEST_EQUAL(ret, 3232);
  }
  {
    kaguya::LuaStackRef ret = testfunref();
    TEST_EQUAL(ret, 3232);
  }
}
KAGUYA_TEST_GROUP_END(test_04_lua_function)
