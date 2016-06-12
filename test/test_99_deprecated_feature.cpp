//ignore deprecated 
#define KAGUYA_DEPRECATED_FEATURE(MSG)

#include "kaguya/kaguya.hpp"
#include "test_util.hpp"


KAGUYA_TEST_GROUP_START(test_99_deprecated_feature)

using namespace kaguya_test_util;

struct ABC
{
	int intmember;
	std::string stringmember;

	ABC() :intmember(0) {}
	ABC(int a) :intmember(a) { }
	ABC(const char* a) :intmember(0), stringmember(a) { }
	ABC(std::string a) :intmember(0), stringmember(a) { }
	ABC(int intmem, const std::string& strmem) :intmember(intmem), stringmember(strmem) { }
	ABC(const std::string& strmem, int intmem) :intmember(intmem), stringmember(strmem) { }
	ABC(const ABC&src) :intmember(src.intmember), stringmember(src.stringmember) {}


	bool operator ==(const ABC& rhs)const {
		return intmember == rhs.intmember && stringmember == rhs.stringmember;
	}
	bool operator <(const ABC& rhs)const {
		return intmember < rhs.intmember || (intmember == rhs.intmember && stringmember < rhs.stringmember);
	}
	bool operator !=(const ABC& rhs)const {
		return !(*this == rhs);
	}
	bool operator >(const ABC& rhs)const {
		return (rhs < *this);
	}
	bool operator >=(const ABC& rhs)const {
		return !(*this < rhs);
	}
	bool operator <=(const ABC& rhs)const {
		return !(*this > rhs);
	}

	int getInt() const {
		return intmember;
	}
	void setInt(const int& n) {
		intmember = n;
	}
	std::string getString()const {
		return stringmember;
	}
	void setString(std::string str) { stringmember = str; }

	ABC copy()const { return *this; }
	const ABC& references()const { return *this; }
	const ABC& const_references()const { return *this; }
	ABC& references() { return *this; }
	ABC* pointer() { return this; }
	const ABC* const_pointer()const { return this; }
	kaguya::standard::shared_ptr<ABC> shared_copy() { return kaguya::standard::shared_ptr<ABC>(new ABC(*this)); }
};

KAGUYA_TEST_FUNCTION_DEF(default_constructor)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor()
		);

	TEST_CHECK(state("value = assert(ABC.new())"));
};
KAGUYA_TEST_FUNCTION_DEF(int_constructor)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addMember("getInt", &ABC::getInt)
		);

	TEST_CHECK(state("value = assert(ABC.new(32))"));
	TEST_CHECK(state("assert(value:getInt() == 32)"));
};
KAGUYA_TEST_FUNCTION_DEF(string_constructor)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<const char*>()
		.addMember("getString", &ABC::getString)
		);

	TEST_CHECK(state("value = assert(ABC.new('string_value'))"));
	TEST_CHECK(state("assert(value:getString() == 'string_value')"));
};
KAGUYA_TEST_FUNCTION_DEF(overloaded_constructor)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<const char*>()
		.addConstructor<int>()
		.addConstructor<std::string>()
		.addConstructor<int, const std::string&>()
		.addConstructor<const std::string&, int>()
		.addMember("getString", &ABC::getString)
		.addMember("getInt", &ABC::getInt)
		.addMember("setInt", &ABC::setInt)
		);

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
};
KAGUYA_TEST_FUNCTION_DEF(copy_constructor)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<const char*>()
		.addConstructor<int>()
		.addConstructor<std::string>()
		.addConstructor<int, const std::string&>()
		.addConstructor<const std::string&, int>()
		.addMember("getString", &ABC::getString)
		.addMember("setString", &ABC::setString)
		.addMember("getInt", &ABC::getInt)
		.addMember("references", static_cast<ABC& (ABC::*)()>(&ABC::references))
		.addMember("references", static_cast<const ABC& (ABC::*)()const>(&ABC::references))
		.addMember("const_pointer", &ABC::const_pointer)
		.addMember("pointer", &ABC::pointer)
		.addMember("copy", &ABC::copy)
		.addMember("shared_copy", &ABC::shared_copy)
		);


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
	state.garbageCollect();//warning!! value3 and value4 to dangling pointer
	TEST_CHECK(state("assert(value5:getString() == 'string_value3')"));
	TEST_CHECK(state("assert(value5:getInt() == 64)"));


	{
		kaguya::standard::shared_ptr<ABC> shared_ptr(new ABC("shared_object", 53));
		state["shared_object"] = shared_ptr;
	}
	TEST_CHECK(state("assert(shared_object:getInt() == 53)"));
	TEST_CHECK(state("assert(shared_object:getString() =='shared_object')"));


};

