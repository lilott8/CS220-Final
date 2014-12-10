cmake_minimum_required(VERSION 2.8.4)
project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
add_compile_options("-v")

include_directories(
    ${PROJECT_SOURCE_DIR}/Headers
)
add_subdirectory(Source)
add_subdirectory(Utilities)