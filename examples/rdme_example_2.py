#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pystospa as ss

# Create a vector of voxel objects. Voxel arguments: vector of number of molecules, size of the voxel
initial_num = [10000]
voxel_size = 1.0
domain = [ss.Voxel(initial_num, voxel_size)]
# We add nine voxels with no molecules
for i in range(9):
    domain.append(ss.Voxel([0], voxel_size))

# Create and add the reaction objects
d = 1.0  # diffusion rate
k = 1.0  # decay rate
propensity = lambda num_mols, length : num_mols[0]
stoch = [-1]
for i, vox in enumerate(domain):
    # Add diffusion jump to the right from voxel i to voxel i+1 (if one exists)
    if i < len(domain)-1: vox.add_reaction(ss.Reaction(d, propensity, stoch, i+1))
    # Add diffusion jump to the left from voxel i+1 to voxel i (if one exists)
    if i > 0: vox.add_reaction(ss.Reaction(d, propensity, stoch, i-1))
    vox.add_reaction(ss.Reaction(k, propensity, stoch))

# Pass the voxels with the reaction(s) to the simulator object
s = ss.Simulator(domain)

# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("rdme_example_2.dat", 0.01, 200)

# Read the file containing the data
data = np.loadtxt("rdme_example_2.dat")
time = data[:, 0]
num_A = data[:, 1:]
x = np.arange(0.5, 10.5)

# We generate a canvas for the figure
fig, ax = plt.subplots()
ax.bar(x, num_A[0], label="t = {:.2}".format(time[0]))
ax.bar(x, num_A[50], label="t = {:.2}".format(time[50]), alpha=0.5)
ax.bar(x, num_A[100], label="t = {:.2}".format(time[100]), alpha=0.5)
ax.set_xlabel(r"$x$-position")
ax.set_ylabel("Number of molecules")
ax.legend()
fig.savefig("rdme_example_2.svg")
