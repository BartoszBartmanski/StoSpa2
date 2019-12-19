cmake_minimum_required(VERSION 3.5)

# Project name
project(StoSpa LANGUAGES CXX VERSION 2.0.0)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# Set the compile flags according to cmake build type
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wpedantic")
else(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
message("-- CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("-- CMAKE_CXX_FLAGS:${CMAKE_CXX_FLAGS}")

# Projects need c++14 standard - for make_unique and make_shared
set(CMAKE_CXX_STANDARD 14)

if(NOT $(CMAKE_BUILD_TYPE))
	set(CMAKE_BUILD_TYPE DEBUG)
endif()

include_directories(src)
add_subdirectory(benchmarks)
add_subdirectory(tests)

add_subdirectory(pybind11)
pybind11_add_module(stospa2py src/stospa2py.cpp)
