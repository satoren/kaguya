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

#check for address sanitizer support
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.8)
	set(HAVE_FLAG_SANITIZE_ADDRESS TRUE)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.2)
	set(HAVE_FLAG_SANITIZE_ADDRESS TRUE)
    endif()
endif()
if(HAVE_FLAG_SANITIZE_ADDRESS)
message(STATUS "address sanitizer enabled.")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif(HAVE_FLAG_SANITIZE_ADDRESS)


set(TEST_SRCS test/test.cpp
		test/test_util.hpp
		test/test_01_primitive.cpp
		test/test_02_classreg.cpp
		test/test_03_function.cpp
		test/test_04_lua_function.cpp
		test/test_05_lua_ref.cpp
		test/test_06_state.cpp
		test/test_07_vector_map_to_luatable.cpp
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

if(COVERAGE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage -coverage")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W -fprofile-arcs -ftest-coverage -coverage")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -coverage")
 add_executable(test_coverage ${TEST_SRCS})
target_link_libraries(test_coverage ${LUA_LIBRARIES} gcov)
add_test(NAME kaguya_test_coverage COMMAND test_coverage)
endif(COVERAGE)
