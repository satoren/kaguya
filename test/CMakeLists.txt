
if(COVERAGE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage -coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage -coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
endif(COVERAGE)

file(GLOB TEST_SRCS files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_*.cpp)


file(GLOB KAGUYA_HEADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ../include/kaguya/*.hpp
  ../include/kaguya/detail/*.hpp)

add_executable(test_runner ${TEST_SRCS} ${KAGUYA_HEADER})
target_link_libraries(test_runner ${LUA_LIBRARIES})
if(HAVE_FLAG_SANITIZE_ADDRESS)
SET_TARGET_PROPERTIES(test_runner PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
SET_TARGET_PROPERTIES(test_runner PROPERTIES LINK_FLAGS "-fsanitize=address")
endif(HAVE_FLAG_SANITIZE_ADDRESS)


if(EMSCRIPTEN)
  SET_TARGET_PROPERTIES(test_runner PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s TOTAL_MEMORY=36777216 -s DISABLE_EXCEPTION_CATCHING=0 --preload-file ${CMAKE_SOURCE_DIR}/test/lua@/lua")
endif(EMSCRIPTEN)


add_test(
  NAME test_runner
  COMMAND $<TARGET_FILE:test_runner>
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(LUA_SHARED_LIBRARIES)
add_subdirectory(shared_library_test)
endif(LUA_SHARED_LIBRARIES)