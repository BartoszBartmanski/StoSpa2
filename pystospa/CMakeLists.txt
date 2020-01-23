
include_directories(src)
pybind11_add_module(pystospa src/pystospa.cpp)
install(TARGETS pystospa LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})
