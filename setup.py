
from skbuild import setup

setup(
    name='pystospa',
    version='2.0.16',
    author='Bartosz Bartmanski',
    author_email='bartoszbartmanski@gmail.com',
    description='A python binding of C++ package for stochastic simulations of spatially extended systems',
    platforms=["Linux"],
    packages=['pystospa'],
    setup_requires=["setuptools", "wheel", "scikit-build >= 0.10.0", "cmake >= 3.5"],
    zip_safe=False,
)
