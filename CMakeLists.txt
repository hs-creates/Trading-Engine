
cmake_minimum_required(VERSION 3.20)

project(TradingEngine VERSION 1.0 LANGUAGES CXX)

# Use C++20 throughout the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Trading engine library
add_library(trading_engine_lib
    src/OrderBook.cpp
)

target_include_directories(trading_engine_lib
    PUBLIC include
)

# Main application
add_executable(trading_engine
    src/main.cpp
)

target_link_libraries(trading_engine
    PRIVATE trading_engine_lib
)

# Download GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Test executable
add_executable(order_book_tests
    tests/test_order_book.cpp
)

target_link_libraries(order_book_tests
    PRIVATE
        trading_engine_lib
        GTest::gtest_main
)

# Automatically discover GoogleTest test cases
include(GoogleTest)
gtest_discover_tests(order_book_tests)
