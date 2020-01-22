

# Find Doxygen package
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
    set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file(${doxyfile_in} ${doxyfile}  @ONLY)

    add_custom_target(docs-doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

# Find Sphinx package
find_package(Sphinx)

set(sphinx_conf_in "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in")
set(sphinx_conf "${CMAKE_CURRENT_SOURCE_DIR}/src/conf.py")
configure_file(${sphinx_conf_in} ${sphinx_conf} @ONLY)

if(SPHINX_FOUND AND DOXYGEN_FOUND)
    add_custom_target(docs-sphinx
        COMMAND ${SPHINX_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_BINARY_DIR}/sphinx
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

endif()
