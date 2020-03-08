
import os
from skbuild import setup


# Read in the version number
with open("version", "r") as fh:
    version = fh.read().strip()

# Read in the description
with open(os.path.join("pystospaboost", "README.md"), "r") as fh:
    long_description = fh.read()

# Run the installation
setup(
    name='pystospaboost',
    version=version,
    author='Bartosz Bartmanski',
    author_email='bartoszbartmanski@gmail.com',
    description='A python binding of C++ package for stochastic simulations of spatially extended systems',
    long_description=long_description,
    long_description_content_type="text/markdown",
    platforms=["Linux"],
    packages=['pystospaboost'],
    setup_requires=["setuptools", "wheel", "scikit-build >= 0.10.0", "cmake >= 3.5"],
    zip_safe=False,
)
