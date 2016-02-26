#include <iostream>
#include <cassert>
#include <sstream>
#include "kaguya/kaguya.hpp"


inline std::string to_string(int v)
{
	char buffer[28] = { 0 };
	snprintf(buffer, sizeof(buffer), "%d", v);
	return buffer;
}

#define TEST_CHECK(B) if(!(B)) throw std::runtime_error( std::string("failed.\nfunction:") +__FUNCTION__  + std::string("\nline:") + to_string(__LINE__) + "\nCHECKCODE:" #B );

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

namespace t_01_primitive
{
	void bool_get(kaguya::State& state)
	{
		state("value = true");
		TEST_EQUAL(state["value"], true);
	};
	void int_get(kaguya::State& state)
	{
		state("value = 3");
		TEST_EQUAL(state["value"], int(3));
	};
	void string_get(kaguya::State& state)
	{
		state("value = 'test'");
		TEST_EQUAL(state["value"], "test");
		TEST_EQUAL(state["value"], std::string("test"));
	};
	void table_get1(kaguya::State& state)
	{
		state("value = {1,32,'teststr'}");

		TEST_EQUAL(state["value"][1], 1);
		TEST_EQUAL(state["value"][2], 32);
		TEST_EQUAL(state["value"][3], "teststr");
	};
	void table_get2(kaguya::State& state)
	{
		state("value={out=32,str='gjgj'}");
		state("value['in'] = 'test'");
		kaguya::LuaRef value = state["value"];

		TEST_EQUAL(value["str"], "gjgj");
		TEST_EQUAL(value["in"], "test");
		TEST_EQUAL(value["out"], "32");
	};

	void bool_set(kaguya::State& state)
	{
		state["value"] = true;
		assert(state["value"] == true);
		TEST_CHECK(state("assert(value == true)"));
	};
	void int_set(kaguya::State& state)
	{
		state["value"] = 3;
		TEST_CHECK(state("assert(value == 3)"));
	};
	void string_set(kaguya::State& state)
	{
		state["value"] = "test";
		TEST_CHECK(state("assert(value == 'test')"));
	};
	void table_set(kaguya::State& state)
	{
		state["value"] = kaguya::NewTable();
		state["value"]["abc"] = kaguya::NewTable();
		state["value"]["abc"]["def"] = 7;
		state["value"]["abc"]["bbb"] = "test";
		TEST_CHECK(state("assert(value.abc.def == 7 and value.abc.bbb == 'test')"));
	};

	enum testenum
	{
		Foo = 0,
		Bar = 1,
	};

	void enum_set(kaguya::State& state)
	{
		state["value"] = Foo;
		TEST_CHECK(state("assert(value == 0)"));
	};
	void enum_get(kaguya::State& state)
	{
		state("value = 1");
		TEST_CHECK(state["value"] == Bar);
	};
}

namespace t_02_classreg
{
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

	void default_constructor(kaguya::State& state)
	{
		state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
			.addConstructor()
			);

		TEST_CHECK(state("value = assert(ABC.new())"));
	};
	void int_constructor(kaguya::State& state)
	{
		state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
			.addConstructor<int>()
			.addMember("getInt", &ABC::getInt)
			);

		TEST_CHECK(state("value = assert(ABC.new(32))"));
		TEST_CHECK(state("assert(value:getInt() == 32)"));
	};
	void string_constructor(kaguya::State& state)
	{
		state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
			.addConstructor<const char*>()
			.addMember("getString", &ABC::getString)
			);

		TEST_CHECK(state("value = assert(ABC.new('string_value'))"));
		TEST_CHECK(state("assert(value:getString() == 'string_value')"));
	};
	void overloaded_constructor(kaguya::State& state)
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
	void copy_constructor(kaguya::State& state)
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

	void data_member_bind(kaguya::State& state)
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


	void overload_member_function(kaguya::State& state)
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


	void operator_bind(kaguya::State& state)
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
	void add_field(kaguya::State& state)
	{
		state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
			.addConstructor<int>()
			.addStaticField("TEST", 1)
			.addStaticField("TEST2", "343")
			.addStaticField("TEST3", 133.23)
			);

		TEST_CHECK(state("assert(ABC.TEST == 1)"));
		TEST_CHECK(state("assert(ABC.TEST2 == '343')"));
		TEST_CHECK(state("assert(ABC.TEST3 == 133.23)"));
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
	private:
		CopyableClass();
		int member;
	};
	void copyable_class_test(kaguya::State& state)
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
	void noncopyable_class_test(kaguya::State& state)
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



	void registering_object_instance(kaguya::State& state)
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
	void registering_derived_class(kaguya::State& state)
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
	void registering_shared_ptr(kaguya::State& state)
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

		state["shared_ptr_function"] = kaguya::overload(&receive_base_shared_ptr_function,&receive_shared_ptr_function);
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

	void shared_ptr_null(kaguya::State& state)
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


	void multiple_inheritance(kaguya::State& state)
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

	void add_property(kaguya::State& state)
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
	void add_property_case2(kaguya::State& state)
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
	void add_property_ref_check(kaguya::State& state)
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

		{//member retain check
			state["prop"] = PropHolder();
			TEST_CHECK(state("member = prop.mem"));
			TEST_CHECK(state("prop = nil"));
			state.garbageCollect();
			TEST_CHECK(state("member.a = 3232"));
		}
	}
}

namespace t_03_function
{
	int arg = 0;
	void free_standing_function(int r)
	{
		arg = r;
	}
	int free_standing_function2()
	{
		return 12;
	}

