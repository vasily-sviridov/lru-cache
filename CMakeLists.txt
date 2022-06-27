cmake_minimum_required(VERSION 3.16)
project(lru-cache)

add_library(lru-cache INTERFACE)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)
add_subdirectory(benchmark)
