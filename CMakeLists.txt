cmake_minimum_required(VERSION 3.16)
project(infinite-input-response)

enable_testing()
find_package(GTest REQUIRED)

add_executable(tests test/tests.cpp)

target_link_libraries(tests GTest::gtest_main)
target_include_directories(tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(tests)
