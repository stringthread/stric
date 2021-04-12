cmake_minimum_required(VERSION 3.10)
project(stric_object CXX)

add_library(object STATIC
  object.cpp
  meta.cpp
  int.cpp
  bool.cpp
  operators.cpp
)
