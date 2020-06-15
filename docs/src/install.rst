
Installation
############

There are two ways of using StoSpa2: with Python or with C++. To install `pystospa`, we only need to follow the installation instructions found in the Python section below and likewise, to compile the C++ code, we only need to follow the instructions in the C++ section.

Prerequisites
=============

Necessary:
    * C++ compiler (GCC, clang)
    * Cmake (>= 3.5)
    * Make (or ninja)

Necessary for pystospa:
    * C++ compiler (GCC, clang)
    * Cmake (>= 3.5)
    * Make (or ninja)
    * Python (>=3.5)
    * Pybind11
    * Scikit-build

Optional:
    * Matplotlib - to run python examples
    * Numpy - to run python examples
    * Boost - if boost branch of StoSpa2 is used
    * Doxygen - to build C++ documentation
    * Sphinx - to build python documentation


Python installation
===================

Note that `scikit-build <https://github.com/scikit-build/scikit-build/>`_ needs to be installed before
installing pystospa, which can be done using the following command:

.. code-block:: bash

    pip install scikit-build

Furthermore, there is no need to compile the C++ code yourself - assuming the dependencies are met, pip and/or scikit-build compiles the C++ part of the software for us.

Easy way
--------

.. code-block:: bash

    pip install pystospa

Hard way
--------

After cloning the following repository

.. code-block:: bash

    git clone https://github.com/BartoszBartmanski/StoSpa2.git --recursive

do the following

.. code-block:: bash

    cd StoSpa2
    python setup.py install

This way of installing pystospa assumes the following packages are installed are met

* scikit-build

* setuptools

* wheel

* cmake


C++ installation
================

The installation steps are as follows:

1. We clone the repository

    .. code-block:: bash

        git clone https://github.com/BartoszBartmanski/StoSpa2.git --recursive

    Note the `recursive` flag at the end of the command, which is needed to compile python-binding of
    StoSpa2.

2. We go to the StoSpa2 root directory and make a build directory

    .. code-block:: bash

        cd StoSpa2
        mkdir build
        cd build

3. We compile

    .. code-block:: bash

        cmake ../
        make all

After the above command we can run the example executable (contained in your current working directory, `<project_dir>/build`), by executing the following command

    .. code-block:: bash

        ./examples/cme_example

which generates an `cme_example.dat` file in your current working directory.

This is the basic installation. There are more options, such as installation that uses the Boost library,
covered in Branches section below.

Another way to install is to execute `setup_stospa.sh` script which will do the above steps for you
(as long as you can execute bash scripts).


Branches
========

There are three branches on the github repository (https://github.com/BartoszBartmanski/StoSpa2.git):
`master`, `c++17` and `boost`. The differences between these are how time is stored in the `Simulator` class, as
the implementation of the Gillespie algorithm (https://pubs.acs.org/doi/abs/10.1021/j100540a008) we are using
is next the sub-volume method, which involves keeping a time for each voxel.

In the `master` branch we keep the dependencies to the minimum (no need to deal with the Boost library) and as such
we use the standard C++ library map to store the times for all the voxels. In the `c++17` branch we update the times for all
voxels using functions only available from C++17 standard onwards. And lastly, there is
the `boost` branch, which uses Fibonacci heap from the Boost library for the voxel times. The differences in
performance are noticeable only in cases with large numbers of voxels (more than 1000), where the `boost` branch
is faster.

The `master` branch and the `boost` branch both have python-bindings available at PyPI, which can be installed using
the following commands. First to install the `master` branch python-binding we use the following

    .. code-block:: bash

        pip install pystospa

To install the `boost` python-binding, we use

    .. code-block:: bash

        pip install pystospaboost

We hope that this choice reduces problems with installation due to dependency issues and if in doubt use
the `master` branch.

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

Testing
=======

To make sure the package has been installed correctly we can run unit tests.

If C++ code was compiled, we can execute the following commands when in the build directory (`<project_dir>/build/`):

    .. code-block:: bash

        make unittests
        ./tests/unittests

If Python binding of StoSpa2, pystospa, was installed, we can run the unit tests using the following command
when within the tests directory (`<project_dir>/tests/`):

    .. code-block:: bash

        python ./test_pystospa.py