	void free_standing_function_test(kaguya::State& state)
	{
		state["ABC"] = &free_standing_function;
		state["ABC"](54);
		state["free2"] = &free_standing_function2;
		TEST_EQUAL(arg, 54);
		TEST_CHECK(state["free2"]() == 12.0);
	}

	struct Foo
	{
		std::string bar;
		void setBar(std::string b) { bar = b; }
	};

	void member_function_test(kaguya::State& state)
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
	void variadic_function_test(kaguya::State& state)
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

		state("var = Vari.new('abc')");
		ptr = state["var"];

		TEST_CHECK(ptr);
		TEST_EQUAL(ptr->args[0].get<std::string>(), "abc");
	}



	kaguya::standard::tuple<int, std::string> tuplefun()
	{
		return kaguya::standard::tuple<int, std::string>(12, "23");
	}

	void multireturn_pass_through_to_arg(int a, int b, int c, int d, int e)
	{
		TEST_EQUAL(a, 1);
		TEST_EQUAL(b, 2);
		TEST_EQUAL(c, 4);
		TEST_EQUAL(d, 8);
		TEST_EQUAL(e, 16);
	}

	void multi_return_function_test(kaguya::State& state)
	{
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
		kaguya::tie(a, b, c, d, e) = state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >();
		TEST_EQUAL(a, 1);
		TEST_EQUAL(b, 2);
		TEST_EQUAL(c, 4);
		TEST_EQUAL(d, 8);
		TEST_EQUAL(e, 16);

		using kaguya::standard::get;
		kaguya::standard::tuple<int, int, int, int, int> tuple_res = state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >();
		TEST_EQUAL(get<0>(tuple_res), 1);
		TEST_EQUAL(get<1>(tuple_res), 2);
		TEST_EQUAL(get<2>(tuple_res), 4);
		TEST_EQUAL(get<3>(tuple_res), 8);
		TEST_EQUAL(get<4>(tuple_res), 16);

		state["multireturn_pass_through_to_arg"] = kaguya::function(multireturn_pass_through_to_arg);
		state["multireturn_pass_through_to_arg"](state["multresfun"].call<kaguya::standard::tuple<int, int, int, int, int> >());
		state["multireturn_pass_through_to_arg"](state["multresfun"]());

		state["multresfun2"] = kaguya::function(tuplefun);

		state("a,b = multresfun2()");
		TEST_CHECK(state("assert(a == 12  and b == '23')"));
	}

	void vector_and_map_from_table_mapping(kaguya::State& state)
	{
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		state("arraytablefn =function() return {32,1,2,4,8,16} end");
		std::vector<int> b = state["arraytablefn"]();
		TEST_EQUAL(b.size(), 6);
		TEST_EQUAL(b[0], 32);
		TEST_EQUAL(b[1], 1);
		TEST_EQUAL(b[2], 2);
		TEST_EQUAL(b[3], 4);
		TEST_EQUAL(b[4], 8);
		TEST_EQUAL(b[5], 16);
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
		state("tablefn =function() return {a=32,b=1,c=2} end");
		std::map<std::string, int> m = state["tablefn"]();

		TEST_EQUAL(m["a"], 32);
		TEST_EQUAL(m["b"], 1);
		TEST_EQUAL(m["c"], 2);
#endif
	}

	void vector_and_map_to_table_mapping(kaguya::State& state)
	{
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		std::vector<double> v; v.push_back(3); v.push_back(13); v.push_back(2); v.push_back(99);
		state["v"] = v;
		TEST_CHECK(state("assert(v[1] == 3 and v[2] == 13 and v[3] == 2 and v[4] == 99)"));

#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
		std::map<std::string, double> m; m["a"] = 4; m["b"] = 32; m["c"] = 24;
		state["m"] = m;
		TEST_CHECK(state("assert(m['a'] == 4 and m['b'] == 32 and m['c'] == 24)"));
#endif
	}



	void coroutine(kaguya::State& state)
	{
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
			while (!cor3.isThreadDead())
			{
				results.push_back(cor3(3));
			}

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
			while (cor2.threadStatus() == LUA_YIELD)
			{
				cor2();
				yieldnum++;
			}


			TEST_EQUAL(yieldnum, 10);
		}
	}

	void corresult_to_main(kaguya::VariadicArgType args)
	{
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
	}
	void corresult_to_main2(kaguya::VariadicArgType args)
	{
		TEST_EQUAL(args.size(), 1);
		TEST_EQUAL(args[0], 6);
	}


	void coroutine_stack(kaguya::State& state)
	{
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
	}
	

	void pointerfun(VariFoo* pointer)
	{
		TEST_EQUAL(pointer, 0);
	}
	void const_pointerfun(const VariFoo* pointer)
	{
		TEST_EQUAL(pointer, 0);
	}
	std::string last_error_message;
	void ignore_error_fun(int status, const char* message)
	{
		last_error_message = message? message:"";
	}
	void zero_to_nullpointer(kaguya::State& state)
	{
		state["pointerfun"] = kaguya::function(pointerfun);
		TEST_CHECK(state("pointerfun(0)"));
		TEST_CHECK(state("pointerfun(nil)"));
		state["const_pointerfun"] = kaguya::function(const_pointerfun);
		TEST_CHECK(state("const_pointerfun(0)"));
		TEST_CHECK(state("const_pointerfun(nil)"));

		state.setErrorHandler(ignore_error_fun);
		TEST_CHECK(!state("pointerfun(32)"));// is error
		TEST_CHECK(!state("pointerfun('232')"));// is error
	}
	void noargs_to_nullpointer(kaguya::State& state)
	{
		state["pointerfun"] = kaguya::function(pointerfun);
		TEST_CHECK(state("pointerfun()"));
		state["const_pointerfun"] = kaguya::function(const_pointerfun);
		TEST_CHECK(state("const_pointerfun()"));
	}

	void reffun(Foo& ref)
	{
		ref.setBar("BarBar");
	}
	void arg_class_ref(kaguya::State& state)
	{
		state["Foo"].setClass(kaguya::ClassMetatable<Foo>()
			.addMember("setBar", &Foo::setBar)
			);

		Foo foo;
		state["reffun"] = kaguya::function(reffun);

		state["reffun"](kaguya::standard::ref(foo));
		TEST_EQUAL(foo.bar, "BarBar");
	}

	void native_function_call_test(kaguya::State& state)
	{
		using namespace kaguya::nativefunction;
		Foo foo;
		state.newRef(6).push();
		state.newRef(9).push();
		state.newRef(2).push();

		call(state.state(), &free_standing_function);

		lua_settop(state.state(), 0);
		state.newRef(&foo).push();
		state.newRef("Bar").push();
		call(state.state(), &Foo::setBar);
#if KAGUYA_USE_CPP11
		state.newRef(&foo).push();
		state.newRef(9).push();
		call(state.state(), [](Foo* foo, int b) {
			foo->setBar("fromlambda");
		});
		TEST_EQUAL(foo.bar, "fromlambda");

		std::string capture("capture");
		call(state.state(), [=](Foo* foo, int b) {
			foo->setBar(capture + "lambda");
		});
		TEST_EQUAL(foo.bar, "capturelambda");
#endif

		lua_settop(state.state(), 0);
	}

	int overload1()
	{
		return 1;
	}
	int overload2(const std::string&)
	{
		return 2;
	}
	int overload3(int)
	{
		return 3;
	}

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	int overload4(const std::vector<int>& a)
	{
		return 4;
	}
