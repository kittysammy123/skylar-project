cmake_minimum_required(VERSION 3.25)
project(skylar_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(skylar_project
        log.cpp
        log.h)
