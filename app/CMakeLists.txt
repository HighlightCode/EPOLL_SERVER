cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/EpollLib/include)


add_executable(echo_server ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(echo_server PUBLIC ${CMAKE_SOURCE_DIR}/EpollLib/include)

target_link_libraries(echo_server PUBLIC EPOLLCXX)