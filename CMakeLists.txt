cmake_minimum_required(VERSION 4.2)
project(Knightfall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chess src/main.cpp
        src/search.cpp
        src/search.h
        src/types.h
        src/board.cpp
        src/board.h
        src/move.cpp
        src/move.h
)
