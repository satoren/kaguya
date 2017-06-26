if(NOT LOCAL_LUA_DIRECTORY)
   set(LOCAL_LUA_DIRECTORY ${LUA_SEARCH_LIB_NAME})
endif()

if(LOCAL_LUA_DIRECTORY)
  #search local directory
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY})
    file(COPY Lua-CMakeLists.txt DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY})
    file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/Lua-CMakeLists.txt ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/CMakeLists.txt)
    add_subdirectory(${LOCAL_LUA_DIRECTORY})

    set(LUA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/src)
    if(NOT EXISTS ${LUA_INCLUDE_DIRS}/lua.h)
      set(LUA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_LUA_DIRECTORY}/include)
    endif()
   set(LUA_LIBRARIES liblua)
   if(NOT EMSCRIPTEN)
    set(LUA_SHARED_LIBRARIES liblua_shared)
    set(LUA_BUILD true)
   endif(NOT EMSCRIPTEN)
  endif()
endif()

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  set(LUA_SEARCHVERS ${LUA_SEARCH_LIB_NAME} lua5.3 lua5.2 luajit lua5.1 lua)
  foreach(modulename ${LUA_SEARCHVERS})
    pkg_search_module(LUA ${modulename})
  endforeach(modulename)
  # maybe system library is shared library
  set(LUA_SHARED_LIBRARIES ${LUA_LIBRARIES})
endif(NOT LUA_INCLUDE_DIRS)

if(NOT LUA_INCLUDE_DIRS)
# message(SEND_ERROR "Can't find lua library")
endif(NOT LUA_INCLUDE_DIRS)


if(NOT EXISTS ${LUA_INCLUDE_DIRS}/lua.h)
  message(SEND_ERROR "Can't find lua.h in ${LUA_INCLUDE_DIRS}")
endif()
