cmake_minimum_required(VERSION 3.18)
project(kernel C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

file(GLOB_RECURSE kerCSrc ${PROJECT_SOURCE_DIR}/*.c)
add_executable(kernel ${kerCSrc})