KAGUYA_TEST_FUNCTION_DEF(data_member_bind)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addMember("intmember", &ABC::intmember)
		.addConstructor<std::string>()
		.addMember("stringmember", &ABC::stringmember)
		);

	TEST_CHECK(state("value = assert(ABC.new(64))"));
	TEST_CHECK(state("assert(value:intmember() == 64)"));


	TEST_CHECK(state("value:intmember(4)"));
	TEST_CHECK(state("assert(value:intmember() == 4)"));


	TEST_CHECK(state("value:stringmember('test')"));
	TEST_CHECK(state("assert(value:stringmember() == 'test')"));
}


KAGUYA_TEST_FUNCTION_DEF(overload_member_function)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addMember("setmember", &ABC::intmember)
		.addMember("setmember", &ABC::stringmember)
		.addMember("intgdata", &ABC::intmember)
		.addMember("stringdata", &ABC::stringmember)
		);

	TEST_CHECK(state("value = assert(ABC.new(64))"));
	TEST_CHECK(state("assert(value:intgdata() == 64)"));

	TEST_CHECK(state("value:setmember(4)"));
	TEST_CHECK(state("assert(value:intgdata() == 4)"));

	TEST_CHECK(state("value:setmember('test')"));
	TEST_CHECK(state("assert(value:stringdata() == 'test')"));
}
struct Foo {
	int func() { return 1; }
	int func(int param) { return param; }
};
KAGUYA_TEST_FUNCTION_DEF(overload_member_function2)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addMemberFunction("func", static_cast<int (Foo::*)()>(&Foo::func))
		.addMemberFunction("func", static_cast<int (Foo::*)(int)>(&Foo::func))
		);
	state["foo"] = Foo();
	state("assert(foo:func() == 1)");
	state("assert(foo:func(64) == 64)");
}
KAGUYA_TEST_FUNCTION_DEF(overload_member_function3)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addStaticField("func", kaguya::overload(static_cast<int (Foo::*)()>(&Foo::func), static_cast<int (Foo::*)(int)>(&Foo::func)))
		);
	state["foo"] = Foo();
	state("assert(foo:func() == 1)");
	state("assert(foo:func(32) == 32)");
}

KAGUYA_TEST_FUNCTION_DEF(operator_bind)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addMember("__eq", &ABC::operator==)
		.addMember("__lt", &ABC::operator<)
		.addMember("__le", &ABC::operator<=)
		);

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
KAGUYA_TEST_FUNCTION_DEF(add_field)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addStaticField("TEST", 1)
		.addStaticField("TEST2", "343")
		.addStaticField("TEST3", 133.23)
		.addStaticField("TEST4", std::string("test"))
		);

	TEST_CHECK(state("assert(ABC.TEST == 1)"));
	TEST_CHECK(state("assert(ABC.TEST2 == '343')"));
	TEST_CHECK(state("assert(ABC.TEST3 == 133.23)"));
	TEST_CHECK(state("assert(ABC.TEST4 == 'test')"));
}


struct CopyableClass
{
	CopyableClass(int i) :member(i) {}
	bool operator==(const CopyableClass& rhs)const
	{
		return member == rhs.member;
	}
	bool operator!=(const CopyableClass& rhs)const
	{
		return member != rhs.member;
	}
	int member;
private:
	CopyableClass();
};
KAGUYA_TEST_FUNCTION_DEF(copyable_class_test)(kaguya::State& state)
{
	state["CopyableClass"].setClass(kaguya::ClassMetatable<CopyableClass>()
		.addConstructor<int>()
		.addMember("__eq", &CopyableClass::operator==)
		);
	state["copy"] = CopyableClass(2);
	TEST_CHECK(state["copy"] == CopyableClass(2));
	TEST_CHECK(CopyableClass(2) == state["copy"]);
	CopyableClass copy = state["copy"];
	TEST_CHECK(copy == CopyableClass(2));

	CopyableClass src(5);
	state["copied"] = src;
	src.member = 12;
	CopyableClass copied = state["copied"];
	TEST_CHECK(copied != src);
}


