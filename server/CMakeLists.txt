cmake_minimum_required(VERSION 3.22)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp
        utils/utils.cpp
        utils/utils.h
        server.cpp
        server.h)
