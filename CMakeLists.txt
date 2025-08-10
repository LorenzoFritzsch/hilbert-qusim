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
  # Check the processor architecture
  execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  target_link_libraries(hilbert PUBLIC "$<LINK_LIBRARY:FRAMEWORK,Accelerate>")
  if(ARCHITECTURE STREQUAL "arm64")
    message(STATUS "Configuring for Apple Silicon (arm64)")
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)
    # Apple Silicon specific settings
  elseif(ARCHITECTURE STREQUAL "x86_64")
    message(STATUS "Configuring for Intel macOS (x86_64)")
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "" FORCE)
    # Intel macOS specific settings
  endif()
else()
  # Non-Apple systems
  message(STATUS "Configuring for non-Apple or Intel PC")
  set(CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)
  # Intel PC specific settings (if any)
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
