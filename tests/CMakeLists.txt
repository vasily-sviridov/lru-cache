cmake_minimum_required(VERSION 3.16)

project(tests)

add_executable(${PROJECT_NAME} lru-cache_tests.cpp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
add_subdirectory(third_party/random)

target_link_libraries(${PROJECT_NAME} PUBLIC effolkronium_random gtest gtest_main)