#include "kaguya/kaguya.hpp"

namespace
{

	int test_native_function(int arg)
	{
		return arg;
	}
}

namespace kaguya_api_benchmark______
{
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
	void simple_get_set(kaguya::State& state)
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

		kaguya::LuaRef lua_function = state["lua_function"];
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
		kaguya::LuaRef lua_table = state["lua_table"];
		for (int i = 0; i < 1000000; i++)
		{
			lua_table["value"] = i;
			int v = lua_table["value"];
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
}


namespace original_api_no_type_check
{
	int static_native_function_binding(lua_State* L)
	{
		int arg = lua_tonumber(L,1);
		int result = test_native_function(arg);
		lua_pushnumber(L,result);
		return 1;
	}
	void call_native_function(kaguya::State& state)
	{
		lua_State* s = state.state();
		lua_pushcclosure(s, static_native_function_binding, 0);  /* closure with those upvalues */
		lua_setglobal(s,"nativefun");

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
		lua_State* s = state.state();
		luaL_dostring(s,"lua_function=function(i)return i;end");
		lua_getglobal(s, "lua_function");
		int funref = luaL_ref(s, LUA_REGISTRYINDEX);
		for (int i = 0; i < 1000000; i++)
		{
			lua_rawgeti(s, LUA_REGISTRYINDEX, funref);
			lua_pushnumber(s, i);
			lua_pcall(s, 1, 1, 0);
			int r = static_cast<int>(lua_tonumber(s, -1));
			if (r != i) { throw std::logic_error(""); }
			lua_pop(s, 1);
		}
	}
	void lua_table_access(kaguya::State& state)
	{
		lua_State* s = state.state();
		luaL_dostring(s, "lua_table={value=0}");
		for (int i = 0; i < 1000000; i++)
		{
			lua_getglobal(s, "lua_table");
			lua_pushnumber(s,i);
			lua_setfield(s,-2, "value");
			lua_settop(s, 0);

			lua_getglobal(s, "lua_table");
			lua_getfield(s,-1, "value");
			int v = static_cast<int>(lua_tonumber(s,-1));
			if (v != i) { throw std::logic_error(""); }
			lua_settop(s,0);
		}
	}
}