#endif
#ifndef KAGUYA_NO_STD_MAP_TO_TABLE	
	int overload5(const std::map<std::string,std::string>& a)
	{
		return 5;
	}
#endif
	int overload6(void*)
	{
		return 6;
	}


	void overload(kaguya::State& state)
	{
		state["overloaded_function"] = kaguya::overload(overload1, overload2, overload3
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
			, overload4
#endif
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
			, overload5
#endif
			, overload6
			);
		kaguya::LuaFunction f = state["overloaded_function"];
		TEST_EQUAL(f(), 1);
		TEST_EQUAL(f(""), 2);
		TEST_EQUAL(f(121), 3);

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		TEST_CHECK(state("assert(overloaded_function({3,4,2,4,5}) == 4)"));
#endif
#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
		TEST_CHECK(state("assert(overloaded_function({a='3',b='3'}) == 5)"));
#endif
		TEST_CHECK(state("assert(overloaded_function(nil) == 6)"));
		TEST_EQUAL(f((void*)0), 6);

	}


	void result_to_table(kaguya::State& state)
	{
		state["result_to_table"] = kaguya::function(overload1);
		state["result"] = state["result_to_table"]();
		TEST_EQUAL(state["result"], 1);
	}
}

namespace t_04_lua_ref
{
	void access(kaguya::State& state)
	{
		kaguya::LuaRef ref(state.state(), "abc");
		TEST_EQUAL(ref.get<std::string>(), "abc");

		state("abc={d =1,e=3,f=64,g='sss'}");
		kaguya::LuaRef abctable = state["abc"];

		TEST_EQUAL(abctable["d"] , 1);
		TEST_EQUAL(abctable["e"] , 3);
		TEST_EQUAL(abctable["f"] , 64);
		TEST_EQUAL(abctable["g"] , std::string("sss"));

		typedef std::map<kaguya::LuaRef, kaguya::LuaRef> maptype;
		const maptype& map = abctable.map();
		TEST_CHECK(map.size() == 4);

		std::map<std::string, std::string> strmap = abctable.map<std::string, std::string>();

		TEST_EQUAL(strmap["d"] , "1");
		TEST_EQUAL(strmap["e"] , "3");
		TEST_EQUAL(strmap["f"] , "64");
		TEST_EQUAL(strmap["g"] , "sss");
		TEST_EQUAL(strmap.size() , 4);

		abctable.setField("a", "test");
		TEST_CHECK(abctable["a"] == std::string("test"));

		abctable.setField("a", 22);
		TEST_CHECK(abctable["a"] == 22);
	}
	void newtable(kaguya::State& state)
	{
		kaguya::LuaTable ref = state.newTable();
		TEST_CHECK(kaguya::LuaRef(ref) == ref);
		TEST_CHECK(ref == kaguya::LuaRef(ref));

		ref["tbl"] = kaguya::NewTable();
		kaguya::LuaRef othertable = ref["tbl"];
		TEST_CHECK(othertable != ref);
		TEST_CHECK(ref != othertable);

		TEST_CHECK(othertable.map().size() == 0);
		TEST_CHECK(othertable.type() == kaguya::LuaRef::TYPE_TABLE);

		othertable["foo"] = 3;
		TEST_CHECK(othertable["foo"] == 3);
	}

	int free_standing_function()
	{
		return 12;
	}

