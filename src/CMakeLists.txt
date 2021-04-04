cmake_minimum_required(VERSION 3.10)
project(stric CXX)

add_subdirectory(parser)
add_executable(parse
  parse.cpp
)
target_link_libraries(parse
  parser
)
link_directories(parser)
