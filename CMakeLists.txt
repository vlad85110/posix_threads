cmake_minimum_required(VERSION 3.21)
project(posix_thread C)

set(CMAKE_C_STANDARD 99)

add_executable(posix_thread main.c lab1/lab1.c lab3/lab3.c lab1/lab1.h labs.h lab3/lab3.h)
