cmake_minimum_required(VERSION 3.20)

project(TradingEngine
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trading_engine
    src/main.cpp
    src/OrderBook.cpp
)

target_include_directories(trading_engine
    PRIVATE include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(trading_engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()