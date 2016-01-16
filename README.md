# Kaguya
C++ binding to Lua

Licensed under [Boost Software License](http://www.boost.org/LICENSE_1_0.txt)

[![Build Status](https://travis-ci.org/satoren/kaguya.svg?branch=master)](https://travis-ci.org/satoren/kaguya)
[![Build status](https://ci.appveyor.com/api/projects/status/cwlu28s42leacidx?svg=true)](https://ci.appveyor.com/project/satoren/kaguya)

## Requirements
- Lua 5.1 to 5.3 (recommended: 5.3)
- C++03 compiler with boost library or C++11 compiler(gcc 4.8+,clang 3.4+,MSVC2015) without boost.


## Introduction
Kaguya is Lua binding library for C++
- header-file only
- seamless access to lua elements


## run test
```
mkdir build
cd build
cmake ..
make
./test_runner
```

## Usage
### Create Lua context
```c++
#include "kaguya/kaguya.hpp"
int main()
{
  kaguya::State state;
  state.dofile("/path/to/script.lua")
}
```

### Or using exist lua state
```c++
extern "C" int luaopen_modulename(lua_State *L)
{
   kaguya::State state(l);
   kaguya::LuaRef module = state.newLib();
   module["function"] = kaguya::funtion(somefunction);
   return 1;//number of return lib
}
```

### Runnig Lua code
```c++
  kaguya::State state;
  state("a = 'test'");//from string
  state.loadFile("path/to/luascript.lua");//from file
```

### Accessing values
```c++
  kaguya::State state;
  state("a = \"test\"");
  std::string a_value = state["a"];
  assert(a_value == "test");

  state["tbl"] = kaguya::NewTable();//tbl ={};
  state["tbl"]["value"] = 1;//tbl.value = 1 in lua

  state("assert(tbl.value == 1)");

```


### Registering Classes
```c++
struct ABC
{
	ABC():v(0) {}
	ABC(int value) :v(value) {}
	int value()const { return v; }
	void setValue(int v) { v_ = v; }
private:
	int v_;
};
state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
	.addConstructor()
	.addConstructor<int>()
	.addMember("get_value", &ABC::value)
	.addMember("set_value", &ABC::setValue)
	);
```

```lua
--Lua
abc = ABC.new()--call default constructor
assert(0 == abc:get_value())
abc = ABC.new(42)--call (int) constructor
assert(42 == abc:get_value())
abc:set_value(30)
assert(30 == abc:get_value())
```
#### Registering inheritance
```c++
struct Base
{
	int a;
};
struct Derived:Base
{
	int b;
};
int base_function(Base* b) {
	b->a = 1;
	return b->a;
}
//
kaguya::State state;
state["Base"].setClass(kaguya::ClassMetatable<Base>()
  .addMember("a", &Base::a)
  );
state["Derived"].setClass(kaguya::ClassMetatable<Derived, Base>()
  .addMember("b", &Derived::b)
  );

state["base_function"] = &base_function;
Derived derived;
state["base_function"](&derived);//Base arguments function
state("assert(1 == derived:a())");//accessing Base member
```

#### Registering object instance
```c++
state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
	.addConstructor()
	.addConstructor<int>()
	.addMember("get_value", &ABC::value)
	.addMember("set_value", &ABC::setValue)
	);
	ABC abc(43);
  //register object pointer
	state["abc"] = &abc;
	state("assert(43 == abc:get_value())");
  //or copy instance
	state["copy_abc"] = abc;
	state("assert(43 == copy_abc:get_value())");
  //or registering shared instance
	state["shared_abc"] = kaguya::standard::shared_ptr<ABC>(new ABC(43));//kaguya::standard::shared_ptr is std::shared_ptr or boost::shared_ptr.
	state("assert(43 == shared_abc:get_value())");
```
#### Object lifetime
```c++
state["Base"].setClass(kaguya::ClassMetatable<Base>());
Base base;

//registering pointer. lifetime is same base
state["b"] = &base;

//registering copy instance. copied instance lifetime is handling in lua vm(garbage collection).
state["b"] = base;
state["b"] = static_cast<Base const&>(base);

```
### Registering function
```c++
void c_free_standing_function(int v){std::cout <<"c_free_standing_function called:" << v << std::endl}
state["fun"] = &c_free_standing_function;
state["fun"](54); //c_free_standing_function called:54
state("fun(22)");//c_free_standing_function called:22

state["lambda"] = kaguya::function([]{std::cout << "lambda called" << std::endl;});//C++11 lambda
state("lambda()");//lambda called
```

#### Variadic argments function
```c++
state["va_fun"] = kaguya::function([](kaguya::VariadicArgType arg) {for (auto v : arg) { std::cout << v.get<std::string>() << ","; }std::cout << std::endl; });//C++11 lambda
state("va_fun(3,4,6,\"text\",6,444)");//3,4,6,text,6,444,

```

#### Coroutine
```c++
kaguya::LuaThread cor = state.newThread();
state("corfun = function(arg)"
"coroutine.yield(arg) "
"coroutine.yield(arg*2) "
"coroutine.yield(arg*3) "
"return arg*4 "
" end");//define corouine function

kaguya::LuaFunction corfun = state["corfun"];//lua function get

//exec coroutine with function and argment
std::cout << int(cor(corfun, 3)) << std::endl;//3
std::cout << int(cor()) << std::endl;//6
std::cout << int(cor()) << std::endl;//9
std::cout << int(cor()) << std::endl;//12

kaguya::LuaThread cor2 = state.newThread();
//3,6,9,12,
while(!cor2.isThreadDead())
{
    std::cout << int(cor2(corfun, 3)) << ",";
}
```
