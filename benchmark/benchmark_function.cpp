#include "kaguya/kaguya.hpp"

#define KAGUYA_BENCHMARK_COUNT 1000000
#define KAGUYA_BENCHMARK_COUNT_STR "1000000"

namespace
{

	int test_native_function(int arg)
	{
		return arg;
	}
	int test_native_function2(const std::string& arg)
	{
		return std::atoi(arg.c_str());
	}
	class SetGet
	{
	public:
		SetGet() :a(0.0),_i(0.0){}
		void set(double i)
		{
			_i = i;
		}
		void setstr(const std::string& str)
		{
			_i = std::atof(str.c_str());
		}
		double get()const
		{
			return _i;
		}
		double a;
	private:
		double _i;
	};

	struct Vector3
	{
		Vector3() :x(0), y(0), z(0) {}
		Vector3(float ax, float ay, float az) :x(ax), y(ay), z(az) {}
		float x, y, z;
	};
	struct Vector3Conv : Vector3
	{
		Vector3Conv() :Vector3() {}
		Vector3Conv(const Vector3& src) :Vector3(src) {}
	};

	struct BaseA
	{
		int a;
	};
	struct BaseB
	{
		int b;
	};

	struct ObjGetSet:BaseA,BaseB
	{
		ObjGetSet() :position() {}
		void set(const Vector3& p)
		{
			position = p;
		}
		const Vector3& get()const
		{
			return position;
		}

		void setconv(const Vector3Conv& p)
		{
			position = p;
		}
		Vector3Conv getconv()const
		{
			return position;
		}
		Vector3 position;
		Vector3Conv positionc;
	};
}
namespace kaguya
{
	template<>struct lua_type_traits<Vector3Conv>
	{
		typedef Vector3Conv get_type;
		typedef const Vector3Conv& push_type;
		static bool checkType(lua_State* l, int index)
		{
			LuaStackRef t(l, index);
			optional<float> x = t.getField<optional<float> >("x");
			optional<float> y = t.getField<optional<float> >("y");
			optional<float> z = t.getField<optional<float> >("z");
			return x && y && z;
		}
		static bool strictCheckType(lua_State* l, int index)
		{
			LuaStackRef t(l, index);
			optional<float> x = t.getField<optional<float> >("x");
			optional<float> y = t.getField<optional<float> >("y");
			optional<float> z = t.getField<optional<float> >("z");
			return x && y && z;
		}

		static get_type get(lua_State* l, int index)
		{
			LuaStackRef t(l, index);
			Vector3Conv data;
			data.x = t.getField<float>("x");
			data.y = t.getField<float>("y");
			data.z = t.getField<float>("z");
			return data;
		}

		static int push(lua_State* l, push_type v)
		{
			lua_createtable(l, 0, 3);
			LuaStackRef table(l, -1);
			table.setRawField("x", v.x);
			table.setRawField("y", v.y);
			table.setRawField("z", v.z);
			return 1;
		}
	};
}

namespace kaguyaapi
{
	void simple_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addFunction("set", &SetGet::set)
			.addFunction("get", &SetGet::get)
		);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}

	void simple_get_set_raw_ptr(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addFunction("set", &SetGet::set)
			.addFunction("get", &SetGet::get)
		);

		SetGet getset;
		state["getset"] = &getset;
		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
		);
	}

	void simple_get_set_shared_ptr(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addFunction("set", &SetGet::set)
			.addFunction("get", &SetGet::get)
		);
		state["getset"] = kaguya::standard::shared_ptr<SetGet>(new SetGet());

		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
#if KAGUYA_USE_CPP11
	void simple_get_set_unique_ptr(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addFunction("set", &SetGet::set)
			.addFunction("get", &SetGet::get)
		);
		state["getset"] = std::unique_ptr<SetGet>(new SetGet());

		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
