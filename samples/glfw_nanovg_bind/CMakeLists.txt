cmake_minimum_required (VERSION 2.8.11)
project (samplenanovgbind)


include(../../cmake/FindLua.cmake)

include_directories(${LUA_INCLUDE_DIRS})
link_directories(${LUA_LIBRARY_DIRS})
include_directories("../../include")#include kaguya dir


add_library(luaeasing SHARED luaeasing.cpp)
target_link_libraries(luaeasing ${LUA_LIBRARIES})
SET_TARGET_PROPERTIES(luaeasing PROPERTIES OUTPUT_NAME luaeasing)
SET_TARGET_PROPERTIES(luaeasing PROPERTIES PREFIX "")

add_subdirectory(glad)
add_subdirectory(glfw)
include_directories(glfw/include)

add_library(luaglfw SHARED luaglfw.cpp)
target_link_libraries(luaglfw ${LUA_LIBRARIES})
target_link_libraries(luaglfw glfw ${GLFW_LIBRARIES})
SET_TARGET_PROPERTIES(luaglfw PROPERTIES OUTPUT_NAME luaglfw)
SET_TARGET_PROPERTIES(luaglfw PROPERTIES PREFIX "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${GLAD_INCLUDE_DIRS})
include_directories(nanovg/src)
add_library(luananovg SHARED luananovg.cpp nanovg/src/nanovg.c)
target_link_libraries(luananovg ${LUA_LIBRARIES})
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
add_definitions("-DUSE_GLES2")
target_link_libraries(luananovg GLESv2)
else()
target_link_libraries(luananovg ${GLAD_LIBRARIES})
endif()

SET_TARGET_PROPERTIES(luananovg PROPERTIES PREFIX "")