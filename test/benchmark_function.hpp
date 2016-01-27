#pragma once
namespace kaguya_benchmark
{

	void simple_get_set(kaguya::State& state);
	void object_pointer_register_get_set(kaguya::State& state);


	void call_lua_function(kaguya::State& state);
	void lua_table_access(kaguya::State& state);
	void lua_table_ref_access(kaguya::State& state);

	void property_access(kaguya::State& state);
}

namespace original_api
{
	void lua_table_access(kaguya::State& state);
}