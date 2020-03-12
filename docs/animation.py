#!/usr/bin/env python

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import sys


# We get the name of the file containing the data
fname = "example.dat" if len(sys.argv) == 1 else sys.argv[1]

# We open the file containing the data
data = np.loadtxt(fname)
time = data[:, 0]  # First column are the values of time at each timepoint
voxels = data[:, 1:]  # All the other columns are the number of molecules

# We generate a canvas for the figure
fig, ax = plt.subplots()

# We plot the first frame in the animation
i = 0
bars = ax.bar(np.arange(voxels.shape[1]), voxels[i])
ax.set_title("t = {0:.2f}".format(time[i]))

# We define a function that updates the canvas
def update(i):
    # We update the values of the time
    for rect, yi in zip(bars, voxels[i]):
        rect.set_height(yi)
    ax.set_title("t = {0:.2f}".format(time[i]))
    return bars, ax

# We generate and save the animation of the simulation
anim = FuncAnimation(fig, update, frames=np.arange(10, 500, 10), interval=50)
anim.save('sim.gif', dpi=100, writer="imagemagick")