	void callfunction(kaguya::State& state)
	{

		int ret = state["math"]["abs"](-32);
		TEST_CHECK(ret == 32);


		kaguya::LuaTable globalTable = state.globalTable();
		globalTable["tbl"] = kaguya::NewTable();
		kaguya::LuaRef tbl = globalTable["tbl"];

		state("tbl.fun=function() return 1 end");
		TEST_CHECK(tbl.map().size() == 1);

		int result = tbl["fun"]();
		TEST_CHECK(result == 1);

		state("tbl.value=6");
		state("tbl.memfun=function(self) return self.value end");
		result = (tbl->*"memfun")();//==tbl["memfun"](tbl) like tbl:memfun() in Lua

		TEST_CHECK(result == 6);

		globalTable["free2"] = kaguya::function(&free_standing_function);

		TEST_CHECK(state("assert(free2()==12)"));


		state("tbl.sum=function(...) local args = {...};"
			"local total=0;"
			"for i = 1, #args do "
			"  total = total + args[i];"
			" end "
			"return total;"
			"end");
		TEST_CHECK(tbl["sum"](1, 2, 3, 4, 5, 6, 7, 8) == 36);
		TEST_CHECK(tbl["sum"].call<int>(1, 2, 3, 4, 5, 6, 7, 8) == 36);

		//no return compile test
		tbl["sum"].call<void>(1, 2, 3, 4, 5, 6, 7, 8);

		state("tbl.retfun=function(tbl) return tbl.sum end");
		TEST_CHECK((tbl->*"retfun")()(1, 2, 3, 4, 5, 6, 7, 8) == 36)

			//callable check
			TEST_CHECK((tbl->*"retfun").call<kaguya::LuaFunction>()(1, 2, 3, 4, 5, 6, 7, 8) == 36)
			TEST_CHECK((tbl->*"retfun")().call<int>(1, 2, 3, 4, 5, 6, 7, 8) == 36)
	}

	struct ob
	{
		operator bool()const { return true; }
	};
	void test_operator_bool(kaguya::State& state)
	{
		kaguya::LuaRef table1 = state.newTable();
		TEST_CHECK(ob() != table1);
		kaguya::LuaTable newTable2 = state.newTable();
		kaguya::LuaTable tabl1_ref_copy = table1;
		TEST_CHECK(newTable2 != table1);
		TEST_CHECK(!(newTable2 == table1));
		TEST_CHECK(table1 == tabl1_ref_copy);
	}
	void typetest_function(kaguya::State& state)
	{
		kaguya::LuaRef newTable = state.newTable();

		kaguya::LuaRef function = state.newRef(kaguya::function(&free_standing_function));
		kaguya::LuaRef function2 = function;
		TEST_CHECK(function != newTable);
		TEST_CHECK(function == function2);
	}
	void operator_equal_test(kaguya::State& state)
	{
		kaguya::LuaTable globalTable = state.globalTable();

		TEST_CHECK(!(globalTable != state.globalTable()));
		TEST_CHECK(globalTable == state.globalTable());

		kaguya::LuaRef luanum = state.newRef(422);
		kaguya::LuaRef luanum2 = state.newRef(42);
		kaguya::LuaRef luanum3 = state.newRef(422);

		TEST_COMPARE_EQ(luanum, luanum3);
		TEST_COMPARE_NE(luanum, luanum2);
		TEST_COMPARE_LT(luanum2, luanum);
		TEST_COMPARE_LE(luanum2, luanum);
		TEST_COMPARE_GT(luanum, luanum2);
		TEST_COMPARE_GE(luanum, luanum2);

		TEST_CHECK(!(luanum != luanum3));
		TEST_CHECK(!(luanum < luanum3));
		TEST_CHECK(luanum <= luanum3);
		TEST_CHECK(!(luanum > luanum3));
		TEST_CHECK(luanum >= luanum3);
	}
	void typetest(kaguya::State& state)
	{
		kaguya::LuaRef luanum = state.newRef(422);
		TEST_CHECK(luanum.typeTest<int>());
		TEST_CHECK(!luanum.typeTest<std::string>());
		TEST_CHECK(!luanum.typeTest<ob>());
	}


	struct CallbackTest
	{
		void callCallback(int x)
		{
			if (function_)
			{
				function_(x);
			}
		}
		void setCallback(kaguya::LuaRef fun)
		{
			function_ = fun;
		}
		kaguya::LuaRef function_;
	};

	void luafun_callback(kaguya::State& state)
	{

		state["CallbackTest"].setClass(kaguya::ClassMetatable<CallbackTest>()
			.addConstructor()
			.addMember("setCallback", &CallbackTest::setCallback)
			.addMember("callCallback", &CallbackTest::callCallback));


		state("callback = CallbackTest.new();"
			"receive_data ={};"
			"callback:setCallback(function(x) table.insert(receive_data,x) end);"
			"callback:callCallback(54)"
			"callback:callCallback(24)"
			);

		(state["callback"]->*"callCallback")(454);
		kaguya::LuaTable callback_data = state["receive_data"];

		TEST_CHECK(callback_data[1] == 54
			&& callback_data[2] == 24 && callback_data[3] == 454)

	}
	struct size_check_struct
	{
		char data[22];
	};

