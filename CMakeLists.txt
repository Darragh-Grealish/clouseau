
cmake_minimum_required(VERSION 3.10)
project(clouseau VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CTEST_OUTPUT_ON_FAILURE TRUE)

# NOTE: Windows requires explicit linking choice 
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# NOTE: Add Google Test submodule as a subdirectory
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(include)
add_executable(clouseau src/main.cpp src/cli.cpp src/indexer.cpp) 

enable_testing()
include(GoogleTest)

# TEST: ArrayList implementation
add_executable(test_array_list tests/test_array_list.cpp)
target_link_libraries(test_array_list gtest gtest_main)
gtest_discover_tests(test_array_list)

# TEST: CLI implementation
add_executable(test_cli tests/test_cli.cpp src/cli.cpp)
target_link_libraries(test_cli gtest gtest_main)
gtest_discover_tests(test_cli)

# TEST: HashTable implementation
add_executable(test_hash_map tests/test_hash_map.cpp)
target_link_libraries(test_hash_map gtest gtest_main)
gtest_discover(test_hash_map)

# TEST: Indexer implementation
add_executable(test_indexer tests/test_indexer.cpp src/indexer.cpp)
target_link_libraries(test_indexer gtest gtest_main)
gtest_discover_tests(test_indexer)

