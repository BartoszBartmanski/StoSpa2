#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pystospa as ss

# Create voxel object. Arguments: vector of number of molecules, size of the voxel
initial_num = [100]  # number of molecules of species A
domain_size = 10.0  # size of the domain in cm
v = ss.Voxel(initial_num, domain_size)

# Create reaction object.
# Arguments: reaction rate, propensity func, stoichiometry vector
k = 1.0
propensity = lambda num_mols, area : num_mols[0]
stoch = [-1]
r = ss.Reaction(k, propensity, stoch)

# Add a reaction to a voxel
v.add_reaction(r)

# Pass the voxel with the reaction(s) to the simulator object
s = ss.Simulator([v])
# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("cme_example.dat", 0.01, 500)

# Read the file containing the data
data = np.loadtxt("cme_example.dat")
time = data[:, 0]  # Time points
num_A = data[:, 1]  # Number of molecules of A

# Plot of the data
fig, ax = plt.subplots()
ax.step(time, num_A, label="A")
ax.set_xlabel("Time [s]")
ax.set_ylabel("Number of molecules")
ax.legend()
fig.savefig("cme_example.svg")
