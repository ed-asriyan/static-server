# HTTP Static Server [![Build Status](https://travis-ci.org/ed-asriyan/static-server.svg?branch=master)](https://travis-ci.org/ed-asriyan/static-server)

HTTP Static server on `boost::asio` and coroutines.

## Requirements
* [Make](https://www.gnu.org/software/make/) tool
* [C++14](https://isocpp.org) compiler
* [CMake 3.9](https://cmake.org) or higher
* [boost libraries](http://www.boost.org) 1.65.0 or higher

## Usage
### Build using cmake
1. Clone and build the server
    ```bash
    git clone https://github.com/ed-asriyan/static-server
    cd static-server
    cmake CMakeLists.txt
    make
    ```
2. Run the server
    ```bash
    ./static_server <PORT> <ROOT_DIRECTORY>
    ```
3. Then stop it by Ctrl-C

### Run using Docker
1. Create an image
    ```bash
    git clone https://github.com/ed-asriyan/static-server
    cd static-server
    docker build -t static_server .
    ```
3. Create a container
    ```bash
    docker run -d --name static_server -p <PORT>:80 -v <ROOT_DIRECTORY>:/static static_server
    ```
3. Run the server
    ```bash
    docker start static_server
    ```
4. Then stop it by the folowing command
    ```bash
    docker stop static_server
    ```
