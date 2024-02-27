cmake_minimum_required(VERSION 3.25)
project(RISC_V_Cross_compiler C)

set(CMAKE_C_STANDARD 11)

add_executable(RISC_V_Cross_compiler main.c include/lexer/token.h include/lexer/lexer.h include/utils/vector.h)
