
Installation
############

Below we outline how to get started with StoSpa2. The basic installation steps are as follows:

1. We clone the repository

    .. code-block:: bash

        git clone https://github.com/BartoszBartmanski/StoSpa2.git

2. We go to the StoSpa2 root directory and make a build directory

    .. code-block:: bash

        cd StoSpa2
        mkdir build
        cd build

3. We compile

    .. code-block:: bash

        cmake ../
        make all

and this is the basic installation. There are more options, such as python binding for the C++ code
or compiling documentation, which are covered in the sections below.

Another option is to execute `setup_stospa.sh` script which will do the above steps for you
(as long as you can execute bash scripts).


Prerequisites
=============

Necessary:
    * C++ compiler (GCC, clang)
    * Cmake (>= 3.5)
    * Make (or ninja)

Optional:
    * Pybind11 - to build pystospa (python binding of StoSpa2)
    * Python (>=3.5)
    * Boost - if boost branch of StoSpa2 is used
    * Doxygen - to build C++ documentation
    * Sphinx - to build python documentation

Branches
========

There are three branches on the github repository (https://github.com/BartoszBartmanski/StoSpa2.git):
`master`, `c++17` and `boost`. The differences between these are how time is stored in the `Simulator` class, as
the implementation of Gillespie algorithm (https://pubs.acs.org/doi/abs/10.1021/j100540a008) we are using
is next sub-volume method, which involves keeping a time for each voxel.

In `master` branch we keep the dependencies to the minimum (no need to deal with Boost library) and as such
we use standard C++ library map to store the times for all the voxels. In `c++17` branch we update the times for all
voxels that uses map member functions only available from C++17 standard onwards. And lastly, there is
the `boost` branch, which uses Fibonacci heap from the Boost library for the voxel times. The differences in
performance are noticeable only in cases with large number of voxels (more than 1000), where the `boost` branch
is faster.

We hope that this choice reduces problems with installation due to dependency issues and if in doubt use
the `master` branch.

Pystospa
========

Installing the python binding of StoSpa2, pystospa, we only need to type in the following command

    .. code-block:: bash

        pip install pystospa

Documentation
=============

To compile documentation for the C++ API, doxygen is needed and to compile Python API and a
getting started guide, both doxygen and sphinx are needed.

If doxygen has been detected by cmake, then we can generate the C++ API documentation by
compiling docs-doxgen (after calling cmake in the build directory)

    .. code-block:: bash

        make docs-doxygen

For more comprehensive guide to StoSpa2, we recommend that the documentation that uses sphinx is
compiled

    .. code-block:: bash

        make docs-sphinx

Either of the documentations can be found in the `<build directory>/docs/`
