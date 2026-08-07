cmake_minimum_required(VERSION 3.11)
project(ThirdTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(roller
    src/main.cpp
    src/find_const.cpp
    src/find_const.h
)