	void lua_ref_size(kaguya::State& state)
	{
#define TEST_SIZE(IDENTIFIER, VALUE, LEN) \
	state(#IDENTIFIER " = " #VALUE); \
	kaguya::LuaRef IDENTIFIER = state[#IDENTIFIER]; \
	TEST_EQUAL(IDENTIFIER.size() , LEN);

		TEST_SIZE(x, 17, 0);
		TEST_SIZE(s, 'hello world!', 12);
		TEST_SIZE(b, true, 0);
		TEST_SIZE(n, nil, 0);
		TEST_SIZE(f, function() return 3 end, 0);

#undef TEST_SIZE

		state["d"] = size_check_struct();
		kaguya::LuaRef d = state["d"];
		TEST_CHECK(d.size() >= sizeof(size_check_struct)); // fixme: determine exact size of userdata if possible
	}

	void function_env(kaguya::State& state)
	{
		kaguya::LuaFunction fun = state.loadstring("foo='bar'");

		fun.setFunctionEnv(kaguya::NewTable());
		fun();

		TEST_CHECK("assert(foo == nil)");
		state["functionEnv"] = fun.getFunctionEnv();
		TEST_CHECK("assert(functionEnv.foo == 'bar')");
	}


	void lua_table_get(kaguya::State& state)
	{
		state("value={out=32,str='gjgj'}");
		state("value['in'] = 'test'");
		kaguya::LuaTable table = state["value"];

		TEST_CHECK(table["str"] == "gjgj" && table["in"] == "test" &&  table["out"] == 32);

		table.isNilref();
	}

	void lua_table_set(kaguya::State& state)
	{
		kaguya::LuaTable globalTable = state.globalTable();
		state["value"] = kaguya::NewTable();
		state["value"]["abc"] = kaguya::NewTable();
		state["value"]["abc"]["def"] = 7;
		state["value"]["abc"]["bbb"] = "test";
		TEST_CHECK(state("assert(value.abc.def == 7 and value.abc.bbb == 'test')"));
	}

	void lua_table_size(kaguya::State& state)
	{
		kaguya::LuaTable table = state.newTable();
		TEST_CHECK(table.size() == 0);

		state("value = {1, 2, 'foo'}");
		kaguya::LuaTable table2 = state["value"];
		TEST_CHECK(table2.size() == 3);

		state("value[3] = nil");
		kaguya::LuaTable table3 = state["value"];
		TEST_CHECK(table3.size() == 2);

		state["value"] = kaguya::NewTable();
		state["value"][1] = "foo";
		state["value"][2] = "bar";
		state["value"][99] = "baz";
		state["value"]["index"] = "foo";
		kaguya::LuaTable table4 = state["value"];
		TEST_CHECK(table4.size() == 2);
	}

	void lua_table_reference(kaguya::State& state)
	{
		kaguya::LuaTable globalTable = state.globalTable();
		state["value"] = kaguya::NewTable();
		state["value"]["abc"] = kaguya::NewTable();
		state["value"]["abc"]["def"] = 7;
		TEST_CHECK(state("assert(value.abc.def == 7)"));
		state["value"]["abc"]["bbb"] = "test";
		TEST_CHECK(state("assert(value.abc.bbb == 'test')"));
		state["value"]["abc"]["ddd"] = state["value"]["abc"]["ccc"] = state["value"]["abc"]["bbb"];
		TEST_CHECK(state("assert(value.abc.ccc == 'test')"));
		TEST_CHECK(state("assert(value.abc.ddd == 'test')"));

		TEST_COMPARE_EQ(state["value"]["abc"]["ccc"], state["value"]["abc"]["bbb"]);
		TEST_COMPARE_EQ(state["value"]["abc"]["ccc"], "test");
		TEST_CHECK(!(state["value"]["abc"]["ccc"] != state["value"]["abc"]["bbb"]));
		TEST_CHECK(!(state["value"]["abc"]["ccc"] != "test"));
		TEST_COMPARE_NE(state["value"]["abc"]["ccc"], state["value"]["abc"]["def"]);
		TEST_COMPARE_NE(state["value"]["abc"]["ccc"], "tes");
	}

	void metatable(kaguya::State& state)
	{
		kaguya::LuaTable table = state.newTable();
		kaguya::LuaTable metatable = state.newTable();
		kaguya::LuaTable metatable_index = state.newTable();
		metatable_index["hana"] = "uta";
		metatable["__index"] = metatable_index;
		table.setMetatable(metatable);
		TEST_EQUAL(table["hana"], "uta");
		TEST_EQUAL(table.getMetatable(), metatable);
	}
	void stream_out_test(kaguya::State& state)
	{
		kaguya::LuaTable table = state.newTable();
		table[1] = 231;
		table[2] = 21;
		table[3] = 2;
		std::stringstream ss;
		std::string text;
		ss << table;
		text = ss.str();
		TEST_EQUAL(text, "{1=231,2=21,3=2}");

		ss.str("");
		ss << table[1];
		text = ss.str();
		TEST_EQUAL(text, "231");

		ss.str("");
		ss << state.newRef(323);
		text = ss.str();
		TEST_EQUAL(text, "323");

		ss.str("");
		ss << state.newRef("test_text");
		text = ss.str();
		TEST_EQUAL(text, "'test_text'");

		ss.str("");
		ss << state.newRef(true);
		text = ss.str();
		TEST_EQUAL(text, "true");
		ss.str("");
		ss << state.newRef(false);
		text = ss.str();
		TEST_EQUAL(text, "false");
		ss.str("");
		ss << state.newRef((void*)(0));
		text = ss.str();
		TEST_EQUAL(text, "nil");

		ss.str("");
		ss << state.newRef(&stream_out_test);
		text = ss.str();
		TEST_EQUAL(text, "function");


		ss.str("");
		ss << state.loadstring("return {1,2,3,4,5}")();
		text = ss.str();
		TEST_EQUAL(text, "{1=1,2=2,3=3,4=4,5=5}");

		ss.str("");
		ss << state.loadstring("return {table={1,2,3,4,5}}")();
		text = ss.str();
		TEST_EQUAL(text, "{'table'={...}}");

		ss.str("");
		ss << state.loadstring("return 2,3")();
		text = ss.str();
		TEST_EQUAL(text, "2,3");

		ss.str("");
		ss << state.loadstring("return 2,3")().result_at(0);
		text = ss.str();
		TEST_EQUAL(text, "2");

		ss.str("");
		ss << state.newRef(&ss);
		text = ss.str();
		TEST_CHECK(text.compare(0,strlen("userdata"), "userdata") ==0);
	}


	void luafun_loadstring(kaguya::State& state)
	{
		{
			kaguya::LuaRef f = kaguya::LuaFunction::loadstring(state.state(), "return function(table, key) return table['other_'..key] end")();
			kaguya::LuaTable table = state.newTable();
			table["x"] = 5;
			table["other_x"] = 55;
			int v = f(table, "x");
			TEST_EQUAL(v, 55);
		}

		{
			kaguya::LuaRef f = kaguya::LuaFunction::loadstring(state.state()
				, "return function(a) return 22,66 end")();

			kaguya::LuaRef forwardf = kaguya::LuaFunction::loadstring(state.state()
				, "return function(...) return ... end")();
			int a = 0; int b = 0;
			kaguya::tie(a, b) = f();
			TEST_EQUAL(a, 22);
			TEST_EQUAL(b, 66);
			a = 0; b = 0;
			kaguya::tie(a, b) = forwardf(f());
			TEST_EQUAL(a, 22);
			TEST_EQUAL(b, 66);
		}


	}


}

namespace t_05_error_handler
{
	int error_count = 0;
	void error_fun(int status, const char* message)
	{
		error_count++;
	}
	void set_error_function(kaguya::State& state)
	{
		error_count = 0;
		state.setErrorHandler(error_fun);
		TEST_CHECK(!state("awdorgkwl;gw"));

		TEST_CHECK(error_count == 1);
		state["yy"]["yy"]["yy"]();
		TEST_CHECK(error_count > 0);
	}
	void function_call_error(kaguya::State& state)
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
}

namespace t_06_state
{
	void other_state(kaguya::State& unused)
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

	void ignore_error_fun(int status, const char* message)
	{
	}
	void load_string(kaguya::State& state)
	{
		kaguya::LuaRef luafun = state.loadstring("assert(11 == 11);return true");
		TEST_CHECK(luafun() != false);
		state.setErrorHandler(ignore_error_fun);
		kaguya::LuaRef errorref = state.loadstring("function() e");//syntax error
		TEST_CHECK(!errorref);
	}


	void load_with_other_env(kaguya::State& state)
	{
		state.dostring("foo = 'bar'");

		state["otherEnv"] = kaguya::NewTable();

		state.dostring("foo = 'dar'", state["otherEnv"]);

		TEST_CHECK(state("assert(foo == 'bar')"));

		TEST_CHECK(state("assert(otherEnv.foo == 'dar')"));
	}
	void no_standard_lib(kaguya::State&)
	{
		kaguya::State state(kaguya::NoLoadLib());
		TEST_CHECK(!state("assert(true)"));//can not call assert
	}
	void load_lib_constructor(kaguya::State&)
	{
		{
			kaguya::LoadLibs libs;
			libs.push_back(kaguya::LoadLib("_G", luaopen_base));
			libs.push_back(kaguya::LoadLib(LUA_MATHLIBNAME, luaopen_math));

			kaguya::State state(libs);
			TEST_CHECK(state("assert(true)"));
			TEST_CHECK(state("assert(math.abs(-2) == 2)"));
		}
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER>=1800)
		{//use c++11 initializer list
			kaguya::State state({ { "_G", luaopen_base },{ LUA_MATHLIBNAME, luaopen_math } });
			TEST_CHECK(state("assert(true)"));
			TEST_CHECK(state("assert(math.abs(-2) == 2)"));
		}
#endif
	}
}

namespace t_07_any_type_test
{
	enum testenum
	{
		Foo = 0,
		Bar = 1,
	};


