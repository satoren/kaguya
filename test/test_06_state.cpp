#include "kaguya/kaguya.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_06_state)

using namespace kaguya_test_util;


int error_count = 0;
void error_fun(int status, const char* message)
{
	error_count++;
}


std::string last_error_message;
void ignore_error_fun(int status, const char* message)
{
	last_error_message = message ? message : "";
}

KAGUYA_TEST_FUNCTION_DEF(set_error_function)(kaguya::State& state)
{
	error_count = 0;
	state.setErrorHandler(error_fun);
	TEST_CHECK(!state("awdorgkwl;gw"));

	TEST_CHECK(error_count == 1);
	state["yy"]["yy"]["yy"]();
	TEST_CHECK(error_count > 0);
}
KAGUYA_TEST_FUNCTION_DEF(function_call_error)(kaguya::State& state)
{
	error_count = 0;
	state.setErrorHandler(error_fun);

	state["errofun"] = kaguya::function(error_fun);
	state["errofun"](33);

	TEST_CHECK(error_count == 1);

	kaguya::LuaRef f;
	f.resume<void>();
	f.call<void>();
	TEST_COMPARE_EQ(f.threadStatus(), LUA_ERRRUN);
	TEST_COMPARE_EQ(state.newRef(1).threadStatus(), LUA_ERRRUN);
}

