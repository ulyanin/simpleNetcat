cmake_minimum_required(VERSION 3.0)
project(simpleNetcat)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -Werror -Wextra")
set(SOURCE_FILES nc.c)
add_executable(simpleNetcat ${SOURCE_FILES})
target_link_libraries(simpleNetcat ${CMAKE_DL_LIBS})

