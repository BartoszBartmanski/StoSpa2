# pystospa

[![Build Status](https://travis-ci.org/BartoszBartmanski/StoSpa2.svg?branch=master)](https://travis-ci.org/BartoszBartmanski/StoSpa2)
[![Documentation Status](https://readthedocs.org/projects/stospa2/badge/?version=latest)](https://stospa2.readthedocs.io/en/latest/?badge=latest)

A Python binding of a C++ software package for stochastic simulations of spatially extended systems, [StoSpa2](https://github.com/BartoszBartmanski/StoSpa2). Code-base has been completely refactored since the previous version of [StoSpa](https://github.com/BartoszBartmanski/StoSpa). Python bindings have also been included.


## Installation

### Easy way
```
pip install pystospa
```

### Hard way
After cloning the following repository
```
git clone https://github.com/BartoszBartmanski/StoSpa2.git --recursive
```
do the following
```
cd StoSpa2
python setup.py install
```

This way of installing pystospa assumes the following packages are installed are met
* scikit-build
* setuptools
* wheel
* cmake

## Example

Let's consider the following chemical reaction
```
A \rightarrow \emptyset
```
happening at rate `k`. The python code for this simulation is as follows
```
import pystospa as ss

v = ss.Voxel([100], 1.0)
r = ss.Reaction(1.0, lambda x, y : x[0], [-1])
v.add_reaction(r)

s = ss.Simulator([v])
s.run("example.dat", 0.01, 500)
```

After importing pystospa, we create the voxel and reaction objects with
```
v = ss.Voxel([100], 1.0)
r = ss.Reaction(1.0, lambda x, y : x[0], [-1])
```
and then we add the reaction object to the voxel object.
```
v.add_reaction(r)
```
And finally, we pass the voxel objects, contained in a list, to a simulator object
```
s = ss.Simulator([v])
s.run("example.dat", 0.01, 500)
```
and we invoke the `run` function of the Simulator class, to run a stochastic simulation saving the state every 0.01 seconds for 500 steps.
