#include "kaguya/optional.hpp"
#include "test_util.hpp"

KAGUYA_TEST_GROUP_START(test_08_optional)

using namespace kaguya_test_util;

KAGUYA_TEST_FUNCTION_DEF(optional_construct)(kaguya::State&)
{
	kaguya::optional<int> opt1 = 1;
	TEST_EQUAL(1, *opt1);

	kaguya::optional<std::string> opt2 = std::string("abc");
	TEST_EQUAL("abc", *opt2);

	TEST_EQUAL(std::string("abc"), opt2.value_or("def"));

	const kaguya::optional<std::string> opt3 = opt2;
	TEST_EQUAL("abc", *opt2);
	TEST_EQUAL("abc", *opt3);
}

KAGUYA_TEST_FUNCTION_DEF(optional_copy)(kaguya::State&)
{
	kaguya::optional<const char*> s1 = "abc", s2; // constructor

	TEST_EQUAL(std::string("def"), s2.value_or("def"));
	s2 = s1; 
	s1 = "def";

	TEST_EQUAL(std::string("def"), s1.value_or("abc"));
	TEST_EQUAL(std::string("abc"), s2.value_or("def"));
	TEST_CHECK(s1);
	TEST_CHECK(s2);
	TEST_EQUAL(std::string("abc"), *s2);
	TEST_EQUAL(std::string("def"), *s1);
}

KAGUYA_TEST_FUNCTION_DEF(optional_ref)(kaguya::State&)
{
	std::string str = "abc";
	kaguya::optional<const std::string&> s1 = str; // constructor
	TEST_EQUAL(std::string("abc"), *s1);

	TEST_EQUAL(std::string("abc"), s1.value_or("def"));

	const kaguya::optional<const std::string&> s2 = str; // constructor
	TEST_CHECK(s2);
	TEST_EQUAL(std::string("abc"), *s2);
	TEST_EQUAL(std::string("abc"), s1.value_or("def"));
}


KAGUYA_TEST_FUNCTION_DEF(optional_value_error)(kaguya::State&)
{
	kaguya::optional<std::string> s1;

	TEST_CHECK(!s1);
	TEST_EQUAL(std::string("def"), s1.value_or("def"));
	try
	{
		s1.value();
	}
	catch (const kaguya::bad_optional_access&)
	{
		return;
	}
	TEST_CHECK(false);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error2)(kaguya::State&)
{
	kaguya::optional<const std::string&> s1;

	TEST_CHECK(!s1);
	TEST_EQUAL(std::string("def"), s1.value_or("def"));
	try
	{
		s1.value();
	}
	catch (const kaguya::bad_optional_access&)
	{
		return;
	}
	TEST_CHECK(false);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error3)(kaguya::State&)
{
	const kaguya::optional<std::string> s1;

	TEST_CHECK(!s1);
	TEST_EQUAL(std::string("def"), s1.value_or("def"));
	try
	{
		s1.value();
	}
	catch (const kaguya::bad_optional_access&)
	{
		return;
	}
	TEST_CHECK(false);
}
KAGUYA_TEST_FUNCTION_DEF(optional_value_error4)(kaguya::State&)
{
	const kaguya::optional<const std::string&> s1;

	TEST_CHECK(!s1);
	TEST_EQUAL(std::string("def"), s1.value_or("def"));
	try
	{
		s1.value();
	}
	catch (const kaguya::bad_optional_access&)
	{
		return;
	}
	TEST_CHECK(false);
}

#if KAGUYA_USE_CPP11
struct MoveOnlyClass
{
	MoveOnlyClass(int i) :member(i) {}
	int member;

	MoveOnlyClass(MoveOnlyClass&& src) :member(src.member) {}
	MoveOnlyClass& operator=(MoveOnlyClass&& src) {
		member = src.member;
		return *this;
	}
private:
	MoveOnlyClass();
	MoveOnlyClass(const MoveOnlyClass&);
	MoveOnlyClass& operator=(const MoveOnlyClass&);
};

KAGUYA_TEST_FUNCTION_DEF(optional_move)(kaguya::State&)
{
	kaguya::optional<MoveOnlyClass> s1 = MoveOnlyClass(43);
	kaguya::optional<MoveOnlyClass> s2; // constructor
	s2 = std::move(s1);
	s1 = MoveOnlyClass(12);
	TEST_EQUAL(43, s2->member);
	TEST_EQUAL(12, s1->member);
}
#endif



KAGUYA_TEST_GROUP_END(test_08_optional)
