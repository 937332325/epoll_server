cmake_minimum_required(VERSION 3.14)
project(webserver)

set(CMAKE_CXX_STANDARD 11)

add_executable(webserver main.cpp HttpServer.h HttpServer.cpp)