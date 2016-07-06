#include "kaguya/kaguya.hpp"

#include "benchmark_function.hpp"



typedef void(*benchmark_function_t)(kaguya::State&);
typedef std::vector<std::pair<std::string, benchmark_function_t> > benchmark_function_map_t;
void empty(kaguya::State&)
{

}

void execute_benchmark(const benchmark_function_map_t& testmap)
{
	for (benchmark_function_map_t::const_iterator it = testmap.begin(); it != testmap.end(); ++it)
	{
		const std::string& test_name = it->first;


		double mintime = std::numeric_limits<double>::max();

		kaguya::State clockstate;

		static const int N = 20;
		for (int i = 0; i < N; ++i)
		{
			double start = clockstate["os"]["clock"]();
			{
				kaguya::State state;
				it->second(state);
			}

			double end = clockstate["os"]["clock"]();
			mintime = std::min(mintime, end - start);
		}

		std::cout << test_name << "," << mintime<< std::endl;
	}
}


int main()
{
	benchmark_function_map_t functionmap;
#define ADD_BENCHMARK(function) functionmap.push_back(std::make_pair(#function,&function));
	ADD_BENCHMARK(empty);

	ADD_BENCHMARK(original_api_no_type_check::simple_get_set);
	ADD_BENCHMARK(kaguya_api_benchmark______::simple_get_set);

	ADD_BENCHMARK(kaguya_api_benchmark______::simple_get_set_raw_ptr);
	ADD_BENCHMARK(kaguya_api_benchmark______::simple_get_set_shared_ptr);
#if KAGUYA_USE_CPP11
	ADD_BENCHMARK(kaguya_api_benchmark______::simple_get_set_unique_ptr);
#endif
//	ADD_BENCHMARK(original_api_no_type_check::object_get_set);
	ADD_BENCHMARK(kaguya_api_benchmark______::object_get_set);
	ADD_BENCHMARK(kaguya_api_benchmark______::object_get_set_property);
	ADD_BENCHMARK(kaguya_api_benchmark______::object_get_set_property_function);
	ADD_BENCHMARK(kaguya_api_benchmark______::object_to_table_get_set);

	ADD_BENCHMARK(kaguya_api_benchmark______::overloaded_get_set);	
	ADD_BENCHMARK(kaguya_api_benchmark______::property_access);
	ADD_BENCHMARK(kaguya_api_benchmark______::simple_get_set_contain_property_member);
	ADD_BENCHMARK(kaguya_api_benchmark______::call_native_function);
	ADD_BENCHMARK(original_api_no_type_check::call_native_function);
	ADD_BENCHMARK(kaguya_api_benchmark______::call_lua_function);
	ADD_BENCHMARK(original_api_no_type_check::call_lua_function);
	ADD_BENCHMARK(kaguya_api_benchmark______::call_lua_function_operator_functional);
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_table_access);
	ADD_BENCHMARK(original_api_no_type_check::lua_table_access);
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_table_bracket_operator_access);
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_table_bracket_operator_assign);
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_table_bracket_operator_get);
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_table_bracket_const_operator_get);
	
	ADD_BENCHMARK(kaguya_api_benchmark______::lua_allocation);
	ADD_BENCHMARK(original_api_no_type_check::lua_allocation);

	ADD_BENCHMARK(kaguya_api_benchmark______::table_to_vector);
	ADD_BENCHMARK(kaguya_api_benchmark______::table_to_vector_with_typecheck);
	
	execute_benchmark(functionmap);

}
