set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.6)
project(Kaguya)

include(cmake/FindLua.cmake)


option(KAGUYA_BUILD_EXAMPLES "Build the Kaguya example programs" ON)
option(KAGUYA_BUILD_BENCHMARK "Build the Kaguya benchmark programs" OFF)
option(KAGUYA_BUILD_EXAMPLE_GLFW_BIND "Build the Kaguya example glfw bind" OFF)
option(KAGUYA_SINGLE_HEADER_VERSION "generate single header" OFF)

include_directories(SYSTEM ${LUA_INCLUDE_DIRS})
link_directories(${LUA_LIBRARY_DIRS})


find_package(Boost)
if(Boost_INCLUDE_DIR)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
endif(Boost_INCLUDE_DIR)


if(KAGUYA_SINGLE_HEADER_VERSION)
file(MAKE_DIRECTORY single_include/kaguya)
file(COPY include/kaguya/another_binding_api.hpp DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/single_include/kaguya/)
execute_process(
  COMMAND python generate_one_header.py
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/utils OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/single_include/kaguya/kaguya.hpp)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/single_include")

else(KAGUYA_SINGLE_HEADER_VERSION)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
endif(KAGUYA_SINGLE_HEADER_VERSION)

file(GLOB KAGUYA_HEADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/kaguya/*.hpp
  include/kaguya/detail/*.hpp)

if(ADDITIONAL_INCLUDE_PATH)
  include_directories(SYSTEM ${ADDITIONAL_INCLUDE_PATH})
endif()

if(NOT MSVC)#-Wall nonsense on MSVC
  add_definitions(-Wall -W -Werror)
  add_definitions(-pedantic)
  add_definitions(-Wno-variadic-macros)
  add_definitions ("-Wno-unused-local-typedefs")
  add_definitions ("-Wno-unknown-warning-option")
  #add_definitions("-std=c++11")
endif(NOT MSVC)


if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions ("-fno-strict-overflow")
endif()

if(KAGUYA_NO_USERDATA_TYPE_CHECK)
  add_definitions("-DKAGUYA_NO_USERDATA_TYPE_CHECK=1")
endif()

if(KAGUYA_USE_SHARED_LUAREF)
  add_definitions("-DKAGUYA_USE_SHARED_LUAREF=1")
endif()

if(EMSCRIPTEN)
  include_directories(SYSTEM "${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include/")
  add_definitions("-std=c++11")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif(EMSCRIPTEN)

#check for address sanitizer support
if(NOT DISABLE_ADDRESS_SANITIZER)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 4.8)
      set(HAVE_FLAG_SANITIZE_ADDRESS TRUE)
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 3.2)
      set(HAVE_FLAG_SANITIZE_ADDRESS TRUE)
    endif()
    #detect stack-buffer-overflow at clang++3.4(on ubuntu 12.04) with luajit but don't know reason
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_EQUAL 3.4)
      if(${LUA_SEARCH_LIB_NAME} STREQUAL "luajit")
        unset(HAVE_FLAG_SANITIZE_ADDRESS)
      endif()
    endif()
endif()
if(CYGWIN OR WIN32)
      unset(HAVE_FLAG_SANITIZE_ADDRESS)
endif()
endif(NOT DISABLE_ADDRESS_SANITIZER)

if(KAGUYA_BUILD_BENCHMARK)
set(BENCHMARK_SRCS benchmark/benchmark.cpp benchmark/benchmark_function.cpp benchmark/benchmark_function.hpp)
add_executable(benchmark ${BENCHMARK_SRCS} ${KAGUYA_HEADER})
target_link_libraries(benchmark ${LUA_LIBRARIES})
endif(KAGUYA_BUILD_BENCHMARK)

if(KAGUYA_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(KAGUYA_BUILD_EXAMPLES)

if(EMSCRIPTEN)
if(KAGUYA_BUILD_BENCHMARK)
  SET_TARGET_PROPERTIES(benchmark PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s TOTAL_MEMORY=36777216")
endif(KAGUYA_BUILD_BENCHMARK)
endif(EMSCRIPTEN)


enable_testing()
add_subdirectory(test)