
function(add_target_copy_to_bin_dir target_name)
  set (copynum ${ARGN})
  list(GET copynum 0 copy_files)
  foreach(v IN LISTS copy_files)
    add_custom_command(
      TARGET ${target_name} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${v} $<TARGET_FILE_DIR:${target_name}>/${v})
  endforeach()
endfunction(add_target_copy_to_bin_dir)


add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world ${LUA_LIBRARIES})

set(EXAMPLE_BINARIES ${EXAMPLE_BINARIES} hello_world)

if(NOT EMSCRIPTEN)

  add_executable(lua_exec lua_exec.cpp)
  target_link_libraries(lua_exec ${LUA_LIBRARIES})

  add_library(hello_lua_module SHARED hello_lua_module.cpp)
  target_link_libraries(hello_lua_module ${LUA_LIBRARIES})
  add_dependencies(hello_lua_module lua_exec)
  add_target_copy_to_bin_dir(hello_lua_module hello_lua_module_exec.lua)
  add_target_copy_to_bin_dir(hello_lua_module hello_lua_module.sh)
  add_target_copy_to_bin_dir(hello_lua_module hello_lua_module.bat)

  add_library(hello_lua_module_experimental SHARED hello_lua_module_experimental.cpp)
  target_link_libraries(hello_lua_module_experimental ${LUA_LIBRARIES})
  add_dependencies(hello_lua_module_experimental lua_exec)
  add_target_copy_to_bin_dir(hello_lua_module_experimental hello_lua_module_experimental_exec.lua)
  add_target_copy_to_bin_dir(hello_lua_module_experimental hello_lua_module_experimental.sh)
  add_target_copy_to_bin_dir(hello_lua_module_experimental hello_lua_module_experimental.bat)
  set(EXAMPLE_BINARIES ${EXAMPLE_BINARIES} hello_lua_module hello_lua_module_experimental lua_exec)
endif(NOT EMSCRIPTEN)

set_target_properties(${EXAMPLE_BINARIES} PROPERTIES FOLDER "Examples")
set_target_properties(${EXAMPLE_BINARIES} PROPERTIES PREFIX "")
