#!/usr/bin/env python

import numpy as np
import matplotlib
from matplotlib.collections import PolyCollection
import matplotlib.pyplot as plt
import meshio
import pystospaboost as ss

matplotlib.rcParams.update({'font.size': 8})


def save(figure, filename):
    for fmt in ("svg", "png"):
        figure.savefig("{}.{}".format(filename, fmt), transparent=True, dpi=600)


def fig1(filename):
    fig, axs = plt.subplots(1, 2, figsize=(6, 3.5), sharey=True)

    msh = meshio.read("unstructured.msh")
    axs[0].triplot(msh.points[:, 0], msh.points[:, 1], msh.cells[2][1])
    axs[0].set_xlabel(r"$x$-position [cm]")
    axs[0].set_ylabel(r"$y$-position [cm]")

    msh = meshio.read("structured.msh")
    pc = PolyCollection(msh.points[:, :2][msh.cells[2][1]], linewidths=1.5, edgecolor="#1F77B4", facecolor="None")
    axs[1].add_collection(pc)
    axs[1].set_xlabel("$x$-position [cm]")
    axs[1].set_xlim([-0.05, 1.05])
    axs[1].set_ylim([-0.05, 1.05])

    save(fig, filename)

def fig2(filename):

    v = ss.Voxel([100], 1.0)
    r = ss.Reaction(1.0, lambda x, y : x[0], [-1])
    v.add_reaction(r)

    s = ss.Simulator([v])
    s.run(filename+".dat", 0.01, 500)

    data = np.loadtxt(filename+".dat")

    fig, ax = plt.subplots(figsize=(6, 3.5))
    plt.subplots_adjust(left=0.3, right=0.7, bottom=0.2)
    ax.step(data[:, 0], data[:, 1])
    ax.set_xlabel("Time [s]")
    ax.set_ylabel("Number of molecules")

    save(fig, filename)


def fig3(filename):

    domain = [ss.Voxel([10000], 1.0)] + [ss.Voxel([0], 1.0) for i in range(9)]

    for i in range(len(domain)-1):
        domain[i].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i+1))
        domain[i+1].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i))

    s = ss.Simulator(domain)
    s.run(filename+".dat", 0.01, 500)

    data = np.loadtxt(filename+".dat")
    x = np.arange(10) + 0.5

    fig, axs = plt.subplots(1, 2, figsize=(6, 3.5), sharey=True)
    plt.subplots_adjust(bottom=0.2, wspace=0.4)
    idxs = [0, -1]
    for i, ax in enumerate(axs):
        ax.bar(x, data[idxs[i], 1:])
        ax.set_title("t = {} s".format(np.round(data[idxs[i], 0], 1)))
        ax.set_xlabel("Position [cm]")
    axs[0].set_ylabel("Number of molecules")

    save(fig, filename)

def fig4(filename):

    growth = lambda t : np.exp(0.2 * t)
    domain = [ss.Voxel([10000], 1.0, growth)] + [ss.Voxel([0], 1.0, growth) for i in range(9)]

    for i in range(len(domain)-1):
        domain[i].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i+1))
        domain[i+1].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i))

    s = ss.Simulator(domain)
    s.run(filename+".dat", 0.01, 500)

    fig3("rdme_example")

    data = np.loadtxt(filename+".dat")
    data2 = np.loadtxt("rdme_example.dat")
    x = np.arange(10) + 0.5
    factor = growth(data[-1, 0])

    fig, ax = plt.subplots(figsize=(6, 3.5))
    plt.subplots_adjust(left=0.3, right=0.7, bottom=0.2)
    ax.bar(x*factor, data[-1, 1:], width=0.9*factor, label="Growing domain")
    ax.bar(x, data2[-1, 1:], label="Static domain")
    ax.set_xlabel("Position [cm]")
    ax.set_ylabel("Number of molecules")
    ax.legend()

    save(fig, filename)

if __name__ == "__main__":
    fig1("meshes")
    fig2("cme_example")
    fig3("rdme_example")
    fig4("growing")
