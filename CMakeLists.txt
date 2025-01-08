cmake_minimum_required(VERSION 3.29)
project(Praktikum5 C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c89 -Werror -pedantic -Wextra -pedantic-errors -Wconversion -Walloc-zero -Wcast-qual -Wfloat-equal")

add_executable(Praktikum5 src/praktikum5Test.c
        src/matrix.c
        src/vector.c
        src/matrix.h
        src/vector.h)
