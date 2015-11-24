# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (samplesdlbind)

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  pkg_search_module(LUA lua5.3)
  if(NOT LUA_INCLUDE_DIRS)
  pkg_search_module(LUA REQUIRED lua5.2)
  endif()
endif(NOT LUA_INCLUDE_DIRS)

if(NOT SDL_INCLUDE_DIRS)
  find_package(PkgConfig)
  pkg_search_module(SDL REQUIRED sdl2)
endif(NOT SDL_INCLUDE_DIRS)

if(MSVC)
add_definitions("-DKAGUYA_USE_BOOST=0")
endif(MSVC)

include_directories(${LUA_INCLUDE_DIRS})
include_directories(${SDL_INCLUDE_DIRS})
include_directories("../../include")#include kaguya dir

add_library(samplesdlbind SHARED sdlbind.cpp)
target_link_libraries(samplesdlbind ${LUA_LIBRARIES})
target_link_libraries(samplesdlbind ${SDL_LIBRARIES})
