cmake_minimum_required(VERSION 3.30)
project(hilbert)

set(CMAKE_CXX_STANDARD 26)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/model ${PROJECT_SOURCE_DIR}/src/engine)

file(GLOB_RECURSE all_srcs "${PROJECT_SOURCE_DIR}/src/*.h"
     "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(hilbert STATIC ${all_srcs})

set_target_properties(hilbert PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
                                         ${CMAKE_BINARY_DIR}/lib)

# Testing
enable_testing()
set(CMAKE_BUILD_TYPE Debug)
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

add_executable(algebra_engine_test "${TEST_DIR}/algebra_engine_test.cpp")
target_link_libraries(algebra_engine_test hilbert)
add_test(NAME "algebra_engine_test" COMMAND algebra_engine_test)

add_executable(lazy_matrix_test "${TEST_DIR}/lazy_matrix_test.cpp")
target_link_libraries(lazy_matrix_test hilbert)
add_test(NAME "lazy_matrix_test" COMMAND lazy_matrix_test)

add_executable(lazy_vector_test "${TEST_DIR}/lazy_vector_test.cpp")
target_link_libraries(lazy_vector_test hilbert)
add_test(NAME "lazy_vector_test" COMMAND lazy_vector_test)
