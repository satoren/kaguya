cmake_minimum_required(VERSION 2.8)
project(Kaguya)

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  pkg_search_module(LUA lua5.3)
  if(NOT LUA_INCLUDE_DIRS)
  pkg_search_module(LUA REQUIRED lua5.2)
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

add_executable(test_runner test/test.cpp ${testSources} ${headers})
target_link_libraries(test_runner ${LUA_LIBRARIES})
