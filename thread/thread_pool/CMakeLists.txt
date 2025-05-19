cmake_minimum_required(VERSION 3.26)
project(thread_pool)

set(CMAKE_CXX_STANDARD 20)

add_executable(thread_pool
        main.cpp
        thread_pool.h)
target_link_libraries(thread_pool PUBLIC -pthread)