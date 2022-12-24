cmake_minimum_required(VERSION 3.20)
project(cconsole)

set(CMAKE_CFG_INTDIR ./obj)
set(CMAKE_CFG_BINARY_DIR ./bin)

include_directories(./inc)

add_executable(cconsole 
    src/main.c
    inc/fw.h
    inc/parser.h)
target_link_libraries(cconsole)