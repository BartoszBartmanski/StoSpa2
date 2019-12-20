# StoSpa2 [![Build Status](https://travis-ci.org/BartoszBartmanski/StoSpa2.svg?branch=master)](https://travis-ci.org/BartoszBartmanski/StoSpa2)

A C++ software package for stochastic simulations of spatially extended systems. Code-base has been completely refactored since the previous version of [StoSpa](https://github.com/BartoszBartmanski/StoSpa). Python bindings have also been included.

## Requirements

* cmake
* pybind11
* Python >= 3.6

## Installation

After cloning this repository using
```
git clone https://github.com/BartoszBartmanski/StoSpa2.git --recursive
```
compile the package with
```
cd StoSpa2
mkdir build
cmake ../
make all
```

## Example

Included in the `src` directory is a cpp file `example.cpp` where we run a simple simulation of diffusion on a one-dimensional domain [0, 10] discretised into 10 voxels.

First, we include the main header from StoSpa2 and we start the main function that will contain the code for the simulation.
```
#include "simulator.hpp"

int main() {
```
Within the `main` function we first define a lambda function for the propensity function of a diffusion reaction.
```
auto diffusion = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };
```
Then, we create an array of voxels, each of size 1, with 10000 molecules in the leftmost one, as shown below.
```
std::vector<Voxel> vs = std::vector<Voxel>(9, Voxel({0}, 1.0));
vs.insert(vs.begin(), Voxel({10000}, 1.0));
```
We add diffusion reactions to all the voxels
```
for (unsigned i=0; i<vs.size()-1; i++) {
    vs[i].add_reaction(Reaction(1.0, diffusion, {-1}, i+1));
    vs[i+1].add_reaction(Reaction(1.0, diffusion, {-1}, i));
}
```
and finally, we create a simulator instance and run the simulation.
```
Simulator s2(vs);
s2.run("example.dat", 0.01, 1000);
}
```

The resulting simulation output:

![Simulation output](.sim.gif)
