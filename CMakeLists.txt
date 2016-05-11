cmake_minimum_required(VERSION 3.0)
project(simplyNetcat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c89 -Wall -Wextra")
set(SOURCE_FILES nc.c)
add_executable(simplyNetcat ${SOURCE_FILES})
target_link_libraries(simplyNetcat ${CMAKE_DL_LIBS})
