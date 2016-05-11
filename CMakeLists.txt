cmake_minimum_required(VERSION 3.5)
project(simpleNetcat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c89 -Werror -Wextra")

set(SOURCE_FILES nc.c)
add_executable(simpleNetcat ${SOURCE_FILES})