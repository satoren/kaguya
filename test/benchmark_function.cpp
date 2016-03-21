#include "kaguya/kaguya.hpp"

namespace
{

	int test_native_function(int arg)
	{
		return arg;
	}
	class SetGet
	{
	public:
		SetGet() :_i(0.0) {}
		void set(double i)
		{
			_i = i;
		}
		double get()const
		{
			return _i;
		}
		double a;
	private:
		double _i;
	};
}

namespace kaguya_api_benchmark______
{
	void simple_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::ClassMetatable<SetGet>()
			.addConstructor()
			.addMember("set", &SetGet::set)
			.addMember("get", &SetGet::get)
			);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = 1000000\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void simple_get_set_contain_propery_member(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::ClassMetatable<SetGet>()
			.addConstructor()
			.addMember("set", &SetGet::set)
			.addMember("get", &SetGet::get)
			.addProperty("a", &SetGet::a)
			);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = 1000000\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void object_pointer_register_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::ClassMetatable<SetGet>()
			.addConstructor()
			.addMember("set", &SetGet::set)
			.addMember("get", &SetGet::get)
			);

		SetGet getset;
		state["getset"] = &getset;
		state(
			"local times = 1000000\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			);
	}
	void call_native_function(kaguya::State& state)
	{
		state["nativefun"] = &test_native_function;
		state(
			"local times = 1000000\n"
			"for i=1,times do\n"
			"local r = nativefun(i)\n"
			"if(r ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			);
	}
	void call_lua_function(kaguya::State& state)
	{
		state("lua_function=function(i)return i;end");

		kaguya::LuaRef lua_function = state["lua_function"];
		for (int i = 0; i < 1000000; i++)
		{
			int r = lua_function.call<int>(i);
			if (r != i) { throw std::logic_error(""); }
		}
	}
	void call_lua_function_operator_functional(kaguya::State& state)
	{
		state("lua_function=function(i)return i;end");

		kaguya::LuaFunction lua_function = state["lua_function"];
		for (int i = 0; i < 1000000; i++)
		{
			int r = lua_function(i);
			if (r != i) { throw std::logic_error(""); }
		}
	}
	
	void lua_table_access(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < 1000000; i++)
		{
			lua_table.setField("value", i);
			int v = lua_table.getField<int>("value");
			if (v != i) { throw std::logic_error(""); }
		}
	}

	void lua_table_bracket_operator_access(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < 1000000; i++)
		{
			lua_table["value"] = i;
			int v = lua_table["value"];
			if (v != i) { throw std::logic_error(""); }
		}
	}
	void lua_table_bracket_operator_assign(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < 1000000; i++)
		{
			lua_table.setField("value",i);
			int v = lua_table["value"];
			if (v != i) { throw std::logic_error(""); }
		}
	}
	void lua_table_bracket_operator_get(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < 1000000; i++)
		{
			lua_table["value"] = i;
			int v = lua_table.getField<int>("value");
			if (v != i) { throw std::logic_error(""); }
		}
	}

	struct Prop
	{
		Prop() :d(0) {}

		double d;
	};
	void property_access(kaguya::State& state)
	{
		state["Prop"].setClass(kaguya::ClassMetatable<Prop>()
			.addConstructor()
			.addProperty("d", &Prop::d)
			);

		state(
			"local getset = Prop.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = 1000000\n"
			"for i=1,times do\n"
			"getset.d =i\n"
			"if(getset.d ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void lua_allocation(kaguya::State& state)
	{
		state("lua_table = { } "
			"local times = 100000\n"
			"for i=1,times do\n"
			"lua_table['key'..i] =i\n"
			"end\n"
			"");
	}


	void vectormapping(kaguya::State& state)
	{
		state("lua_table={4,2,3,4,4,23,32,34,23,34,4,245,235,432,6,7,76,37,64,5,4}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < 100000; i++)
		{
			std::vector<double> r = lua_table;
		}
	
	}
}


namespace original_api_no_type_check
{
	int setget_new(lua_State* L)
	{
		void* ptr = lua_newuserdata(L, sizeof(SetGet));
		new(ptr) SetGet();
		luaL_setmetatable(L, "SetGet");
		return 1;
	}
	int setget_set(lua_State* L)
	{
		SetGet* setget = static_cast<SetGet*>(luaL_checkudata(L, 1, "SetGet"));
		setget->set(lua_tonumber(L,2));
		return 0;
	}
	int setget_get(lua_State* L)
	{
		SetGet* setget = static_cast<SetGet*>(luaL_checkudata(L, 1, "SetGet"));
		lua_pushnumber(L, setget->get());
		return 1;
	}

	void simple_get_set(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_openlibs(s);
		luaL_newmetatable(s,"SetGet");

		luaL_Reg funcs[] = 
		{
			{ "new",setget_new },
			{ 0 ,0 },
		};
		luaL_setfuncs(s, funcs, 0);

		lua_newtable(s);

		luaL_Reg indexfuncs[] =
		{
			{ "set",setget_set },
			{ "get",setget_get },
			{ 0 ,0 },
		};
		luaL_setfuncs(s, indexfuncs, 0);
		lua_setfield(s,-2,"__index");


		lua_setglobal(s, "SetGet");

		luaL_dostring(s,
			"local getset = SetGet.new()\n"
			"local times = 1000000\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
		lua_close(s);
	}


	int static_native_function_binding(lua_State* L)
	{
		int arg = static_cast<int>(lua_tonumber(L,1));
		int result = test_native_function(arg);
		lua_pushnumber(L,result);
		return 1;
	}
	void call_native_function(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_openlibs(s);
		lua_pushcclosure(s, static_native_function_binding, 0);  /* closure with those upvalues */
		lua_setglobal(s,"nativefun");

		luaL_dostring(s,
			"local times = 1000000\n"
			"for i=1,times do\n"
			"local r = nativefun(i)\n"
			"if(r ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			);
		lua_close(s);
	}
	void call_lua_function(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_openlibs(s);
		luaL_dostring(s,"lua_function=function(i)return i;end");
		lua_getglobal(s, "lua_function");
		int funref = luaL_ref(s, LUA_REGISTRYINDEX);
		for (int i = 0; i < 1000000; i++)
		{
			lua_rawgeti(s, LUA_REGISTRYINDEX, funref);
			lua_pushnumber(s, i);
			lua_call(s, 1, 1);
			int r = static_cast<int>(lua_tonumber(s, -1));
			if (r != i) { throw std::logic_error(""); }
			lua_pop(s, 1);
		}
		lua_close(s);
	}
	void lua_table_access(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_openlibs(s);
		luaL_dostring(s, "lua_table={value=0}");
		for (int i = 0; i < 1000000; i++)
		{
			lua_getglobal(s, "lua_table");
			lua_pushnumber(s, i);
			lua_setfield(s, -2, "value");
			lua_settop(s, 0);

			lua_getglobal(s, "lua_table");
			lua_getfield(s, -1, "value");
			int v = static_cast<int>(lua_tonumber(s, -1));
			if (v != i) { throw std::logic_error(""); }
			lua_settop(s, 0);
		}
		lua_close(s);
	}
	void lua_allocation(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_dostring(s, "lua_table = { } "
			"local times = 100000\n"
			"for i=1,times do\n"
			"lua_table['key'..i] =i\n"
			"end\n"
			"");
		lua_close(s);
	}
}