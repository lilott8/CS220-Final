cmake_minimum_required(VERSION 2.8.4)
project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
add_compile_options("-v")

include_directories(
    ${PROJECT_SOURCE_DIR}/Headers
)
add_subdirectory(Source)
add_subdirectory(Utilities)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_INLCUDE_DIR /usr/local/Cellar/boost/1.57.0/includes)
find_package(Boost 1.57.0 COMPONENTS Polygon geometry)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #add_executable(flow file1.cxx file2.cxx)
    target_link_libraries(final ${Boost_LIBRARIES})
endif()