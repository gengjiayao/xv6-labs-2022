cmake_minimum_required(VERSION 3.18)
project(user C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

set(MY_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${MY_HEADER_DIR})

file(GLOB_RECURSE usrCSrc ${PROJECT_SOURCE_DIR}/*.c)
add_executable(user ${usrCSrc})