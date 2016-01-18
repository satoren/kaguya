#include "kaguya/kaguya.hpp"

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
	private:
		double _i;
	};
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

	void call_lua_function(kaguya::State& state)
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
		Prop():d(0){}

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