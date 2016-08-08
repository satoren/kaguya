
LuaRef
==================================

LuaRef has strong reference to lua value.
It means not collect by gc.


LuaRef has specialized 4 types.

* LuaTable is LUA_TTABLE
* LuaUserData is LUA_TUSERDATA
* LuaFunction is LUA_TFUNCTION
* LuaThread is LUA_TTHREAD


example

.. code-block:: c++

  state.dostring("mytable={}");//create table
  kaguya::LuaRef table = state["mytable"];
  state.dostring("mytable.value = 3");
  int v = table["value"];//v is 3

  state.dostring("mytable.value = 6");//assign new value
  state.dostring("mytable=nil");//remove from global table
  state.dostring("collectgarbage()");//run fullgc
  v = table["value"];//v is 6.

.. toctree::
  :maxdepth: 1

  lua_ref
  lua_table
  lua_thread
  lua_userdata
  lua_function
