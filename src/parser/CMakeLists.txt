cmake_minimum_required(VERSION 3.10)
project(stric_parser CXX)

add_library(parser STATIC
  syntax.cpp
  lexer.cpp
  dfa.cpp
  parser_gen.cpp
  parser.cpp
  parser_operations.cpp
)
