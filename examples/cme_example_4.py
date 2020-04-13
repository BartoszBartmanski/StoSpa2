#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pystospa as ss


# Create voxel object. Arguments: vector of number of molecules, size of the voxel
initial_num = [100, 50, 20]  # number of molecules of species A, B and C respectively
domain_size = 10.0  # size of the domain in cm
v = ss.Voxel(initial_num, domain_size)

# Create reaction object for reaction A + B -> C.
# Arguments: reaction rate, propensity func, stoichiometry vector
k1 = 0.1
propensity1 = lambda num_mols, area : num_mols[0]*num_mols[1]/area
stoch1 = [-1, -1, 1]
r1 = ss.Reaction(k1, propensity1, stoch1)

# Create reaction object for reaction B + 2C -> 0. Arguments: reaction rate, propensity func, stoichiometry vector
k2 = 10.0
propensity2 = lambda num_mols, area : num_mols[1] * num_mols[2] * (num_mols[2] - 1) / (area**2)
stoch2 = [0, -1, -2]
r2 = ss.Reaction(k2, propensity2, stoch2)

# Add a reaction to the voxel
v.add_reaction(r1)
v.add_reaction(r2)

# Pass the voxel with the reaction(s) to the simulator object
s = ss.Simulator([v])

# Run the simulation. Arguments: path to output file, time step, number of steps
s.run("cme_example_4.dat", 0.01, 500)

# Read the file containing the data
data = np.loadtxt("cme_example_4.dat")
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
fig.savefig("cme_example_4.svg")
