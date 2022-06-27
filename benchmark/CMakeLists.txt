cmake_minimum_required(VERSION 3.16)

project(benchmark)

add_executable(${PROJECT_NAME} lru-cache_benchmark.cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)

target_link_libraries(${PROJECT_NAME} PUBLIC benchmark::benchmark)