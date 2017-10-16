cmake_minimum_required(VERSION 3.5)
project(static_server)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(server)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(static_server ${SOURCE_FILES})
target_link_libraries(static_server server Threads::Threads)
