#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pystospa as ss


# Create a vector of voxel objects. Voxel arguments: vector of number of molecules, size of the voxel
initial_num = [10000, 0]
voxel_size = 1.0
domain = [ss.Voxel(initial_num, voxel_size)]
# We add nine voxels with no molecules
for i in range(9):
    domain.append(ss.Voxel([0, 0], voxel_size))

# Create and add the reaction objects
d = 1.0  # diffusion rate
k = 0.001
prop_diff_A = lambda num_mols, length : num_mols[0]
prop_diff_B = lambda num_mols, length : num_mols[1]
prop_other = lambda num_mols, length : num_mols[0] * (num_mols[0] - 1) / length
stoch_diff_A = [-1, 0]
stoch_diff_B = [0, -1]
stoch_other = [-2, 1]
for i, vox in enumerate(domain):
    # Add diffusion jump to the right from voxel i to voxel i+1 (if one exists)
    if i < len(domain)-1:
        vox.add_reaction(ss.Reaction(d, prop_diff_A, stoch_diff_A, i+1))
        vox.add_reaction(ss.Reaction(d, prop_diff_B, stoch_diff_B, i+1))
    # Add diffusion jump to the left from voxel i+1 to voxel i (if one exists)
    if i > 0:
        vox.add_reaction(ss.Reaction(d, prop_diff_A, stoch_diff_A, i-1))
        vox.add_reaction(ss.Reaction(d, prop_diff_B, stoch_diff_B, i-1))
    vox.add_reaction(ss.Reaction(k, prop_other, stoch_other))

# Pass the voxels with the reaction(s) to the simulator object
s = ss.Simulator(domain)

# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("rdme_example_3.dat", 0.01, 200)

# Read the file containing the data
data = np.loadtxt("rdme_example_3.dat")
time = data[:, 0]
num_A = data[:, 1::2]
num_B = data[:, 2::2]
x = np.arange(0.5, 10.5)

# We generate a canvas for the figure
fig, axs = plt.subplots(1, 2, figsize=(10, 5), sharey=True)
titles = [r"Species $A$", r"Species $B$"]
for i, num in enumerate([num_A, num_B]):
    axs[i].bar(x, num[0], label="t = {:.2}".format(time[0]))
    axs[i].bar(x, num[50], label="t = {:.2}".format(time[50]), alpha=0.5)
    axs[i].bar(x, num[100], label="t = {:.2}".format(time[100]), alpha=0.3)
    axs[i].set_xlabel(r"$x$-position")
    axs[i].set_ylabel("Number of molecules")
    axs[i].set_title(titles[i])
    axs[i].legend()
fig.savefig("rdme_example_3.svg")
