cmake_minimum_required(VERSION 3.30)
project(hilbert)

set(CMAKE_CXX_STANDARD 26)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE all_srcs "${PROJECT_SOURCE_DIR}/src/*.h"
     "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(hilbert STATIC ${all_srcs})

set_target_properties(hilbert PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
                                         ${CMAKE_BINARY_DIR}/lib)

# Testing
enable_testing()
set(CMAKE_BUILD_TYPE Debug)
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

add_executable(tests "${TEST_DIR}/algebra_engine_test.cpp")
target_link_libraries(tests hilbert)
add_test(NAME "algebra_engine_test" COMMAND tests)
