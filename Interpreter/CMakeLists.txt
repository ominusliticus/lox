cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    ilox
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(
    -Wall -Wpedantic -Wextra
    -ferror-limit=3
    -std=c++20
    -Wno-gnu-statement-expression
)

add_subdirectory(util)
add_subdirectory(internals)

set(SOURCE_FILES ${UTIL_SOURCES} ${INTERNALS_SOURCES})

add_executable(ilox ilox.cpp)

target_include_directories(ilox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(ilox PUBLIC ${SOURCE_FILES})

