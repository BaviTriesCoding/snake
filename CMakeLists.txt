cmake_minimum_required(VERSION 3.27)
project(snake)

set(CMAKE_CXX_STANDARD 17)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake main.cpp
        library.cpp
        library.h)

target_link_libraries(snake ${CURSES_LIBRARIES})