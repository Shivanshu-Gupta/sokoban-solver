cmake_minimum_required(VERSION 3.17)
project(sokoban_solver)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(sokoban_solver
        src/coord.hpp
        src/sokoban.h
        src/sokoban.cpp
        src/main.cpp)
