cmake_minimum_required(VERSION 3.5)
project(C_interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")


set(SOURCE_FILES NFA_Constructor.c NFA_Constructor.h reader.c lists.c reader.h lists.h RegexParser.h RegexParser.c PrettyPrinter.h PrettyPrinter.c)
add_executable(C_interpreter ${SOURCE_FILES})