#endif


	kaguya::UserdataMetatable<Vector3> vec3meta = kaguya::UserdataMetatable<Vector3>()
		.setConstructors<Vector3(), Vector3(float, float, float)>()
		.addProperty("x", &Vector3::x)
		.addProperty("y", &Vector3::y)
		.addProperty("z", &Vector3::z);

	void call_construct(kaguya::State& state)
	{
		state["Vector3"].setClass(vec3meta);
		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"local a = Vector3(i,i+1,i+2)\n"
			"if(a.x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}
	void new_construct(kaguya::State& state)
	{
		state["Vector3"].setClass(vec3meta);
		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"local a = Vector3.new(i,i+1,i+2)\n"
			"if(a.x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}

	void object_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet>()
			.setConstructors<ObjGetSet()>()
			.addFunction("set", &ObjGetSet::set)
			.addFunction("get", &ObjGetSet::get)
		);
		state["Vector3"].setClass(vec3meta);

		state(
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(Vector3.new(i,i+1,i+2))\n"
			"if(getset:get().x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}
	void object_get_set_property(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet>()
			.setConstructors<ObjGetSet()>()
			.addProperty("position", &ObjGetSet::position)
		);
		state["Vector3"].setClass(vec3meta);
		state(
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset.position=Vector3.new(i,i+1,i+2)\n"
			"if(getset.position.x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}
	void object_get_set_property_function(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet>()
			.setConstructors<ObjGetSet()>()
			.addProperty("position", &ObjGetSet::get, &ObjGetSet::set)
		);
		state["Vector3"].setClass(vec3meta);
		state(
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset.position=Vector3.new(i,i+1,i+2)\n"
			"if(getset.position.x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}

	void object_to_table_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet>()
			.setConstructors<ObjGetSet()>()
			.addFunction("get", &ObjGetSet::getconv)
			.addFunction("set", &ObjGetSet::setconv)
		);
		state["Vector3"].setClass(vec3meta);
		state(
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set({x=i,y=i+1,z=i+2})\n"
			"if(getset:get().x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}
	void object_to_table_property(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet>()
			.setConstructors<ObjGetSet()>()
			.addProperty("position", &ObjGetSet::positionc)
		);
		state["Vector3"].setClass(vec3meta);
		state(
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset.position={x=i,y=i+1,z=i+2}\n"
			"if(getset.position.x ~= i)then\n"
			"error('error')\n"
			"end\n"
			"if (collectgarbage('count') > 10240) then collectgarbage() end\n"
			"end\n"
			"");
	}

	void multiple_inheritance_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<BaseA>()
			.addFunction("setA", &BaseA::a)
			.addFunction("getA", &BaseA::a)
		);
		state["SetGet"].setClass(kaguya::UserdataMetatable<BaseB>()
			.addFunction("set", &BaseB::b)
			.addFunction("get", &BaseB::b)
		);
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet,kaguya::MultipleBase<BaseA, BaseB> >()
			.setConstructors<ObjGetSet()>()
		);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void multiple_inheritance_property(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<BaseA>()
			.addProperty("a", &BaseA::a)
		);
		state["SetGet"].setClass(kaguya::UserdataMetatable<BaseB>()
			.addFunction("set", &BaseB::b)
			.addFunction("get", &BaseB::b)
		);
		state["SetGet"].setClass(kaguya::UserdataMetatable<ObjGetSet, kaguya::MultipleBase<BaseA, BaseB> >()
			.setConstructors<ObjGetSet()>()
		);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset.a=i\n"
			"if(getset.a ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}

	void constructor_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
		);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void overloaded_get_set(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addOverloadedFunctions("set", &SetGet::setstr, &SetGet::setstr, &SetGet::setstr, &SetGet::setstr, &SetGet::setstr, &SetGet::set)
			.addFunction("get", &SetGet::get)
		);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void simple_get_set_contain_property_member(kaguya::State& state)
	{
		state["SetGet"].setClass(kaguya::UserdataMetatable<SetGet>()
			.setConstructors<SetGet()>()
			.addFunction("set", &SetGet::set)
			.addFunction("get", &SetGet::get)
			.addProperty("a", &SetGet::a)
			);

		state(
			"local getset = SetGet.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"getset:set(i)\n"
			"if(getset:get() ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			"");
	}
	void call_native_function(kaguya::State& state)
	{
		state["nativefun"] = &test_native_function;
		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"local r = nativefun(i)\n"
			"if(r ~= i)then\n"
			"error('error')\n"
			"end\n"
			"end\n"
			);
	}

	void call_overloaded_function(kaguya::State& state)
	{
		state["nativefun"] = kaguya::overload(&test_native_function2,&test_native_function);
		state(
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
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
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			int r = lua_function.call<int>(i);
			if (r != i) { throw std::logic_error(""); }
		}
	}
	void call_lua_function_operator_functional(kaguya::State& state)
	{
		state("lua_function=function(i)return i;end");

		kaguya::LuaFunction lua_function = state["lua_function"];
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			int r = lua_function(i);
			if (r != i) { throw std::logic_error(""); }
		}
	}
	
	void lua_table_access(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
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
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
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
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
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
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			lua_table["value"] = i;
			int v = lua_table.getField<int>("value");
			if (v != i) { throw std::logic_error(""); }
		}
	}

	void lua_table_bracket_const_operator_get(kaguya::State& state)
	{
		state("lua_table={value=0}");
		kaguya::LuaTable lua_table = state["lua_table"];
		const kaguya::LuaTable& const_lua_table = lua_table;
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			lua_table["value"] = i;
			int v = const_lua_table.getField("value");
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
		state["Prop"].setClass(kaguya::UserdataMetatable<Prop>()
			.setConstructors<Prop()>()
			.addProperty("d", &Prop::d)
			);

		state(
			"local getset = Prop.new()\n"
			//"getset={set = function(self,v) self.i = v end,get=function(self) return self.i end}\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
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
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"lua_table['key'..i] =i\n"
			"end\n"
			"");
	}


	void table_to_vector(kaguya::State& state)
	{
		state("lua_table={4,2,3,4,4,23,32,34,23,34,4,245,235,432,6,7,76,37,64,5}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			std::vector<double> r = lua_table.values < double > ();
		}
	}
	void table_to_vector_with_typecheck(kaguya::State& state)
	{
		state("lua_table={4,2,3,4,4,23,32,34,23,34,4,245,235,432,6,7,76,37,64,5}");
		kaguya::LuaTable lua_table = state["lua_table"];
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			std::vector<double> r = lua_table.values<double>();
		}
	}
	void vector_to_table(kaguya::State& state)
	{
		std::vector<double> v;
		for (double d = 0; d < 20; d+=1)
		{
			v.push_back(d);
		}
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			state.pushToStack(v);
			state.popFromStack();
		}
	}
}