struct NoncopyableClass
{
	NoncopyableClass(int i) :member(i) {}
	bool operator==(const NoncopyableClass& rhs)const
	{
		return member == rhs.member;
	}
	bool operator!=(const NoncopyableClass& rhs)const
	{
		return member != rhs.member;
	}
private:
	NoncopyableClass();
	NoncopyableClass(const NoncopyableClass&);
	NoncopyableClass& operator=(const NoncopyableClass&);
	int member;
};
KAGUYA_TEST_FUNCTION_DEF(noncopyable_class_test)(kaguya::State& state)
{
	state["NoncopyableClass"].setClass(kaguya::ClassMetatable<NoncopyableClass>()
		.addConstructor<int>()
		.addMember("__eq", &NoncopyableClass::operator==)
		);

	NoncopyableClass noncopy(2);
	state["noncopy"] = &noncopy;

	const NoncopyableClass* noncpyref = state["noncopy"];
	TEST_CHECK(noncpyref == &noncopy);
}



KAGUYA_TEST_FUNCTION_DEF(registering_object_instance)(kaguya::State& state)
{
	state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
		.addConstructor<int>()
		.addMember("get_value", &ABC::getInt)
		);

	ABC abc(43);
	//register object pointer
	state["abc"] = &abc;
	state("assert(43 == abc:get_value())");
	//or copy instance
	state["copy_abc"] = abc;
	state("assert(43 == copy_abc:get_value())");
	//or registering shared pointer
	state["shared_abc"] = kaguya::standard::shared_ptr<ABC>(new ABC(43));//kaguya::standard::shared_ptr is std::shared_ptr or boost::shared_ptr.
	state("assert(43 == shared_abc:get_value())");
}

struct Base
{
	Base() :a(0) {};
	int a;
	void f(int,int,int){}
};
struct Derived :Base
{
	Derived() :b(0) {};
	int b;
};
struct Derived2 :Derived
{
	Derived2() :c(0) {};
	int c;
};

int base_function(Base* b) {
	TEST_CHECK(b);
	b->a = 1;
	return b->a;
}
int derived_function(Derived* d) {
	TEST_CHECK(d);
	d->b = 2;
	return d->b;
}
KAGUYA_TEST_FUNCTION_DEF(registering_derived_class)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addMember("a", &Base::a)
		);

	state["Derived"].setClass(kaguya::ClassMetatable<Derived, Base>()
		.addMember("b", &Derived::b)
		.addStaticMember("test", derived_function)
		);
	state["Derived2"].setClass(kaguya::ClassMetatable<Derived2, Derived>()
		.addMember("c", &Derived2::c)
		.addStaticMember("test", derived_function)
		);

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
KAGUYA_TEST_FUNCTION_DEF(registering_shared_ptr)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addMember("a", &Base::a)
		);

	state["Derived"].setClass(kaguya::ClassMetatable<Derived, Base>()
		.addMember("b", &Derived::b)
		);

	kaguya::standard::shared_ptr<Derived> derived(new Derived());
	kaguya::standard::shared_ptr<Base> base(new Base());
	state["base"] = base;
	state["derived"] = derived;
	state["base_function"] = &base_function;
	state["derived_function"] = &derived_function;
	state["receive_shared_ptr_function"] = &receive_shared_ptr_function;
	state["receive_base_shared_ptr_function"] = &receive_base_shared_ptr_function;



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

	state["shared_ptr_function"] = kaguya::overload(&receive_shared_ptr_function, &receive_base_shared_ptr_function);
	TEST_CHECK(state("assert(5 == shared_ptr_function(derived))"));
	TEST_EQUAL(derived->b, 5);
	TEST_CHECK(state("assert(2 == shared_ptr_function(base))"));
	TEST_EQUAL(base->a, 2);

	state["shared_ptr_function"] = kaguya::overload(&receive_base_shared_ptr_function, &receive_shared_ptr_function);
	TEST_CHECK(state("assert(5 == shared_ptr_function(derived))"));
	TEST_EQUAL(derived->b, 5);
	TEST_CHECK(state("assert(2 == shared_ptr_function(base))"));
	TEST_EQUAL(base->a, 2);


	//first function is always miss.second function typecheck
	state["shared_ptr_function"] = kaguya::overload(&registering_derived_class, &receive_base_shared_ptr_function);
	TEST_CHECK(state("assert(2 == shared_ptr_function(derived))"));
	TEST_EQUAL(derived->a, 2);

}

