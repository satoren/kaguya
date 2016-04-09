cmake_minimum_required(VERSION 2.6)
project(Kaguya)

include(cmake/FindLua.cmake)

include_directories(${LUA_INCLUDE_DIRS})
link_directories(${LUA_LIBRARY_DIRS})
include_directories("include")

file(GLOB KAGUYA_HEADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/kaguya/*.hpp
  include/kaguya/detail/*.hpp)


include_directories("./")

if(NOT MSVC)#-Wall nonsense on MSVC
add_definitions("-Wall")
#add_definitions("-std=c++11")
endif(NOT MSVC)


set(TEST_SRCS test/test.cpp
		test/test_util.hpp
		test/test_01_primitive.cpp
		test/test_02_classreg.cpp
		test/test_03_function.cpp
		test/test_04_lua_function.cpp
		test/test_05_lua_ref.cpp
		test/test_06_state.cpp
		test/test_07_any_type_test.cpp		
		test/test_11_cxx11_feature.cpp
		)

link_directories(${LUA_LIBRARY_DIRS})
add_executable(test_runner
				${TEST_SRCS} ${KAGUYA_HEADER})
target_link_libraries(test_runner ${LUA_LIBRARIES})

set(BENCHMARK_SRCS test/benchmark.cpp test/benchmark_function.cpp test/benchmark_function.hpp)

add_executable(benchmark ${BENCHMARK_SRCS} ${KAGUYA_HEADER})
target_link_libraries(benchmark ${LUA_LIBRARIES})

enable_testing()
add_test(kaguya_test test_runner)
