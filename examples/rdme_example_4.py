#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from os import path
import pystospa as ss

assert path.isfile("rdme_example.dat"), "Note: 'rdme_example.py' script needs to be run before this one"


# Create a vector of voxel objects. Voxel arguments: vector of number of molecules, size of the voxel
initial_num = [10000]
voxel_size = 1.0
growth = lambda t : np.exp(0.2 * t)
domain = [ss.Voxel(initial_num, voxel_size, growth)]
# We add nine voxels with no molecules
for i in range(9):
    domain.append(ss.Voxel([0], voxel_size, growth))

# Create and add the reaction objects
d = 1.0  # diffusion rate
propensity = lambda num_mols, length : num_mols[0]
stoch = [-1]
for i in range(9):
    # Add diffusion jump to the right from voxel i to voxel i+1
    domain[i].add_reaction(ss.Reaction(d, propensity, stoch, i+1))
    # Add diffusion jump to the left from voxel i+1 to voxel i
    domain[i+1].add_reaction(ss.Reaction(d, propensity, stoch, i))

# Pass the voxels with the reaction(s) to the simulator object
s = ss.Simulator(domain)

# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("rdme_example_4.dat", 0.01, 200)

# Read the file containing the data
data = np.loadtxt("rdme_example_4.dat")
data_static = np.loadtxt("rdme_example.dat")
time = data[:, 0]
num_A = data[:, 1:]
num_A_static = data_static[:, 1:]
x = np.arange(0.5, 10.5)
growth = lambda t : np.exp(0.2 * t)

# We generate a canvas for the figure
fig, ax = plt.subplots()
ax.bar(x*growth(time[199]), num_A[199], label="Growing domain", width=0.8*growth(time[199]))
ax.bar(x, num_A_static[199], label="Static domain")
ax.set_xlabel(r"$x$-position")
ax.set_ylabel("Number of molecules")
ax.legend()
fig.savefig("rdme_example_4.svg")
