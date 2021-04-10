cmake_minimum_required(VERSION 3.10)
project(stric CXX)

add_subdirectory(parser)
add_subdirectory(object)
add_executable(parse
  parse.cpp
)
target_link_libraries(parse
  parser
)
add_executable(lex
  lex.cpp
)
target_link_libraries(lex
  parser
)
add_executable(main
  main.cpp
  executor.cpp
)
target_link_libraries(main
  parser
  object
)
link_directories(parser)
link_directories(object)
