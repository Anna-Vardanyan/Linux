cmake_minimum_required(VERSION 3.26)
project(super_rm)

set(CMAKE_CXX_STANDARD 20)

add_executable(super_rm
        main.cpp
        super_rm.h)

target_include_directories(super_rm PRIVATE ../utils/install/include)
target_link_directories(super_rm PRIVATE ../utils/install/lib)
target_link_libraries(super_rm PRIVATE -llogger_shared)