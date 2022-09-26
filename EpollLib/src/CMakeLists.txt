cmake_minimum_required(VERSION 3.1)

PROJECT(
    EPOLLCXX
    VERSION 0.1
    DESCRIPTION "EPOLL SERVER LIBRARY"
    LANGUAGES CXX
    )

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )


# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify the Compile Options
add_compile_options(-Wall)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)


#print the direcories
MESSAGE(STATUS "PROJECT Name: " ${PROJECT_NAME})
MESSAGE(STATUS "CPP_FILES: " ${CPP_FILES})
MESSAGE(STATUS "Binary dir: " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "Source dir: " ${PROJECT_SOURCE_DIR})

# add target libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)

# set Library Files to Lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