namespace plain_api
{

	void setmetatable(lua_State *L, const char *tname) {
		luaL_getmetatable(L, tname);
		lua_setmetatable(L, -2);
	}
	void setfuncs(lua_State *L, const luaL_Reg *l) {
		for (; l->name != 0; l++) { 
			lua_pushcclosure(L, l->func, 0); 
			lua_setfield(L, -2, l->name);
		}
	}

	int setget_new(lua_State* L)
	{
		void* ptr = lua_newuserdata(L, sizeof(SetGet));
		new(ptr) SetGet();
		setmetatable(L, "SetGet");
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
		setfuncs(s, funcs);

		lua_newtable(s);

		luaL_Reg indexfuncs[] =
		{
			{ "set",setget_set },
			{ "get",setget_get },
			{ 0 ,0 },
		};
		setfuncs(s, indexfuncs);
		lua_setfield(s,-2,"__index");


		lua_setglobal(s, "SetGet");

		luaL_dostring(s,
			"local getset = SetGet.new()\n"
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
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
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
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
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			lua_rawgeti(s, LUA_REGISTRYINDEX, funref);
			lua_pushnumber(s, i);
			lua_pcall(s, 1, 1,0);
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

		lua_getglobal(s, "lua_table");
		int table_ref = luaL_ref(s, LUA_REGISTRYINDEX);//get lua_table reference
		for (int i = 0; i < KAGUYA_BENCHMARK_COUNT; i++)
		{
			lua_rawgeti(s, LUA_REGISTRYINDEX, table_ref);
			lua_pushnumber(s, i);
			lua_setfield(s, -2, "value");
			lua_settop(s,0);

			lua_rawgeti(s, LUA_REGISTRYINDEX, table_ref);
			lua_getfield(s, -1, "value");
			int v = static_cast<int>(lua_tonumber(s, -1));
			if (v != i) { throw std::logic_error(""); }
			lua_settop(s, 0);
		}
		luaL_unref(s, LUA_REGISTRYINDEX, table_ref);
		lua_close(s);
	}
	void lua_allocation(kaguya::State& )
	{
		lua_State* s = luaL_newstate();
		luaL_dostring(s, "lua_table = { } "
			"local times = " KAGUYA_BENCHMARK_COUNT_STR "\n"
			"for i=1,times do\n"
			"lua_table['key'..i] =i\n"
			"end\n"
			"");
		lua_close(s);
	}
	void object_get_set()
	{
	}
}