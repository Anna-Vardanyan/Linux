cmake_minimum_required(VERSION 3.26)
project(blocking_queue)

set(CMAKE_CXX_STANDARD 14)

add_executable(blocking_queue
        blocking_queue.h
        main.cpp)

target_include_directories(blocking_queue PRIVATE ../)