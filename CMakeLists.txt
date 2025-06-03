cmake_minimum_required(VERSION 3.10)

project(TUNIT 
    VERSION 0.4.0
    LANGUAGES CXX
    DESCRIPTION "A C++ testing framework"
)

# Project Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

# Compilers
if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ compiler" FORCE)
    message(WARNING "CMAKE_CXX_COMPILER was not set. Falling back to g++.")
endif()
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "gcc" CACHE STRING "C compiler" FORCE)
    message(WARNING "CMAKE_C_COMPILER was not set. Falling back to gcc.")
endif()
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")


# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Compiler flags 
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DNDEBUG -DTUNIT_MODE=0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG -DTUNIT_MODE=1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DNDEBUG -DTUNIT_MODE=0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_definitions(-DNDEBUG -DTUNIT_MODE=1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG")
endif()

# Library Sources
set(TUNIT_LIB_SOURCES
    src/orchestrator/orchestrator.cpp
    src/orchestrator/suite.cpp
    src/orchestrator/test.cpp
    src/utils/trace_support.cpp
)

# Static Library Target
add_library(tunit STATIC ${TUNIT_LIB_SOURCES})

target_include_directories(tunit PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Main Executable (Demonstrates the Evaluator class)
add_executable(Evaluator main.cpp) 
target_link_libraries(Evaluator PRIVATE tunit)

# Test Configuration
enable_testing()

# Test Sources 
add_executable(tUnitTests 
    tests/main.cpp
    tests/common_predicates_test.cpp
    tests/complex_predicates_test.cpp
    tests/container_predicates_test.cpp
    tests/evaluator_core_test.cpp
    tests/integration_test.cpp
    tests/logical_predicates_test.cpp
    tests/syntax_demo_test.cpp
    tests/exception_tracing_test.cpp
)
target_link_libraries(tUnitTests PRIVATE tunit)
target_include_directories(tUnitTests PRIVATE include)

# Register tests with CTest
add_test(NAME tUnitTests COMMAND tUnitTests)

