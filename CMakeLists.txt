cmake_minimum_required(VERSION 2.8)
project(Kaguya)

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  set(LUA_SEARCHVERS lua5.3 lua5.2 luajit lua5.1)
  foreach(modulename ${LUA_SEARCHVERS})
    pkg_search_module(LUA ${modulename})
  endforeach(modulename)

  if(NOT LUA_INCLUDE_DIRS)
    message(SEND_ERROR "Can't find lua library")   
  endif()
endif(NOT LUA_INCLUDE_DIRS)

if(NOT EXISTS ${LUA_INCLUDE_DIRS}/lua.h)
  message(SEND_ERROR "Can't find lua.h in ${LUA_INCLUDE_DIRS}")
endif()

include_directories(${LUA_INCLUDE_DIRS})
include_directories("include")

file(GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/kaguya/*.hpp)

 
file(GLOB testSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  test/*.h)

include_directories("./")

if(MSVC)
add_definitions("-DKAGUYA_USE_BOOST=0")
else(MSVC)
#-Wall nonsense on MSVC
add_definitions("-Wall")
#add_definitions("-std=c++11")
endif(MSVC)

if (CMAKE_COMPILER_IS_GNUCC)
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)     
add_definitions("-std=c++11")
endif()
endif()


link_directories(${LUA_LIBRARY_DIRS})
add_executable(test_runner test/test.cpp ${testSources} ${headers})
target_link_libraries(test_runner ${LUA_LIBRARIES})
