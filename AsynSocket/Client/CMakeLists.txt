cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client main.cpp Socket.cpp Socket.h)