cmake_minimum_required(VERSION 3.10)
project(stric CXX)

add_library(lexer STATIC lexer.cpp syntax.cpp)

add_library(dfa STATIC dfa.cpp syntax.cpp)

add_library(parser_gen STATIC parser_gen.cpp parser_operations.cpp syntax.cpp)

add_library(parser STATIC parser.cpp parser_operations.cpp syntax.cpp)
