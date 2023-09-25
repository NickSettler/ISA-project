cmake_minimum_required(VERSION 3.16)

project(test CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
        test
        src/args.c
        src/error.c
        src/req.c
        src/res.c
        tests/unit/test.h
        tests/unit/test.cpp
        tests/unit/test_config.cpp
        tests/unit/test_request.cpp
        tests/unit/test_response.cpp
)

target_link_libraries(
        test
        GTest::gtest_main
)

project(dns C)

set(CMAKE_C_STANDARD 99)

add_executable(
        dns
        src/types.h
        src/error.h
        src/error.c
        src/main.c
        src/args.c
        src/req.c
        src/res.c
)
