#include "kaguya/push_any.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_12_push_any)
using namespace kaguya_test_util;

KAGUYA_TEST_FUNCTION_DEF(push_any_type)(kaguya::State& state)
{
	using namespace kaguya;
	
	std::vector<AnyLuaData> data;
	data.push_back(3);
	data.push_back("data");
	data.push_back("abc");
	data.push_back(std::string("abc"));


	TEST_COMPARE_EQ(state.newRef(AnyLuaData(3)), 3);
	TEST_COMPARE_EQ(state.newRef(AnyLuaData("data")), "data");
	TEST_COMPARE_EQ(state.newRef(AnyLuaData(std::string("abc"))), std::string("abc"));
}

KAGUYA_TEST_GROUP_END(test_12_push_any)
