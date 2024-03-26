cmake_minimum_required(VERSION 3.27)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)

add_executable(srver main.cpp
        utils/utils.cpp
        utils/utils.h
        server.cpp
        server.h)
