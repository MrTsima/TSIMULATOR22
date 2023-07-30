cmake_minimum_required(VERSION 3.25)
project(TSIMULATOR)

set(CMAKE_CXX_STANDARD 17)

add_executable(TSIMULATOR main.cpp emulator.h ALU.cpp ALU.h Memory.cpp Memory.h Utils.cpp Utils.h emulator.cpp)
