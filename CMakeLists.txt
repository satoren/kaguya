cmake_minimum_required(VERSION 2.8)
project(Kaguya)

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  set(LUA_SEARCHVERS lua5.3 lua5.2 luajit lua5.1)
  foreach(modulename ${LUA_SEARCHVERS})
    pkg_search_module(LUA ${modulename})
  endforeach(modulename)
endif(NOT LUA_INCLUDE_DIRS)


if(NOT LUA_INCLUDE_DIRS)
  #search local directory
  set(LOCAL_LUA_DIRECTORY lua-5.3.1)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY})
    file(COPY Lua-CMakeLists.txt DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY})
    file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/Lua-CMakeLists.txt ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/CMakeLists.txt)
    add_subdirectory(${LOCAL_LUA_DIRECTORY})
   set(LUA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/src)
   set(LUA_LIBRARIES liblua)
  else()
    message(SEND_ERROR "Can't find lua library")   
  endif()
endif()

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

if(NOT MSVC)#-Wall nonsense on MSVC
add_definitions("-Wall")
#add_definitions("-std=c++11")
endif(MSVC)



link_directories(${LUA_LIBRARY_DIRS})
add_executable(test_runner test/test.cpp ${testSources} ${headers})
target_link_libraries(test_runner ${LUA_LIBRARIES})
