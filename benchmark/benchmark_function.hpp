#pragma once
namespace kaguyaapi
{

	void simple_get_set(kaguya::State& state);

	void simple_get_set_raw_ptr(kaguya::State& state);
	void simple_get_set_shared_ptr(kaguya::State& state);
#if KAGUYA_USE_CPP11
	void simple_get_set_unique_ptr(kaguya::State& state);
#endif
	void overloaded_get_set(kaguya::State& state);
	void simple_get_set_contain_property_member(kaguya::State& state);

	void call_construct(kaguya::State& state);
	void new_construct(kaguya::State& state);

	void object_get_set(kaguya::State& state);
	void object_get_set_property(kaguya::State& state);
	void object_get_set_property_function(kaguya::State& state);
	void object_to_table_get_set(kaguya::State& state);
	void object_to_table_property(kaguya::State& state);

	void multiple_inheritance_get_set(kaguya::State& state);
	void multiple_inheritance_property(kaguya::State& state);
	
	void call_native_function(kaguya::State& state);
	void call_overloaded_function(kaguya::State& state);

	void call_lua_function(kaguya::State& state);
	void call_lua_function_operator_functional(kaguya::State& state);
	void lua_table_access(kaguya::State& state);
	void lua_table_bracket_operator_access(kaguya::State& state);
	void lua_table_bracket_operator_assign(kaguya::State& state);
	void lua_table_bracket_operator_get(kaguya::State& state);
	void lua_table_bracket_const_operator_get(kaguya::State& state);
	
	void property_access(kaguya::State& state);

	void table_to_vector(kaguya::State& state);
	void table_to_vector_with_typecheck(kaguya::State& state);
	void vector_to_table(kaguya::State& state);
	

	void lua_allocation(kaguya::State& state);
}

namespace plain_api
{
	void simple_get_set(kaguya::State& state);
	void object_get_set(kaguya::State& state);
	void call_native_function(kaguya::State& state);
	void call_lua_function(kaguya::State& state);
	void lua_table_access(kaguya::State& state);
	void lua_allocation(kaguya::State& state);
}