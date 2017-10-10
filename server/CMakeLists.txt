cmake_minimum_required(VERSION 3.9)
project(server)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system filesystem coroutine REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES
        Server.cpp Server.hpp
        Header.hpp
        )

add_library(server STATIC ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