KAGUYA_TEST_FUNCTION_DEF(other_state)(kaguya::State& unused)
{
	lua_State* L = luaL_newstate();

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

KAGUYA_TEST_FUNCTION_DEF(load_string)(kaguya::State& state)
{
	kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
	TEST_CHECK(luafun() != false);
	state.setErrorHandler(ignore_error_fun);
	kaguya::LuaRef errorref = state.loadstring("function() e");//syntax error
	TEST_CHECK(!errorref);
}


KAGUYA_TEST_FUNCTION_DEF(load_with_other_env)(kaguya::State& state)
{
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
KAGUYA_TEST_FUNCTION_DEF(no_standard_lib)(kaguya::State&)
{
	kaguya::State state(kaguya::NoLoadLib());
	state.setErrorHandler(ignore_error_fun);
	TEST_CHECK(!state("assert(true)"));//can not call assert
}
KAGUYA_TEST_FUNCTION_DEF(load_lib_constructor)(kaguya::State&)
{
	{
		kaguya::LoadLibs libs;
		libs.push_back(kaguya::LoadLib("_G", luaopen_base));
		libs.push_back(kaguya::LoadLib(LUA_MATHLIBNAME, luaopen_math));

		kaguya::State state(libs);
		TEST_CHECK(state("assert(true)"));
		TEST_CHECK(state("assert(math.abs(-2) == 2)"));
		TEST_CHECK(lua_gettop(state.state()) == 0);//leak check
	}
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER>=1800)
	{//use c++11 initializer list
		kaguya::State state({ { "_G", luaopen_base },{ LUA_MATHLIBNAME, luaopen_math } });
		TEST_CHECK(state("assert(true)"));
		TEST_CHECK(state("assert(math.abs(-2) == 2)"));

		TEST_CHECK(lua_gettop(state.state()) == 0);//leak check
	}
#endif
}

bool errorOccurred = false;
std::string lastMsg;
void registerError(int status, const char* message)
{
	if (errorOccurred)
		throw std::runtime_error("Should have reset errorOccurred");
	errorOccurred = true;
	lastMsg = message;
}

struct Foo {};

void foobar1(int) { throw std::runtime_error("MyRuntimeError"); }
void foobar2(int, int) {}
void foobar3(int, Foo, int) {}

KAGUYA_TEST_FUNCTION_DEF(errorThrowing)(kaguya::State& state)
{
	state["foobar"] = kaguya::overload(foobar1, foobar2, foobar3);
	state["Foo"].setClass(kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());
	state.setErrorHandler(registerError);
	TEST_CHECK(!state("foobar()"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);
	lastMsg = lastMsg.substr(lastMsg.find("candidate is:\n"));
	lastMsg = lastMsg.substr(0, lastMsg.find("stack "));
	std::vector<std::string> parts = remove_empty(split(lastMsg, '\n'));
	TEST_EQUAL(parts.size(), 4);
	std::string intName = typeid(int).name();
	TEST_CHECK(parts[1].find(intName) != std::string::npos);
	TEST_CHECK(parts[2].find(intName + "," + intName) != std::string::npos);
	TEST_CHECK(parts[3].find(intName + "," + typeid(Foo).name() + "," + intName) != std::string::npos);

	errorOccurred = false;
	TEST_CHECK(!state("foobar(Foo.new(), 1, 1)"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("candidate is:") != std::string::npos);

	errorOccurred = false;
	TEST_CHECK(!state("foobar(1)"));
	TEST_EQUAL(errorOccurred, true);
	TEST_CHECK(lastMsg.find("MyRuntimeError") != std::string::npos);

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


KAGUYA_TEST_FUNCTION_DEF(load_from_stream)(kaguya::State& state)
{
	{
		std::stringstream sstream;
		sstream << "value=true";
		TEST_CHECK(state.dostream(sstream, "streamchunk"));
		TEST_EQUAL(state["value"], true);
	}

	{//BOM
		std::stringstream sstream;
		sstream << "\xEF\xBB\xBFvalue2=true";
		TEST_CHECK(state.dostream(sstream, "streamchunk"));
		TEST_EQUAL(state["value2"], true);
	}


	{//comment
		std::stringstream sstream;
		sstream << "#/bin/lua\n"
			       "value3=true";
		TEST_CHECK(state.dostream(sstream, "streamchunk"));
		TEST_EQUAL(state["value3"], true);
	}
	
}



struct CountLimitAllocator
{
	typedef void* pointer;
	typedef size_t size_type;
	size_type allocated_count;
	size_type allocate_limit;
	size_type allocate_block_max_size;
	CountLimitAllocator() :allocated_count(0), allocate_limit(0), allocate_block_max_size(0) {}
	CountLimitAllocator(size_type limit) :allocated_count(0), allocate_limit(limit), allocate_block_max_size(0){}
	CountLimitAllocator(size_type limit, size_type blockmax) :allocated_count(0), allocate_limit(limit), allocate_block_max_size(blockmax){}
	pointer allocate(size_type n)
	{
		if (allocate_limit != 0 && allocate_limit < allocated_count)
		{
			return 0;
		}
		if (allocate_block_max_size != 0 && allocate_block_max_size < n)
		{
			return 0;
		}
		allocated_count++;
		return std::malloc(n);
	}
	pointer reallocate(pointer p, size_type n)
	{
		if (allocate_block_max_size != 0 && allocate_block_max_size < n)
		{
			return 0;
		}
		return std::realloc(p, n);
	}
	void deallocate(pointer p, size_type n)
	{
		if (p)
		{
			allocated_count--;
		}
		std::free(p);
	}
};

struct alloctest { int data; };

KAGUYA_TEST_FUNCTION_DEF(allocator_test)(kaguya::State& )
{
	kaguya::standard::shared_ptr<CountLimitAllocator> allocator(new CountLimitAllocator);
	{
		kaguya::State state(allocator);
		if (!state.state())
		{	//can not use allocator e.g. using luajit
			return;
		}
		state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
		state("a='abc'");
		state["data"] = alloctest();
		TEST_CHECK(allocator->allocated_count > 0);
	}

	TEST_CHECK(allocator->allocated_count == 0);
}


KAGUYA_TEST_FUNCTION_DEF(allocation_error_test)(kaguya::State& )
{
	for (size_t alloclimit = 32; alloclimit < 512; ++alloclimit)
	{

		kaguya::standard::shared_ptr<CountLimitAllocator> allocator(new CountLimitAllocator(alloclimit));
		kaguya::State state(allocator);
		if (!state.state())
		{	//can not use allocator e.g. using luajit
			continue;
		}
		try
		{
			state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

			state["Foo"].setClass(kaguya::UserdataMetatable<Foo>().setConstructors<Foo()>());

			state["data"] = kaguya::NewTable();
			Foo foodata;
			for (size_t i = 0; i < alloclimit; ++i)
			{
				state("data[" +to_string(i) + "] ='abc'");
				state["data"][i+ alloclimit*2] = alloctest();
				state["data"][i+ alloclimit*3] = 1;
				state["data"][i + alloclimit * 4] = "str";
				state["data"][i + alloclimit * 5] = foodata;//copy
				state["data"][i + alloclimit * 6] = &foodata;//ptr
			}
		}
		catch (const kaguya::LuaMemoryError&)
		{
			continue;
		}
		TEST_CHECK(false);
	}
}



KAGUYA_TEST_FUNCTION_DEF(syntax_error_throw_test)(kaguya::State& state)
{
	state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
	try
	{
		state("tes terror");//syntax_error
	}
	catch (const kaguya::LuaSyntaxError& e)
	{
		std::string errormessage(e.what());
		TEST_CHECK(errormessage.find("terror") != std::string::npos);
		return;
	}
	TEST_CHECK(false);
}

KAGUYA_TEST_FUNCTION_DEF(running_error_throw_test)(kaguya::State& state)
{
	state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);
	try
	{
		state("error('error message')");//error
	}
	catch (const kaguya::LuaRuntimeError& e)
	{
		std::string errormessage(e.what());
		TEST_CHECK(errormessage.find("error message") != std::string::npos);
		return;
	}
	TEST_CHECK(false);
}

void throwUnknownError(int status, const char* message)
{
	kaguya::ErrorHandler::throwDefaultError(32323232, "unknown error");
}
KAGUYA_TEST_FUNCTION_DEF(unknown_error_throw_test)(kaguya::State& state)
{
	state.setErrorHandler(throwUnknownError);
	try
	{
		state("error('')");//error
	}
	catch (const kaguya::LuaUnknownError& e)
	{
		std::string errormessage(e.what());
		TEST_CHECK(errormessage.find("unknown error") != std::string::npos);
		return;
	}
	TEST_CHECK(false);
}

void throwErrorRunningError(int status, const char* message)
{
	throw kaguya::LuaErrorRunningError(LUA_ERRERR,"error handler error");
}
KAGUYA_TEST_FUNCTION_DEF(errorrunning_error_throw_test)(kaguya::State& state)
{
	state.setErrorHandler(throwErrorRunningError);
	try
	{
		state("error('')");//error
	}
	catch (const kaguya::LuaErrorRunningError& e)
	{
		std::string errormessage(e.what());
		TEST_CHECK(errormessage.find("error handler error") != std::string::npos);
		return;
	}
	TEST_CHECK(false);
}



KAGUYA_TEST_FUNCTION_DEF(this_typemismatch_error_test)(kaguya::State& state)
{
	state.setErrorHandler(ignore_error_fun);

	{
		last_error_message = "";
		std::stringstream sstream;
		sstream << "=true";
		state.dostream(sstream, "streamchunk");
		TEST_CHECK(last_error_message.find("streamchunk") != std::string::npos);
	}
}



#if LUA_VERSION_NUM >= 502
KAGUYA_TEST_FUNCTION_DEF(gc_error_throw_test)(kaguya::State&)
{
	try
	{
		kaguya::State state;
		state.setErrorHandler(kaguya::ErrorHandler::throwDefaultError);

		state("testtable ={}"
			"meta ={__gc=function() error('gc error') end}"
			"setmetatable(testtable,meta)"
			"testtable={}"
			);
		state.gc().collect();
	}
	catch (const kaguya::LuaGCError& e)
	{
		std::string errormessage(e.what());
		TEST_CHECK(errormessage.find("gc error") != std::string::npos);
		return;
	}
	TEST_CHECK(false);
}
#endif

KAGUYA_TEST_GROUP_END(test_06_state)
