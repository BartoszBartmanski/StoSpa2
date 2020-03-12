
import os
import platform
from skbuild import setup


# Read in the version number
with open("version", "r") as fh:
    version = fh.read().strip()

# Read in the description
with open(os.path.join("pystospa", "README.md"), "r") as fh:
    long_description = fh.read()

cmake_args = []
if platform.system() == "Darwin":
    cmake_args.append("-DCMAKE_CXX_FLAGS=-stdlib=libc++")

# Run the installation
setup(
    name='pystospa',
    version=version,
    author='Bartosz Bartmanski',
    author_email='bartoszbartmanski@gmail.com',
    description='A python binding of C++ package for stochastic simulations of spatially extended systems',
    long_description=long_description,
    long_description_content_type="text/markdown",
    platforms=["Linux"],
    cmake_args=cmake_args,
    packages=['pystospa'],
    setup_requires=["setuptools", "wheel", "scikit-build >= 0.10.0", "cmake >= 3.5"],
    zip_safe=False,
)
