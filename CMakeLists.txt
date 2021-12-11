cmake_minimum_required(VERSION 3.21)
project(Tarea_SO2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Tarea_SO2 main.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
