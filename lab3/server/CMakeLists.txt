cmake_minimum_required(VERSION 3.26)
project(server)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)


add_executable(server server.cpp)
