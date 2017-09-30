cmake_minimum_required(VERSION 3.9)
project(static_server)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(static_server ${SOURCE_FILES})
