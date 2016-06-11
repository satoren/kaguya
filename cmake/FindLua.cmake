
if(LUA_SEARCH_LIB_NAME)
  find_package(PkgConfig)
  foreach(modulename ${LUA_SEARCH_LIB_NAME})
    pkg_search_module(LUA ${modulename})
  endforeach(modulename)
endif(LUA_SEARCH_LIB_NAME)


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
   if(NOT ${MSVC})
      set(LUA_LIBRARIES libm)
   endif()
  endif()
endif()

if(NOT LUA_INCLUDE_DIRS)
  find_package(PkgConfig)
  set(LUA_SEARCHVERS lua5.3 lua5.2 luajit lua5.1 lua)
  foreach(modulename ${LUA_SEARCHVERS})
    pkg_search_module(LUA ${modulename})
  endforeach(modulename)
endif(NOT LUA_INCLUDE_DIRS)

if(NOT LUA_INCLUDE_DIRS)
# message(SEND_ERROR "Can't find lua library")
endif(NOT LUA_INCLUDE_DIRS)


if(NOT EXISTS ${LUA_INCLUDE_DIRS}/lua.h)
  message(SEND_STATUS "Can't find lua.h in ${LUA_INCLUDE_DIRS}")
endif()
