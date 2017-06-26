
if(LUA_BUILD)
add_custom_target(copy_lua_shared_lib
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LUA_SHARED_LIBRARIES}> ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${LUA_SHARED_LIBRARIES}
)
endif(LUA_BUILD)

add_definitions(-DKAGUYA_SUPPORT_MULTIPLE_SHARED_LIBRARY=1)

add_library(test_shared_class_reg SHARED test_shared_class_reg.cpp)
target_link_libraries(test_shared_class_reg ${LUA_SHARED_LIBRARIES})
SET_TARGET_PROPERTIES(test_shared_class_reg PROPERTIES PREFIX "")

add_library(test_shared_class_use SHARED test_shared_class_use.cpp)
target_link_libraries(test_shared_class_use ${LUA_SHARED_LIBRARIES})
SET_TARGET_PROPERTIES(test_shared_class_use PROPERTIES PREFIX "")

add_executable(test_shared_class_main test_shared_class_main.cpp)
target_link_libraries(test_shared_class_main ${LUA_SHARED_LIBRARIES})

add_dependencies(test_shared_class_main test_shared_class_reg test_shared_class_use copy_lua_shared_lib)

if(HAVE_FLAG_SANITIZE_ADDRESS)
SET_TARGET_PROPERTIES(test_shared_class_reg PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
SET_TARGET_PROPERTIES(test_shared_class_reg PROPERTIES LINK_FLAGS "-fsanitize=address")
SET_TARGET_PROPERTIES(test_shared_class_use PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
SET_TARGET_PROPERTIES(test_shared_class_use PROPERTIES LINK_FLAGS "-fsanitize=address")
SET_TARGET_PROPERTIES(test_shared_class_main PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
SET_TARGET_PROPERTIES(test_shared_class_main PROPERTIES LINK_FLAGS "-fsanitize=address")
endif(HAVE_FLAG_SANITIZE_ADDRESS)


add_test(
  NAME test_shared_class
  COMMAND $<TARGET_FILE:test_shared_class_main>
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})