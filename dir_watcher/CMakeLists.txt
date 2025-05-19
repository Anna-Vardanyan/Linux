cmake_minimum_required(VERSION 3.26)
project(dir_watcher)

set(CMAKE_CXX_STANDARD 20)

add_executable(dir_watcher
        main.cpp
        daemon.h
        daemon.cpp)

