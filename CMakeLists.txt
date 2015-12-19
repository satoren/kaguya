cmake_minimum_required(VERSION 2.8)
project(Kaguya)

include(cmake/FindLua.cmake)

include_directories(${LUA_INCLUDE_DIRS})
link_directories(${LUA_LIBRARY_DIRS})
include_directories("include")

file(GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/kaguya/*.hpp)


file(GLOB testSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  test/*.h)

include_directories("./")

if(NOT MSVC)#-Wall nonsense on MSVC
add_definitions("-Wall")
#add_definitions("-std=c++11")
endif(NOT MSVC)

link_directories(${LUA_LIBRARY_DIRS})
add_executable(test_runner test/test.cpp ${testSources} ${headers})
target_link_libraries(test_runner ${LUA_LIBRARIES})

add_executable(benchmark test/benchmark.cpp ${headers})
target_link_libraries(benchmark ${LUA_LIBRARIES})

enable_testing()
add_test(kaguya_test test_runner)
