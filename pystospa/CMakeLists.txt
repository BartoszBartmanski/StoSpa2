
# Make sure header files can be included
include_directories(src)

# Use pybind11 to create pystospa
pybind11_add_module(pystospa src/pystospa.cpp)

# Generate __init__.py
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__init__.py INPUT ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in)

# Install the binary image
install(TARGETS pystospa LIBRARY DESTINATION pystospa)

# Install __init__.py file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION pystospa)
