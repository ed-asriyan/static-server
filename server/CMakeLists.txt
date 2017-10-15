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
        Request.hpp
        RequestParser.cpp RequestParser.hpp
        Response.cpp Response.hpp
        FileHandler.cpp FileHandler.hpp
        mime_types.cpp mime_types.hpp
        )

add_library(server STATIC ${SOURCE_FILES})
target_link_libraries(server m ${Boost_LIBRARIES})
