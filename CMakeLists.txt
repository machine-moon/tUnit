cmake_minimum_required(VERSION 3.10)

project(CppEvaluatorProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
include_directories(src)

# Main executable (excluding test files)
file(GLOB MAIN_SOURCES "src/main.cpp")
add_executable(CppEvaluator ${MAIN_SOURCES})

# Enable testing
enable_testing()

# Core Test Executables with New TestRunner API
add_executable(EvaluatorCoreTest tests/evaluator_core_test.cpp)
# Add individual test cases
add_test(NAME EvaluatorCoreTests COMMAND EvaluatorCoreTest)



