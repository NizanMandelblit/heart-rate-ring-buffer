cmake_minimum_required(VERSION 3.10)
project(Heart-Rate-Monitor)

# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

#include the CTest CMake module
include (CTest)

# Set CTest temporary output directory under build/test
set_property(GLOBAL PROPERTY CTEST_BINARY_DIRECTORY "${CMAKE_BINARY_DIR}/test")

# Add Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add ring_buffer_test executable and link GoogleTest libraries
add_executable(ring_buffer_test ring_buffer_test.cpp ../src/ring_buffer.c)
target_link_libraries(ring_buffer_test gtest gtest_main)


# Register the tests with CTest
add_test(NAME ring_buffer_test COMMAND ring_buffer_test)
