cmake_minimum_required(VERSION 2.7)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES main.c)

set(CMAKE_C_FLAGS "-std=c11 -O2 -Wall")
add_executable(mumsh ${SOURCE_FILES})
add_executable(mumsh_memory_check ${SOURCE_FILES})

