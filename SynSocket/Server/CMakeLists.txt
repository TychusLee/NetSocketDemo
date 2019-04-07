cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server main.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h)