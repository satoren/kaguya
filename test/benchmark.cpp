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

		kaguya::State state; state.openlibs();

		double total_time = 0;
		double start = state["os"]["clock"]();

		it->second(state);

		double end = state["os"]["clock"]();
		total_time += end - start;

		std::cout << test_name << " time:" << total_time << std::endl;
	}
}


int main()
{
	benchmark_function_map_t functionmap;
#define ADD_BENCHMARK(function) functionmap.push_back(std::make_pair(#function,&function));
	ADD_BENCHMARK(empty);
	ADD_BENCHMARK(simple_get_set);
	ADD_BENCHMARK(object_pointer_register_get_set);
	ADD_BENCHMARK(call_lua_function);
	ADD_BENCHMARK(lua_table_access);

	

	execute_benchmark(functionmap);

}
