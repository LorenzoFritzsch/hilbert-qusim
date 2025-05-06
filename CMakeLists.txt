cmake_minimum_required(VERSION 3.30)
project(hilbert)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/core ${PROJECT_SOURCE_DIR}/src/engine)

add_library(hilbert STATIC
        src/hilbert_namespace.h
        src/core/lazy_matrix.cpp
        src/core/lazy_matrix.h
        src/core/lazy_matrix_operation.h
        src/core/lazy_matrix_operation_cast.cpp
        src/core/lazy_matrix_operation_cast.h
        src/core/lazy_matrix_operation_lazy_scalar_product.cpp
        src/core/lazy_matrix_operation_lazy_scalar_product.h
        src/core/lazy_matrix_operation_lazy_tensor_product.cpp
        src/core/lazy_matrix_operation_lazy_tensor_product.h
        src/core/lazy_matrix_operation_scalar_product.cpp
        src/core/lazy_matrix_operation_scalar_product.h
        src/core/lazy_matrix_operation_tensor_product.cpp
        src/core/lazy_matrix_operation_tensor_product.h
        src/core/lazy_vector.cpp
        src/core/lazy_vector.h
        src/core/lazy_vector_operation.h
        src/core/lazy_vector_operation_cast.cpp
        src/core/lazy_vector_operation_cast.h
        src/core/lazy_vector_operation_lazy_matrix_vector_product.cpp
        src/core/lazy_vector_operation_lazy_matrix_vector_product.h
        src/core/lazy_vector_operation_lazy_scalar_product.cpp
        src/core/lazy_vector_operation_lazy_scalar_product.h
        src/core/lazy_vector_operation_lazy_tensor_product.cpp
        src/core/lazy_vector_operation_lazy_tensor_product.h
        src/core/lazy_vector_operation_matrix_vector_product.cpp
        src/core/lazy_vector_operation_matrix_vector_product.h
        src/core/lazy_vector_operation_scalar_product.cpp
        src/core/lazy_vector_operation_scalar_product.h
        src/core/lazy_vector_operation_tensor_product.cpp
        src/core/lazy_vector_operation_tensor_product.h
        src/engine/algebra_engine.cpp
        src/engine/algebra_engine.h
        src/engine/gate_engine.cpp
        src/engine/gate_engine.h
        src/core/lazy_matrix_operation_lazy_outer_product.cpp
        src/core/lazy_matrix_operation_lazy_outer_product.h)

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
