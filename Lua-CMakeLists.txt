cmake_minimum_required (VERSION 2.6)
project (lua)

file(GLOB LIB_LUA_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/*.h
  src/*.c
  include/*.h)

include_directories("src/")
include_directories("include/")

list(REMOVE_ITEM LIB_LUA_SRCS src/lua.c src/luac.c)

add_library(liblua STATIC ${LIB_LUA_SRCS})
SET_TARGET_PROPERTIES(liblua PROPERTIES OUTPUT_NAME lua)

if(UNIX AND NOT EMSCRIPTEN)
add_definitions("-DLUA_USE_POSIX -DLUA_USE_DLOPEN")
target_link_libraries(liblua m dl)
endif(UNIX AND NOT EMSCRIPTEN)

#add_executable(lua src/lua.c)
#target_link_libraries(lua liblua)

#add_executable(luac src/luac.c)
#target_link_libraries(luac liblua)
