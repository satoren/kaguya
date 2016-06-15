#include "test_util.hpp"

int main()
{
	bool test_result = kaguya_test_util::TestRunner::instance().execute();
	return test_result ? 0 : -1;
}
