cmake_minimum_required(VERSION 3.23)
project(ThreadAPI)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadAPITest main.cpp details/LinuxThreadDetails.h LinuxThread.h LinuxMutex.h)
target_link_libraries(ThreadAPITest PUBLIC -lpthread)