cmake_minimum_required(VERSION 3.5)

# Read in the version number
file(STRINGS "version" stospa2_version)

# Project name
project(StoSpa2 LANGUAGES CXX VERSION "${stospa2_version}")

# Generate the header file with version numbers
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/pystospa/src/version.hpp @ONLY)
message("-- Project version: ${PROJECT_VERSION}")

# We set the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# If build type is not specified then Release is used
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
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

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++i -mmacosx-version-min=10.9")
endif(APPLE)

# Projects need c++14 standard - for make_unique and make_shared
set(CMAKE_CXX_STANDARD 14)

# Include all the header files and add executables found within benchmarks and tests directories
include_directories(src)
add_subdirectory(benchmarks)
add_subdirectory(tests)

add_subdirectory(docs)

# Add pybind11 and compile a python module
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/pybind11/CMakeLists.txt")
	add_subdirectory(pybind11)
	add_subdirectory(pystospa)
else()
	message("-- Could NOT find pybind11")
endif()

# Add an executable for the example
add_executable(example src/example.cpp)
