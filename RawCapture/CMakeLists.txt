cmake_minimum_required(VERSION 3.13)
project(RawCapture)

set(CMAKE_CXX_STANDARD 14)

add_executable(RawCapture main.cpp TCPSniffer.cpp TCPSniffer.h)
