cmake_minimum_required(VERSION 3.16)
project(lru-cache VERSION 1.0)

add_library(LruCache INTERFACE)

set(CMAKE_CXX_STANDARD 17)

add_library(cache::lru-cache ALIAS LruCache)

install(DIRECTORY include DESTINATION include)
install(TARGETS LruCache EXPORT LruCacheConfig)
install(EXPORT LruCacheConfig NAMESPACE cache:: DESTINATION share/LruCache/cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/LruCacheConfigVersion.cmake"
        VERSION
        ${PROJECT_VERSION}
        COMPATIBILITY
        AnyNewerVersion)

install(FILES "${PROJECT_BINARY_DIR}/LruCacheConfigVersion.cmake" DESTINATION share/LruCache/cmake)

if (DEBUG)
    if (SANITIZERS)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
    endif ()

    if (TESTING)
        enable_testing()
        add_subdirectory(tests)
    endif ()
endif ()