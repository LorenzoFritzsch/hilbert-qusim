# Copyright 2025 Lorenzo Fritzsch
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.30)
project(hilbert)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/core ${PROJECT_SOURCE_DIR}/src/engine)

add_library(hilbert STATIC
        src/hilbert_namespace.h
        src/engine/algebra_engine.cpp
        src/engine/algebra_engine.h
        src/engine/gate_engine.cpp
        src/engine/gate_engine.h
        src/engine/circuit_engine.h
        src/engine/circuit_engine.cpp
        src/core/qubit.h
        src/core/lazy_operation.h
        src/core/lazy_operation.cpp
        src/core/complex_vectorised_matrix.h
        src/core/operation.h
        src/core/complex_vector_split.h)

if(APPLE)
  message(STATUS "Configuring to run with Apple Accelerate")
  target_link_libraries(hilbert PUBLIC "$<LINK_LIBRARY:FRAMEWORK,Accelerate>")
else()
  message(STATUS "Configuring to run with AVX/AVX2 extensions")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiling with Clang/GCC")
    target_compile_options(hilbert PRIVATE -mavx2 -mfma)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") 
    message(STATUS "Compiling with MSVC")
    target_compile_options(hilbert PRIVATE /arch:AVX2)
  endif()
endif()

set_target_properties(hilbert PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
                                         ${CMAKE_BINARY_DIR}/lib)

# Testing
enable_testing()
set(CMAKE_BUILD_TYPE Debug)
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

add_executable(algebra_engine_test "${TEST_DIR}/algebra_engine_test.cpp")
target_link_libraries(algebra_engine_test hilbert)
add_test(NAME "algebra_engine_test" COMMAND algebra_engine_test)

add_executable(vectorised_matrix_test "${TEST_DIR}/complex_vectorised_matrix_test.cpp")
target_link_libraries(vectorised_matrix_test hilbert)
add_test(NAME "vectorised_matrix_test" COMMAND vectorised_matrix_test)

add_executable(qubit_test "${TEST_DIR}/qubit_test.cpp")
target_link_libraries(qubit_test hilbert)
add_test(NAME "qubit_test" COMMAND qubit_test)

add_executable(gate_engine_test "${TEST_DIR}/gate_engine_test.cpp")
target_link_libraries(gate_engine_test hilbert)
add_test(NAME "gate_engine_test" COMMAND gate_engine_test)

add_executable(circuit_engine_test "${TEST_DIR}/circuit_engine_test.cpp")
target_link_libraries(circuit_engine_test hilbert)
add_test(NAME "circuit_engine_test" COMMAND circuit_engine_test)
