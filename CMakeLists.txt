cmake_minimum_required(VERSION 3.10)
project(clouseau VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# NOTE: Submodule for testing framework
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(include)
add_executable(clouseau src/main.cpp) 
enable_testing()

# TEST: ArrayList implementation
add_executable(test_array_list tests/test_array_list.cpp)
target_link_libraries(test_array_list gtest gtest_main)
add_test(NAME test_array_list COMMAND test_array_list)

# TEST: CLI implementation
add_executable(test_cli tests/test_cli.cpp)
target_link_libraries(test_cli gtest gtest_main)
add_test(NAME test_cli COMMAND test_cli)
