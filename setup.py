
from skbuild import setup

with open("pystospa/README.md", "r") as fh:
    long_description = fh.read()

setup(
    name='pystospa',
    version='2.0.19',
    author='Bartosz Bartmanski',
    author_email='bartoszbartmanski@gmail.com',
    description='A python binding of C++ package for stochastic simulations of spatially extended systems',
    long_description=long_description,
    long_description_content_type="text/markdown",
    platforms=["Linux"],
    packages=['pystospa'],
    setup_requires=["setuptools", "wheel", "scikit-build >= 0.10.0", "cmake >= 3.5"],
    zip_safe=False,
)
