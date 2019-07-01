cmake_minimum_required(VERSION 3.14)
project(BBSerial)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc  -static-libstdc++")

add_executable(FlightComputer main.cpp)