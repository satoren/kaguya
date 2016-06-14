#pragma once
#include <iostream>
#include <cassert>
#include <sstream>
#include "kaguya/kaguya.hpp"

namespace kaguya_test_util
{
	inline std::string to_string(int v)
	{
		char buffer[64] = {};
#if (defined(_MSC_VER) && _MSC_VER < 1900)
		sprintf_s(buffer, "%d", v);
#else
		snprintf(buffer, 64, "%d", v);
#endif
		return buffer;
	}

	inline std::vector<std::string> split(std::string s, const char delim)
	{
		std::vector<std::string> result;
		std::size_t pos;
		while ((pos = s.find(delim)) != std::string::npos)
		{
			result.push_back(s.substr(0, pos));
			s = s.substr(pos + 1);
		}
		result.push_back(s);
		return result;
	}
	inline std::vector<std::string> remove_empty(const std::vector<std::string>& src)
	{
		std::vector<std::string> result;
		result.reserve(src.size());
		for (std::vector<std::string>::const_iterator i = src.begin(); i != src.end(); ++i)
		{
			if (!i->empty())
			{
				result.push_back(*i);
			}
		}
		return result;
	}


	typedef void(*TestFunctionType)(kaguya::State&);

	class TestRunner
	{
		TestRunner() {}
		TestRunner(const TestRunner&);
		TestRunner& operator=(const TestRunner&);


		typedef std::map<std::string, TestFunctionType> TestFunctionMapType;

		TestFunctionMapType test_functions_;
	public:
		static TestRunner& instance()
		{
			static TestRunner ins;
			return ins;
		}
		static void test_error_handler(int status, const char* message)
		{
			throw std::runtime_error(std::string(message));
		}

		void addTest(const std::string& name, TestFunctionType function)
		{
			test_functions_[name] = function;
		}

		bool execute()
		{
			bool fail = false;
			size_t testcount = test_functions_.size();
			size_t testindex = 1;

			std::vector<std::string> pass_tests;
			std::vector<std::string> fail_tests;
			for (TestFunctionMapType::const_iterator it = test_functions_.begin(); it != test_functions_.end(); ++it, ++testindex)
			{
				kaguya::State state;

				state.setErrorHandler(test_error_handler);

				const std::string& test_name = it->first;

				std::cout << test_name << "  (" << testindex << "/" << testcount << ") ...";

				bool result = false;
				try
				{
					it->second(state);
					result = true;
				}
				catch (std::exception& e)
				{
					result = false;
					std::cout << e.what() << std::endl;
				}
				catch (...)
				{
					result = false;
					std::cout << "unknown exception" << std::endl;
				}

				if (result)
				{
					//test pass
					int stack_leak = lua_gettop(state.state());
					if (stack_leak == 0)
					{
						std::cout << "done" << std::endl;
						pass_tests.push_back(test_name);
					}
					else
					{
						std::cout << "stack leaked count=" << stack_leak << std::endl;
						fail = true;
						fail_tests.push_back(test_name);
					}
				}
				else
				{
					//test failure
					std::cout << "failure" << std::endl;
					fail = true;
					fail_tests.push_back(test_name);
				}
			}
			if (fail)
			{
				std::cout << "test failed!!" << std::endl;
				std::cout << "error in ";

				for (size_t i = 0; i < fail_tests.size(); ++i)
				{

					std::cout << fail_tests[i] << ",";
				}

				std::cout << std::endl;

			}
			else
			{
				std::cout << "all pass." << std::endl;
			}

			return !fail;
		}

	};
}

#define TEST_CHECK(B) if(!(B)) throw std::runtime_error( std::string("failed.\nfunction:") +__FUNCTION__  + std::string("\nline:") + kaguya_test_util::to_string(__LINE__) + "\nCHECKCODE:" #B );

#define TEST_COMPARE(A,B,OP)  if(!(A OP B))\
{\
std::stringstream ss;\
ss << "failed.\nfunction:" << __FUNCTION__ << std::endl \
<< " line:" << __LINE__<< std::endl \
<< " CHECKCODE:"#A #OP #B << std::endl \
<< " left:" << A << std::endl \
<< " right:" << B << std::endl; \
	throw std::runtime_error(ss.str());\
}
#define TEST_EQUAL(A,B)  TEST_COMPARE(A,B,==)
#define TEST_COMPARE_EQ(A,B)  TEST_COMPARE(A,B,==)
#define TEST_COMPARE_NE(A,B)  TEST_COMPARE(A,B,!=)
#define TEST_COMPARE_LT(A,B)  TEST_COMPARE(A,B,<)
#define TEST_COMPARE_LE(A,B)  TEST_COMPARE(A,B,<=)
#define TEST_COMPARE_GT(A,B)  TEST_COMPARE(A,B,>)
#define TEST_COMPARE_GE(A,B)  TEST_COMPARE(A,B,>=)


#define KAGUYA_TEST_GROUP_START(TEST_GROUP) namespace TEST_GROUP{ static const char* kaguya_test_group_name= #TEST_GROUP;
#define KAGUYA_TEST_GROUP_END(TEST_GROUP) }


#define KAGUYA_TEST_FUNCTION_DEF(TEST_NAME) void TEST_NAME(kaguya::State&);\
	struct TEST_NAME##_register\
	{\
		TEST_NAME##_register()\
		{\
			kaguya_test_util::TestRunner::instance().addTest(std::string(kaguya_test_group_name) + "::" + #TEST_NAME, &TEST_NAME);\
		}\
	} TEST_NAME##_register;\
	void TEST_NAME


	int main()
	{
		bool test_result = kaguya_test_util::TestRunner::instance().execute();
		return test_result ? 0 : -1;
	}
