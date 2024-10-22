cmake_minimum_required(VERSION 3.10)
project(clouseau VERSION 1.0)

option(ENABLE_COVERAGE "Enable coverage reporting" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CTEST_OUTPUT_ON_FAILURE TRUE)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    add_compile_options(/std:c17)
    add_definitions(-DPLATFORM_WINDOWS)
else()
    add_definitions(-DPLATFORM_UNIX)
    # Coverage flags for non-MSVC compilers
    if(ENABLE_COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
    endif()
endif()

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(include)

add_executable(clouseau 
    src/main.cpp 
    src/cli.cpp 
    src/indexer.cpp 
    src/trie.cpp
) 

enable_testing()
include(GoogleTest)

set(TEST_TARGETS "")

# TEST: ArrayList implementation
add_executable(test_array_list tests/test_array_list.cpp)
target_link_libraries(test_array_list gtest gtest_main)
gtest_discover_tests(test_array_list)
list(APPEND TEST_TARGETS test_array_list)

# TEST: CLI implementation
add_executable(test_cli 
    tests/test_cli.cpp 
    src/cli.cpp 
    src/trie.cpp 
    src/indexer.cpp
)
target_link_libraries(test_cli gtest gtest_main)
gtest_discover_tests(test_cli)
list(APPEND TEST_TARGETS test_cli)

# TEST: HashTable implementation
add_executable(test_hash_map tests/test_hash_map.cpp)
target_link_libraries(test_hash_map gtest gtest_main)
gtest_discover_tests(test_hash_map)
list(APPEND TEST_TARGETS test_hash_map)

# TEST: Set implementation
add_executable(test_set tests/test_set.cpp)  
target_link_libraries(test_set gtest gtest_main)  
gtest_discover_tests(test_set)  
list(APPEND TEST_TARGETS test_set) 

# TEST: Indexer implementation
add_executable(test_indexer 
    tests/test_indexer.cpp 
    src/indexer.cpp 
    src/trie.cpp
)
target_link_libraries(test_indexer gtest gtest_main)
gtest_discover_tests(test_indexer)
list(APPEND TEST_TARGETS test_indexer)

# TEST: Trie implementation
add_executable(test_trie src/trie.cpp tests/test_trie.cpp)
target_link_libraries(test_trie gtest gtest_main)
gtest_discover_tests(test_trie)
list(APPEND TEST_TARGETS test_trie)


if(ENABLE_COVERAGE AND NOT MSVC)
    find_program(LCOV lcov REQUIRED)
    find_program(GENHTML genhtml REQUIRED)

    add_custom_target(coverage
        COMMAND ${LCOV} --directory . --zerocounters
        
        COMMAND ctest --output-on-failure
        
        COMMAND ${LCOV} --directory . --capture --output-file coverage.info
        
        COMMAND ${LCOV} 
            --remove coverage.info 
            '/usr/*' 
            '${PROJECT_SOURCE_DIR}/tests/*'
            '${PROJECT_SOURCE_DIR}/googletest/*'
            --output-file coverage.info.cleaned
        
        COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info.cleaned
        
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        
        COMMENT "Generating code coverage report"
    )
endif()
