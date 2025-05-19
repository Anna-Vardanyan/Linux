cmake_minimum_required(VERSION 3.10)

project(Logger)

set(SOURCES logger.cpp
        logger.h)

add_library(logger_shared SHARED ${SOURCES})
target_include_directories(logger_shared PRIVATE .)
target_compile_options(logger_shared PRIVATE -std=c++17)

add_library(logger_static STATIC ${SOURCES})
target_include_directories(logger_static PRIVATE .)
target_compile_options(logger_static PRIVATE -std=c++17)

install(TARGETS logger_shared logger_static  DESTINATION lib/)
install(FILES logger.h DESTINATION include/)