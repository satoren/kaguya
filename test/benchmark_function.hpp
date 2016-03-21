#pragma once
namespace kaguya_api_benchmark______
{

	void simple_get_set(kaguya::State& state);
	void simple_get_set_contain_propery_member(kaguya::State& state);
	
	void object_pointer_register_get_set(kaguya::State& state);

	void call_native_function(kaguya::State& state);

	void call_lua_function(kaguya::State& state);
	void call_lua_function_operator_functional(kaguya::State& state);
	void lua_table_access(kaguya::State& state);
	void lua_table_bracket_operator_access(kaguya::State& state);
	void lua_table_bracket_operator_assign(kaguya::State& state);
	void lua_table_bracket_operator_get(kaguya::State& state);
	void property_access(kaguya::State& state);

	void vectormapping(kaguya::State& state);

	void lua_allocation(kaguya::State& state);
}

namespace original_api_no_type_check
{
	void simple_get_set(kaguya::State& state);
	void call_native_function(kaguya::State& state);
	void call_lua_function(kaguya::State& state);
	void lua_table_access(kaguya::State& state);
	void lua_allocation(kaguya::State& state);
}