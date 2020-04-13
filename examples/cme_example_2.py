#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pystospa as ss


# Create voxel object. Arguments: vector of number of molecules, size of the voxel
initial_num = [100, 70, 0]  # number of molecules of A, B and C repectively
domain_size = 10.0  # size of the domain in cm
v = ss.Voxel(initial_num, domain_size)

# Create reaction object for reaction A + B -> C.
# Arguments: reaction rate, propensity func, stoichiometry vector
k = 1.0
propensity = lambda num_mols, length : num_mols[0]*num_mols[1]/length
stoch = [-1, -1, 1]
r = ss.Reaction(k, propensity, stoch)

# Add a reaction to a voxel
v.add_reaction(r)

# Pass the voxel with the reaction(s) to the simulator object
s = ss.Simulator([v])

# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("cme_example_2.dat", 0.01, 500)

# Read the file containing the data
data = np.loadtxt("cme_example_2.dat")
time = data[:, 0]
num_A = data[:, 1]
num_B = data[:, 2]
num_C = data[:, 3]

# Plot of the data
fig, ax = plt.subplots()
ax.step(time, num_A, label="A")
ax.step(time, num_B, label="B")
ax.step(time, num_C, label="C")
ax.set_xlabel("Time [s]")
ax.set_ylabel("Number of molecules")
ax.legend()
fig.savefig("cme_example_2.svg")