struct shared_ptr_fun
{
	kaguya::standard::shared_ptr<int>& ptr;
	shared_ptr_fun(kaguya::standard::shared_ptr<int>& r) :ptr(r) {}
	void operator()(kaguya::standard::shared_ptr<int> p)
	{
		ptr = p;
	}
};

KAGUYA_TEST_FUNCTION_DEF(shared_ptr_null)(kaguya::State& state)
{
	kaguya::standard::shared_ptr<int> ptr(new int(5));
	state["foo"] = kaguya::function<void(kaguya::standard::shared_ptr<int>)>(shared_ptr_fun(ptr));
	state("foo(nil)");
	TEST_EQUAL(ptr, 0);
}

struct Base2
{
	Base2() :b(0) {};
	int b;
	int test2() { return 1192; }
	int test() { return 794; }
	int consttest()const { return 794; }
};
struct MultipleInheritance :Base, Base2
{
	MultipleInheritance() :d(0) {};
	int d;
	int test() { return 1192; }
	int test3() { return 710; }
	int consttest2()const { return 1560; }
};


KAGUYA_TEST_FUNCTION_DEF(multiple_inheritance)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addProperty("a", &Base::a)
		);
	state["Base2"].setClass(kaguya::ClassMetatable<Base2>()
		.addProperty("b", &Base2::b)
		.addMember("test", &Base2::test)
		.addMember("test2", &Base2::test2)
		.addMember("consttest", &Base2::consttest)
		);
	state["MultipleInheritance"].setClass(kaguya::ClassMetatable<MultipleInheritance, kaguya::MultipleBase<Base, Base2> >()
		.addMember("d", &MultipleInheritance::d)
		.addProperty("propd", &MultipleInheritance::d)
		.addMember("test", &MultipleInheritance::test)
		.addMember("consttest2", &MultipleInheritance::consttest2)
		.addMember("test3", &MultipleInheritance::test3));

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


	state["constobj"] = static_cast<const MultipleInheritance*>(&data);
	TEST_CHECK(state("assert(constobj:consttest()==794)"));
	TEST_CHECK(state("assert(constobj:consttest2()==1560)"));
}

KAGUYA_TEST_FUNCTION_DEF(add_property)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);

	state["Derived"].setClass(kaguya::ClassMetatable<Derived, Base>()
		.addConstructor()
		.addProperty("b", &Derived::b)
		);

	Derived derived;
	Base base;
	const Base* constbase = &base;
	state["base"] = &base;
	state["derived"] = kaguya::standard::ref(derived);
	TEST_CHECK(state("base.a=1"));
	TEST_CHECK(state("derived.a = 2"));
	TEST_CHECK(state("derived.b=3"));
	TEST_CHECK(state("assert(1 == base.a)"));
	TEST_CHECK(state("assert(2 == derived.a)"));
	TEST_CHECK(state("assert(3 == derived.b)"));
	TEST_EQUAL(base.a, 1);
	TEST_EQUAL(derived.a, 2);
	TEST_EQUAL(derived.b, 3);

	TEST_CHECK(state("base2 = Base.new()"));
	TEST_CHECK(state("base2.a=5"));
	TEST_CHECK(state("assert(5 == base2.a)"));

	base.a = 3;
	state["constbase"] = constbase;
	TEST_CHECK(state("assert(3 == constbase.a)"));
}
KAGUYA_TEST_FUNCTION_DEF(add_property_case2)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);

	state["Derived"].setClass(kaguya::ClassMetatable<Derived, Base>()
		.addConstructor()
		);

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

