cmake_minimum_required(VERSION 3.11)
project(ThirdTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(rollerlib
    src/find_mount.cpp
    src/find_const.cpp
)

target_include_directories(rollerlib PUBLIC src)

add_executable(roller
    src/main.cpp
)
target_link_libraries(roller PUBLIC rollerlib)

enable_testing()

add_executable(run_tests 
    test/test_find_const.cpp
    test/test_find_mount.cpp
)

target_link_libraries(run_tests
    PUBLIC
    rollerlib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)