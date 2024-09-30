cmake_minimum_required(VERSION 3.10)

project(clouseau VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(clouseau src/main.cpp src/cli.cpp)

enable_testing()
add_executable(test_cli tests/test_cli.cpp src/cli.cpp)
add_test(NAME cli COMMAND test_cli)
