
Tutorial
==================================

.. contents::
   :depth: 2
   :local:

Installation
----------------------------------
add "kaguya/include" directory to "header search path" of your project.

Or generate single header file and add it to your project.

.. code-block:: bash

  cd  utils
  python generate_one_header.py > ../kaguya.hpp


Hello world
----------------------------------

Create Lua context, and execute Lua code.

kaguya is automaticaly load all standard libraries.

.. code-block:: c++

  #include<kaguya/kaguya.hpp>
  int main()
  {
    using namespace kaguya;
    State state;//create Lua context
    state.dostring("print('hello lua world')");
  }





Initialize State
----------------------------------

1. default construct.

  Create lua_State and load all standard libraries.

  .. code-block:: c++

    kaguya::State state;

2. Load library control.

  Create lua_State without load libraries.

  .. code-block:: c++

    kaguya::State state(NoLoadLib());

  Create lua_State and load base library.

  .. code-block:: c++

    kaguya::LoadLibs libs;
    libs.push_back(kaguya::LoadLib("_G", luaopen_base));
    kaguya::State state(libs);


3. Memory management.

  .. warning::
    Do not work luajit on x64

  Create lua_State with allocator. Need Allocator concept.

  .. code-block:: c++

    kaguya::State state(std::make_shared<kaguya::DefaultAllocator>());

4. Wrap existing lua_State.

  .. code-block:: c++

    lua_State* L = luaL_newstate();//create lua_State by plain API
    {
      kaguya::State state(L);
    }
    lua_close(L);//Must lua_close yourself.



Runnig Lua code
----------------------------------

1. from string

.. code-block:: c++

  kaguya::State state;
  state.dostring("a = 1");

2. from file

.. code-block:: c++

  kaguya::State state;
  state.dofile("path/to/file.lua");


3. from stream

.. code-block:: c++

  kaguya::State state;
  std::ifstream ifs("path/to/file.lua", ios::in | ios::binary);
  state.dostream(ifs,"chunkname e.g filename");

Accessing Lua table values
----------------------------------

.. code-block:: c++

  kaguya::State state;
  state.dostring("a = 'test'");
  std::string a_value = state["a"];
  assert(a_value == "test");

  state["tbl"] = kaguya::NewTable();//tbl ={};
  state["tbl"]["value"] = 1;//tbl.value = 1 in lua

  state["tbl"] = kaguya::TableData{ 23,"test",{"key","value"}}; //using initializer list(C++11)
  state.dostring("assert(tbl[1] == 23)");
  state.dostring("assert(tbl[2] == 'test')");
  state.dostring("assert(tbl['key'] == 'value')");


Invoke lua function
----------------------------------

basic

.. code-block:: c++

  int ret = state["math"]["abs"](-32);
  assert(ret == 32);

Specified result type

.. code-block:: c++

  auto ret = state["math"]["abs"].call<int>(-32);
  assert(ret == 32);

Optional result value

.. code-block:: c++

  kaguya::optional<int> ret = state["math"]["abs"](-32);
  assert(ret && *ret == 32);

Multiple results

.. code-block:: c++

  state("multresfun =function() return 1,2,4 end");
  int a, b, c;
  kaguya::tie(a, b, c) = state["multresfun"]();
  std::cout << a << "," << b << "," << c << std::endl;//1,2,4


Registering c function
----------------------------------

free function

.. code-block:: c++

  void f(int v){std::cout <<"f called:" << v << std::endl}
  ...
  state["fun"] = &f;
  state["fun"](54); //f called:54
  state.dostring("fun(22)"); //f called:22

C++11 lambda

.. code-block:: c++

  state["lambda"] = kaguya::function([]{std::cout << "lambda called" << std::endl;});
  state.dostring("lambda()");//lambda called

function overloads

.. code-block:: c++

  void f1(int v){std::cout <<"int version" << std::endl}
  void f2(const std::string& v){std::cout <<"string version" << std::endl}
  void f3(){std::cout <<"no argument version" << std::endl}
  ...
  state["overload"] = kaguya::overload(f1,f2,f3);

  state.dostring("overload()");//no args version
  state.dostring("overload(2)");//int version
  state.dostring("overload('text')");//string version

default arguments

.. code-block:: c++

  int defargfn(int a = 3, int b = 2, int c = 1)
  {
      return a*b*c;
  }
  KAGUYA_FUNCTION_OVERLOADS(defargfn_wrapper, defargfn,0,3)
  ...
  state["defarg"] = kaguya::function(defargfn_wrapper());
  state.dostring("assert(defarg() == 6)");
  state.dostring("assert(defarg(6) == 12)");
  state.dostring("assert(defarg(6,5) == 30)");
  state.dostring("assert(defarg(2,2,2) == 8)");


Registering Classes
----------------------------------

.. code-block:: c++

  //C++
  struct ABC
  {
      ABC():v(0) {}
      ABC(int value) :v(value) {}
      int value()const { return v; }
      void setValue(int v) { v_ = v; }
      void overload() {std::cout << "call overload1"<<std::endl }
      void overload(int) {std::cout << "call overload2"<<std::endl }
      void default_arguments_function(int a = 3, int b = 2, int c = 1) { return return a * b * c; }
  private:
      int v_;
  };

  KAGUYA_MEMBER_FUNCTION_OVERLOADS(default_arguments_function_wrapper, ABC, default_arguments_function, 0, 3)
  ...
  state["ABC"].setClass(kaguya::UserdataMetatable<ABC>()
    .setConstructors<ABC(),ABC(int)>()
    .addFunction("get_value", &ABC::value)
    .addFunction("set_value", &ABC::setValue)
    .addOverloadedFunctions("overload", &ABC::overload1, &ABC::overload2)
    .addStaticFunction("nonmemberfun", [](ABC* self,int){return 1;})//c++11 lambda function
    .addFunction("defarg", default_arguments_function_wrapper())
    );

.. code-block:: Lua

  -- Lua
  local abc = ABC.new()
  assert(0 == abc:get_value())
  abc = ABC.new(42)--call (int) constructor
  assert(42 == abc:get_value())
  abc:set_value(30)
  assert(30 == abc:get_value())
  abc:overload() -- call overload1
  abc:overload(1) --call overload2

  assert(abc:defarg() == 6)
  assert(abc:defarg(6) == 12)
  assert(abc:defarg(6,5) == 30)
  assert(abc:defarg(2,2,2) == 8)


Handling Errors
----------------------------------

Lua error encountered will write to the console by default, and it is customizable:

.. code-block:: c++

  void HandleError(int errCode, const char * szError)
  {
    //customize your error handling, eg. write to file...
  }
  kaguya::State l;
  l.setErrorHandler(HandleError);
  l.dofile("./scripts/custom.lua"); // eg. accesing a file not existed will invoke HandleError above
