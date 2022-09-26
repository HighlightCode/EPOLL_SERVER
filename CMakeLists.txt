cmake_minimum_required(VERSION 3.1)

project(
    epoll_server_example
    VERSION 0.1
    DESCRIPTION "EXAMPLE OF EPOLL SERVER WITH UNREAL CLIENT"
    LANGUAGES CXX)

# add library files to build/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# add build file to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(EpollLib)
add_subdirectory(app)
