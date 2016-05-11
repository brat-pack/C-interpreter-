cmake_minimum_required(VERSION 3.5)
project(C_interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
file(COPY "files" DESTINATION "files")
add_executable(C_interpreter ${SOURCE_FILES})