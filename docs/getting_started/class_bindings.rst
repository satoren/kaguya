
Class Bindings
==================================
C++ classes binding use :doc:`/api_reference/metatable`.


A quick example
-------------------------

Exposing classes to Lua

.. code-block:: c++

  //C++
  struct MyClass
  {
      MyClass():x(0),y() {}
      MyClass(int x,const std::string& y):x(x),y(y) {}
      void setX(int v){x=v;}
      int getX()const{return x;}
      void setY(const char* v){y=v;}
      const std::string& getY()const{return y;}
  private:
      int x;
      std::string y;
  };

  ...
  state["MyClass"] = kaguya::UserdataMetatable<MyClass>()
    .setConstructors<MyClass(),MyClass(int,const std::string&)>()
    .addFunction("setX", &MyClass::setX)
    .addFunction("getX", &MyClass::getX)
    .addProperty("y", &MyClass::getY, &MyClass::setY)
    ;


Usage in Lua

.. code-block:: lua

  local v = MyClass.new(4,'text')
  print(v:getX()) -- output 4
  print(v.y) -- output 'text'



Object lifetime
-------------------------

Copy assign
^^^^^^^^^^^^^^^^^^^^^^

  Basic assign.
  Copy object and managing lifetime by Lua.

  example:

  .. code-block:: c++

    {
      MyClass myobj;
      state["a"] = myobj;
    }
    state.dostring("print(a.y)");//myobj is destroyed, but a is living.
    state["a"] = 0;//override a.
    state.gc().collect();  //copied object is garbage collected.

Pointer assign
^^^^^^^^^^^^^^^^^^^^^^

  You need managing object lifetime by yourself.

  broken code example:

  .. code-block:: c++

    MyClass* myptr = new MyClass();
    state["a"] = myptr;
    delete myptr;
    state.dostring("print(a.y)")// a is destroyed. This is undefined behavior as dangling pointer.


Smartpointers
^^^^^^^^^^^^^^^^^^^^^^
.. _class-bindings-smartpointers:

  If you think troublesome for managing lifetime, can use shared_ptr.

  .. code-block:: c++

    kaguya::standard::shared_ptr<MyClass> mysptr = kaguya::standard::make_shared<MyClass>();
    state["a"] = mysptr;
    state.dostring("print(a.y)");

  .. note::

    If :ref:`KAGUYA_USE_CPP11<preprocessor-use-cpp11>` is 0,
    std::shared_ptr(and std::tr1::shared_ptr) is unrecognized, and vice versa.
    see :doc:`/api_reference/standard`
