cmake_minimum_required(VERSION 3.21)
project(posix_thread C)

set(CMAKE_C_STANDARD 99)

add_executable(posix_thread main.c lab1/lab1.c lab3/lab3.c lab1/lab1.h labs.h lab3/lab3.h lab18/lab18.c lab18/lab18.h lab18/list.c lab18/list.h lab24/lab24.c lab24/lab24.h lab24/detail.h lab24/detail.c lab26/lab26.c lab26/lab26.h lab26/queue.c lab8/lab8.c lab8/lab8.h)
