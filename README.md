# Kaguya
C++ binding to Lua

Licensed under [Boost Software License](http://www.boost.org/LICENSE_1_0.txt)

## Requirements
- Lua 5.2 or 5.3(recommended)
- C++03 compiler + boost library or C++11 compiler. see kaguya/config.hpp


## Introduction

Kaguya is Lua binding library for C++ 
- header-file only

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
  state.openlibs();//open lua standar library
}
```

### Or free standing lua state
```c++
lua_State* l = luaL_newstate();
kaguya::State state(l);
state.openlibs();
lua_close(l);//need close
```

### Runnig code
```c++
  kaguya::State state;state.openlibs();
  state("a = \"test\"");//from string
  state.loadFile("path/to/luascript.lua");//from file
```

### Accessing values
```c++
  kaguya::State state;state.openlibs();
  state("a = \"test\"");
  std::string a_value = state["a"];
  assert(a_value == "test");
  
  state["tbl"]["value"] = 1;//tbl ={};tbl.value = 1 in lua
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

#### Registering object instance

```c++
state["ABC"].setClass(kaguya::ClassMetatable<ABC>()
	.addConstructor()
	.addConstructor<int>()
	.addMember("get_value", &ABC::value)
	.addMember("set_value", &ABC::setValue)
	);
	ABC abc(43);
	state["abc"] = &abc;
	state("assert(43 == abc:get_value())")
```

#### Registering function
```c++
void c_free_standing_function(int v){std::cout <<"c_free_standing_function called:" << v << std::endl}
state["fun"] = kaguya::function(&c_free_standing_function);
state["fun"](54); 
state("fun(22)");
```