struct Prop
{
	Prop() :a(0) {}
	int a;
};
struct PropHolder
{
	Prop mem;
	~PropHolder() {
	}
};
KAGUYA_TEST_FUNCTION_DEF(add_property_ref_check)(kaguya::State& state)
{
	state["Prop"].setClass(kaguya::ClassMetatable<Prop>()
		.addProperty("a", &Prop::a)
		);

	state["PropHolder"].setClass(kaguya::ClassMetatable<PropHolder>()
		.addProperty("mem", &PropHolder::mem)
		);

	PropHolder prop;
	state["prop"] = &prop;
	TEST_CHECK(state("prop.mem.a=455"));
	TEST_EQUAL(prop.mem.a, 455);

	{//member retain not supported now
//			state["prop"] = PropHolder();
//			TEST_CHECK(state("member = prop.mem"));
//			TEST_CHECK(state("prop = nil"));
//			state.garbageCollect();
//			TEST_CHECK(state("member.a = 3232"));
	}
}

KAGUYA_TEST_FUNCTION_DEF(object_take_copy)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);

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
	{//
		Base basesrc;
		Base& base = basesrc;
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
	{//
		Base basesrc;
		const Base& base = basesrc;
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
KAGUYA_TEST_FUNCTION_DEF(object_take_pointer)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);
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
KAGUYA_TEST_FUNCTION_DEF(object_take_reference)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);
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
KAGUYA_TEST_FUNCTION_DEF(object_take_const_reference)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);
	TEST_CHECK(state("fn = function(object,value) object.a = value end"));

	Base basesrc;
	basesrc.a = 232;
	const Base* base = &basesrc;
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
KAGUYA_TEST_FUNCTION_DEF(object_take_const_pointer)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		);
	TEST_CHECK(state("fn = function(object,value) object.a = value end"));

	Base basesrc;
	basesrc.a = 232;
	const Base& base = basesrc;
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

KAGUYA_TEST_FUNCTION_DEF(null_shared_ptr)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
	);


	state["test"] = kaguya::standard::shared_ptr<Base>();
	TEST_CHECK(state("assert(test==nil)"));


	state["test"] = kaguya::standard::shared_ptr<Derived>();

	TEST_CHECK(state("assert(test==nil)"));

	state["test"] = kaguya::standard::shared_ptr<void>();
	TEST_CHECK(state("assert(test==nil)"));

	kaguya::standard::shared_ptr<void> sptr = state["test"];
	TEST_CHECK(!sptr);
}

namespace FunctionTest
{
struct Foo
{
	std::string bar;
	void setBar(std::string b) { bar = b; }
};

struct Bar
{
	std::string member;
	Bar() {}
	Bar(const std::string& data):member(data){}
};
KAGUYA_TEST_FUNCTION_DEF(member_function_test)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addMember("setBar", &Foo::setBar)
		.addCodeChunkResult("luafunc", "return function(a,b) return a*b end")
		);
	Foo foo;
	kaguya::LuaRef lfoo = state["Foo"];
	lfoo["bar"] = kaguya::function(&Foo::setBar);
	lfoo["bar"](&foo, "bar");
	TEST_EQUAL(foo.bar, "bar");

	state["foo"] = &foo;

	state("foo:setBar('test')");
	TEST_EQUAL(foo.bar, "test");

	TEST_CHECK(state("assert(foo.luafunc(3,4) == 12)"));


	(state["foo"]->*"setBar")("test2");
	TEST_EQUAL(foo.bar, "test2");
#if KAGUYA_USE_CPP11
	lfoo["bar"] = kaguya::function<void(std::string)>([&foo](std::string str) { foo.bar = str; });
	TEST_CHECK(state("Foo.bar('test3')"));
	TEST_EQUAL(foo.bar, "test3");
	lfoo["bar"] = kaguya::function([&foo](std::string str) { foo.bar = str; });
	TEST_CHECK(state("Foo.bar('test4')"));
	TEST_EQUAL(foo.bar, "test4");