	template<class T, class T2> bool setget_eq(kaguya::State& state, T setvalue, T2 condvalue)
	{
		return state.newRef(setvalue) == condvalue;
	}

	template< class T2> struct callback_function
	{

		T2 v_;
		callback_function(T2& v) :v_(v) {}

		bool operator()(T2 v)
		{
			return v_ == v;
		}
	};
	template<class T, class T2> bool function_call(kaguya::State& state, T setvalue, T2 condvalue)
	{
		callback_function<T2> fun(condvalue);
		state["testfn"] = kaguya::function<bool(T2)>(fun);
		state["value"] = setvalue;
		return state("assert(testfn(value))");
	}

	template<class T, class T2>
	bool test(kaguya::State& state, T value1, T2 value2)
	{
		return setget_eq(state, value1, value2) && function_call(state, value1, value2);
	}
	void any_type_test(kaguya::State& state)
	{
		TEST_CHECK(test(state, true, true));
		TEST_CHECK(test(state, 0, 0));
		TEST_CHECK(test(state, "", std::string("")));
		TEST_CHECK(test(state, "false", std::string("false")));
		TEST_CHECK(test(state, std::string(""), std::string("")));
		TEST_CHECK(test(state, 1, 1));
		TEST_CHECK(test(state, kaguya::LuaRef(), kaguya::LuaRef()));
		TEST_CHECK(test(state, Foo, Foo));
		TEST_CHECK(test(state, Bar, Bar));

		TEST_CHECK(test(state, kaguya::LuaRef(), false));
		TEST_CHECK(test(state, (void*)0, false));
		TEST_CHECK(test(state, 0, true));
		TEST_CHECK(test(state, "", true));
		TEST_CHECK(test(state, "false", true));
		TEST_CHECK(test(state, std::string(""), true));
		TEST_CHECK(test(state, 1, true));
		TEST_CHECK(test(state, std::numeric_limits<float>::min(), std::numeric_limits<float>::min()));
		TEST_CHECK(test(state, std::numeric_limits<double>::min(), std::numeric_limits<double>::min()));
		TEST_CHECK(test(state, std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
		TEST_CHECK(test(state, std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
		TEST_CHECK(test(state, std::numeric_limits<kaguya::luaInt>::min(), std::numeric_limits<kaguya::luaInt>::min()));
		TEST_CHECK(test(state, std::numeric_limits<kaguya::luaInt>::max(), std::numeric_limits<kaguya::luaInt>::max()));
		TEST_CHECK(test(state, short(5), short(5)));
		TEST_CHECK(test(state, 1, state.newRef(1)));
		TEST_CHECK(test(state, "test", state.newRef("test")));
		TEST_CHECK(test(state, false, state.newRef(false)));
		TEST_CHECK(test(state, 5.674f, state.newRef(5.674f)));
		TEST_CHECK(test(state, 5.674, state.newRef(5.674)));
		TEST_CHECK(test(state, state.newRef(5.674f), 5.674f));
		TEST_CHECK(test(state, state.newRef(5.674), 5.674));
		TEST_CHECK(test(state, state.newRef(short(5)), short(5)));
		TEST_CHECK(test(state, state.newRef(long(5)), long(5)));

		{
			std::map<std::string, int> setmap;
			setmap["3"] = 23232;
			setmap["4"] = 232;
			setmap["5"] = 23;
			std::map<std::string, int> copy = setmap;
			TEST_CHECK(test(state, setmap, copy));
		}
		{
			std::vector<std::string> setvec;
			setvec.push_back("342");
			setvec.push_back("32");
			std::vector<std::string> copy = setvec;
			TEST_CHECK(test(state, setvec, copy));
		}
		{
			std::vector<std::vector<std::string> > setvec;
			setvec.push_back(std::vector<std::string>(12, "332"));
			setvec.push_back(std::vector<std::string>(42, "3232"));
			std::vector<std::vector<std::string> > copy = setvec;
			TEST_CHECK(test(state, setvec, copy));
		}
	}
}

#if KAGUYA_USE_CPP11

namespace t_08_cxx11_feature
{

	enum class testenumclass
	{
		Foo = 0,
		Bar = 1,
	};


	void enum_class_set(kaguya::State& state)
	{
		state["value"] = testenumclass::Foo;
		TEST_CHECK(state("assert(value == 0)"));
	};
	void enum_class_get(kaguya::State& state)
	{
		state("value = 1");
		TEST_CHECK(state["value"] == testenumclass::Bar);
	};



	struct MoveOnlyClass
	{
		MoveOnlyClass(int i) :member(i) {}
		int member;

		MoveOnlyClass(MoveOnlyClass&& src) :member(src.member) {}
	private:
		MoveOnlyClass();
		MoveOnlyClass(const MoveOnlyClass&);
		MoveOnlyClass& operator=(const MoveOnlyClass&);
	};
	void movable_class_test(kaguya::State& state)
	{
		state["MoveOnlyClass"].setClass(kaguya::ClassMetatable<MoveOnlyClass>()
			.addConstructor<int>()
			.addProperty("member", &MoveOnlyClass::member)
			);

		state["moveonly"] = MoveOnlyClass(2);

		const MoveOnlyClass* ref = state["moveonly"];
		TEST_CHECK(ref);
		TEST_CHECK(ref->member == 2);

		state("func =function(arg) return assert(arg.member == 5) end");
		state["func"](MoveOnlyClass(5));

		state.newRef(MoveOnlyClass(5));
	}

	void lambdafun(kaguya::State& state)
	{
		state["ABC"] = kaguya::function([](int a) {return a * 2; });
		int a = state["ABC"](54);
		TEST_EQUAL(a, 108);

		state["free2"] = kaguya::function([]() {return 12; });
		TEST_EQUAL(state["free2"](), 12.0);


		state["sum"] = kaguya::function([](kaguya::VariadicArgType args) {
			int sum = 0;
			for (int arg : args)
			{
				sum += arg;
			}

			TEST_EQUAL(args.size(), 10);
			TEST_EQUAL(args[0], 1);
			TEST_EQUAL(args[1], 2);
			TEST_EQUAL(args[2], 3);
			TEST_EQUAL(args[3], 4);
			TEST_EQUAL(args[4], 5);
			TEST_EQUAL(args[5], 6);
			TEST_EQUAL(args[6], 7);
			TEST_EQUAL(args[7], 8);
			TEST_EQUAL(args[8], 9);
			TEST_EQUAL(args[9], 10);
			return sum;
		});
		TEST_EQUAL(state["sum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 55);



		state["ssum"] = kaguya::function([](kaguya::VariadicArgType args) {
			std::string sum;
			for (std::string arg : args)
			{
				sum += arg;
			}
			return sum;
		});
		TEST_EQUAL(state["ssum"](1, 2, 3, 4, 5, 6, 7, 8, 9, 10), "12345678910");


		state["overload"] = kaguya::overload([](int) {return 1; },
			[](const std::string&) {return 2; },
			[]() {return 3; }
		);
		state("assert(overload(2) == 1)");//int version
		state("assert(overload('2') == 2)");//string version
		state("assert(overload() == 3)");//no argument version

	}

	void put_unique_ptr(kaguya::State& state)
	{
		state["MoveOnlyClass"].setClass(kaguya::ClassMetatable<MoveOnlyClass>()
			.addConstructor<int>()
			.addProperty("member", &MoveOnlyClass::member)
			);

		state["uniqueptr"] = std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(2));

		const MoveOnlyClass* ref = state["uniqueptr"];
		TEST_CHECK(ref);
		TEST_EQUAL(ref->member, 2);

		state("func =function(arg) return assert(arg.member == 5) end");
		TEST_CHECK(state["func"](std::unique_ptr<MoveOnlyClass>(new MoveOnlyClass(5))) == true);

	}
	void compare_null_ptr(kaguya::State& state)
	{
		kaguya::LuaRef nullref = state.newRef(nullptr);
		TEST_CHECK(nullref == nullptr);
	}
}
#endif

void test_error_handler(int status, const char* message)
{
	throw std::runtime_error(std::string(message));
}

typedef void(*test_function_t)(kaguya::State&);
typedef std::map<std::string, test_function_t> test_function_map_t;

bool execute_test(const test_function_map_t& testmap)
{
	bool fail = false;
	size_t testcount = testmap.size();
	size_t testindex = 1;

	std::vector<std::string> pass_tests;
	std::vector<std::string> fail_tests;
	for (test_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it, ++testindex)
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


int main()
{
	bool test_result = false;
	{
		test_function_map_t testmap;
#define ADD_TEST(function) testmap[#function] = &function;

		ADD_TEST(t_01_primitive::bool_get);
		ADD_TEST(t_01_primitive::int_get);
		ADD_TEST(t_01_primitive::string_get);
		ADD_TEST(t_01_primitive::table_get1);
		ADD_TEST(t_01_primitive::table_get2);
		ADD_TEST(t_01_primitive::bool_set);
		ADD_TEST(t_01_primitive::int_set);
		ADD_TEST(t_01_primitive::string_set);
		ADD_TEST(t_01_primitive::table_set);
		ADD_TEST(t_01_primitive::enum_set);
		ADD_TEST(t_01_primitive::enum_get);
		ADD_TEST(t_02_classreg::default_constructor);
		ADD_TEST(t_02_classreg::int_constructor);
		ADD_TEST(t_02_classreg::string_constructor);
		ADD_TEST(t_02_classreg::overloaded_constructor);
		ADD_TEST(t_02_classreg::copy_constructor);
		ADD_TEST(t_02_classreg::data_member_bind);
		ADD_TEST(t_02_classreg::overload_member_function);		
		ADD_TEST(t_02_classreg::operator_bind);
		ADD_TEST(t_02_classreg::add_field);
		ADD_TEST(t_02_classreg::copyable_class_test);
		ADD_TEST(t_02_classreg::noncopyable_class_test);
		ADD_TEST(t_02_classreg::registering_object_instance);
		ADD_TEST(t_02_classreg::registering_derived_class);
		ADD_TEST(t_02_classreg::registering_shared_ptr);
		ADD_TEST(t_02_classreg::shared_ptr_null);
		ADD_TEST(t_02_classreg::multiple_inheritance);
		ADD_TEST(t_02_classreg::add_property);
		ADD_TEST(t_02_classreg::add_property_case2);
		ADD_TEST(t_02_classreg::add_property_ref_check);
		ADD_TEST(t_03_function::free_standing_function_test);
		ADD_TEST(t_03_function::member_function_test);
		ADD_TEST(t_03_function::variadic_function_test);
		ADD_TEST(t_03_function::multi_return_function_test);
		ADD_TEST(t_03_function::vector_and_map_from_table_mapping);
		ADD_TEST(t_03_function::vector_and_map_to_table_mapping);
		ADD_TEST(t_03_function::coroutine);
		ADD_TEST(t_03_function::coroutine_stack);
		ADD_TEST(t_03_function::zero_to_nullpointer);
		ADD_TEST(t_03_function::noargs_to_nullpointer);
		
		ADD_TEST(t_03_function::arg_class_ref);

		ADD_TEST(t_03_function::native_function_call_test);
		ADD_TEST(t_03_function::overload);
		ADD_TEST(t_03_function::result_to_table);

		ADD_TEST(t_04_lua_ref::access);
		ADD_TEST(t_04_lua_ref::newtable);
		ADD_TEST(t_04_lua_ref::callfunction);
		ADD_TEST(t_04_lua_ref::test_operator_bool);
		ADD_TEST(t_04_lua_ref::typetest_function);
		ADD_TEST(t_04_lua_ref::operator_equal_test);
		ADD_TEST(t_04_lua_ref::typetest);
		ADD_TEST(t_04_lua_ref::luafun_callback);
		ADD_TEST(t_04_lua_ref::lua_ref_size);
		ADD_TEST(t_04_lua_ref::function_env);
		ADD_TEST(t_04_lua_ref::lua_table_get);
		ADD_TEST(t_04_lua_ref::lua_table_set);
		ADD_TEST(t_04_lua_ref::lua_table_size);
		ADD_TEST(t_04_lua_ref::lua_table_reference);
		ADD_TEST(t_04_lua_ref::luafun_loadstring);
		ADD_TEST(t_04_lua_ref::metatable);
		ADD_TEST(t_04_lua_ref::stream_out_test);
		ADD_TEST(t_05_error_handler::set_error_function);
		ADD_TEST(t_05_error_handler::function_call_error);
		ADD_TEST(t_06_state::other_state);
		ADD_TEST(t_06_state::load_string);
		ADD_TEST(t_06_state::load_with_other_env);
		ADD_TEST(t_06_state::no_standard_lib);
		ADD_TEST(t_06_state::load_lib_constructor);

		ADD_TEST(t_07_any_type_test::any_type_test);



#if KAGUYA_USE_CPP11
		ADD_TEST(t_08_cxx11_feature::enum_class_set);
		ADD_TEST(t_08_cxx11_feature::enum_class_get);
		ADD_TEST(t_08_cxx11_feature::movable_class_test);
		ADD_TEST(t_08_cxx11_feature::lambdafun);

		ADD_TEST(t_08_cxx11_feature::put_unique_ptr);

		ADD_TEST(t_08_cxx11_feature::compare_null_ptr);


#endif

		test_result = execute_test(testmap);
	}
	return test_result ? 0 : -1;
}
