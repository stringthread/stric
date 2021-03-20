cmake_minimum_required(VERSION 3.10)
project(stric_root CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

add_subdirectory(src)

if(NOT without-test)
  enable_testing()
  add_subdirectory(test)
  add_test(
    NAME tests
    COMMAND tests
  )
endif()