#else
#endif
}

struct VariFoo
{
	std::vector<kaguya::LuaRef> args;

	VariFoo() {}
	VariFoo(kaguya::VariadicArgType a) :args(a) {

	}

	void variadic_arg_func(kaguya::VariadicArgType args)
	{
		this->args = args;
	}

};
KAGUYA_TEST_FUNCTION_DEF(variadic_function_test)(kaguya::State& state)
{
	state["Vari"].setClass(kaguya::ClassMetatable<VariFoo>()
		.addConstructor()
		.addConstructorVariadicArg()
		.addMember("variadicfun", &VariFoo::variadic_arg_func)
		);
	state("var = Vari.new()");
	state("var:variadicfun('hanaregumi','hana-uta',5)");;
	VariFoo* ptr = state["var"];
	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "hanaregumi");
	TEST_EQUAL(ptr->args[1].get<std::string>(), "hana-uta");
	TEST_EQUAL(ptr->args[2].get<int>(), 5);

	bool typevalid;
	TEST_EQUAL(ptr->args[0].get<std::string>(typevalid), "hanaregumi");
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[1].get<std::string>(typevalid), "hana-uta");
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[2].get<int>(typevalid), 5);
	TEST_CHECK(typevalid);
	TEST_EQUAL(ptr->args[2].get<std::string>(typevalid), "5");
	TEST_CHECK(typevalid);
	ptr->args[2].get<std::string>(typevalid, false);
	TEST_CHECK(!typevalid);
	
	state("var:variadicfun()");
	TEST_EQUAL(ptr->args.size(), 0);


	state("var = Vari.new('abc')");
	ptr = state["var"];

	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "abc");

	state("var = Vari.new('abc', 'def')");
	ptr = state["var"];

	TEST_CHECK(ptr);
	TEST_EQUAL(ptr->args[0].get<std::string>(), "abc");
	TEST_EQUAL(ptr->args[1].get<std::string>(), "def");
}

void pointerfun(VariFoo* pointer)
{
	TEST_EQUAL(pointer, 0);
}
void const_pointerfun(const VariFoo* pointer)
{
	TEST_EQUAL(pointer, 0);
}
void reffun(Foo& ref)
{
	ref.setBar("BarBar");
}
KAGUYA_TEST_FUNCTION_DEF(arg_class_ref)(kaguya::State& state)
{
	state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
		.addMember("setBar", &Foo::setBar)
	);

	Foo foo;
	state["reffun"] = kaguya::function(reffun);

	state["reffun"](kaguya::standard::ref(foo));
	TEST_EQUAL(foo.bar, "BarBar");
}
}

std::string last_error_message;
void ignore_error_fun(int status, const char* message)
{
	last_error_message = message ? message : "";
}
KAGUYA_TEST_FUNCTION_DEF(error_check)(kaguya::State& state)
{
	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addProperty("a", &Base::a)
		.addMemberFunction("fa", &Base::a)
	);


	state["test"] = Base();
	TEST_CHECK(state("assert(test~=nil)"));

	TEST_CHECK(state("test.fa(test)"));
	TEST_CHECK(state("test:fa()"));

	state.setErrorHandler(ignore_error_fun);
	last_error_message = "";

	state("test.fa()");//error
	TEST_COMPARE_NE(last_error_message, "");
}

KAGUYA_TEST_FUNCTION_DEF(this_typemismatch_error_test)(kaguya::State& state)
{
	state.setErrorHandler(ignore_error_fun);

	state["Base"].setClass(kaguya::ClassMetatable<Base>()
		.addConstructor()
		.addMemberFunction("a", &Base::a)
		.addMemberFunction("f", &Base::f)
	);
	state["test"] = Base();
	TEST_CHECK(state("assert(test~=nil)"));
	TEST_CHECK(state("test.a(test)"));

	last_error_message = "";
	state("test.a()");

	TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);


	last_error_message = "";
	state("test.f(1,2,3,4,5,6,7,8,test)");

	TEST_CHECK(last_error_message.find("mismatch") != std::string::npos);
}

KAGUYA_TEST_GROUP_END(test_99_deprecated_feature)
