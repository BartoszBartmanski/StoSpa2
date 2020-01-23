
Usage
#####

First, we will walk through some C++ code for running simulations, followed by a
few examples of running simulations in Python.

C++ examples
============

Chemical master equation
------------------------

As first example, let us consider the following chemical reaction

    .. math::

        A \rightarrow \emptyset

happening at some rate `k`. We can simulate this chemical system with the following code

    .. code-block:: c++

        #include "simulator.hpp"

        int main() {
            auto decay = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

            StoSpa2::Voxel v({100}, 1.0);
            StoSpa2::Reaction r(1.0, decay, {-1});

            v.add_reaction(r);

            StoSpa2::Simulator sim({v});
            sim.run("example.dat", 0.01, 500);
        }

First line of code in the above example makes sure that we can use StoSpa2 classes. Then in the main function
we first define the lambda function that returns the propensity (with the rate factored out)

    .. code-block:: c++

        auto decay = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

and the lambda functions for the propensities have to take two arguments: a vector and a double. The vector
representing number of molecules and the double representing the area of a voxel, these two arguments will be passed
within the Voxel class. Next, we create both voxel and reactions objects

    .. code-block::

        StoSpa2::Voxel v({100}, 1.0);
        StoSpa2::Reaction r(1.0, decay, {-1});

where we place `10000` molecules of species `A` into a domain of size `1` (which in our case is a single
voxel of size `1.0`). Then we assign the reaction object a rate of `k = 1.0`, the propensity function `decay`
as defined before and the stoichiometry vector which decreases the number of molecules by one any time
that the decay reaction happens. We then add the reaction object we just created to the voxel object,
and then run the simulation, which saves the output into `example.dat` file every `0.01` for `500` steps.

    .. code-block:: c++

        v.add_reaction(r);

        StoSpa2::Simulator sim({v});
        sim.run("example.dat", 0.01, 500);

We can plot the output of the simulation to get

    .. image:: images/cme_example.svg
      :width: 400
      :align: center
      :alt: CME Example

Reaction-diffusion master equation example
------------------------------------------

To show how the StoSpa2 can be used to run simulations within reaction-diffusion master equation framework
we use the example of a one-dimensional domain :math:`[0, 10]`, discretised into 10 voxels and we model the diffusion
as a jump process

    .. math::

        A_1 \leftrightarrow A_2 \leftrightarrow A_3 \leftrightarrow ... \leftrightarrow A_{10}

where :math:`A_i` is the the number of moleules in voxel :math:`i`. The C++ code for such a system is as follows

    .. code-block:: c++

        #include "simulator.hpp"

        using namespace StoSpa2;

        int main() {
            auto diffusion = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

            // First we create an array of voxels with 10000 molecules in the leftmost one
            std::vector<Voxel> vs = std::vector<Voxel>(9, Voxel({0}, 1.0));
            vs.insert(vs.begin(), Voxel({10000}, 1.0));

            // We add diffusion reactions to all the voxels
            for (unsigned i=0; i<vs.size()-1; i++) {
                vs[i].add_reaction(Reaction(1.0, diffusion, {-1}, i+1));
                vs[i+1].add_reaction(Reaction(1.0, diffusion, {-1}, i));
            }

            // We create a simulator instance and run the simulation.
            Simulator sim(vs);
            sim.run("example.dat", 0.01, 500);
        }

which is included in the `src` directory of StoSpa2.

The code is somewhat similar to the chemical master equation example, except we have a few more voxel objects.
We also have the extra line :code:`using namespace StoSpa2` to save us having to write :code:`StoSpa2::` in
front of every StoSpa2 class.

The first line within the main function, defines the propensity function for the diffusion of molecules as
a jump process between two adjacent voxels (again with rate factored out as it is given as a separate parameter
of the Reaction class constructor).

    .. code-block:: c++

        auto diffusion = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

Then, we create a vector of Voxel objects, with one of them having 10000 molecules

    .. code-block:: c++

        std::vector<Voxel> vs = std::vector<Voxel>(9, Voxel({0}, 1.0));
        vs.insert(vs.begin(), Voxel({10000}, 1.0));

and then add reactions to the voxels where we assume that these voxels are ordered by their position on the
`x`-axis. When adding the diffusion reactions, we have one additional parameter in the Reaction class constructors,
namely :code:`diffusion_idx`, which is the index of a neighbouring voxel to which a molecule jumps to if a diffusion
reaction were to happen

    .. code-block:: c++

        for (unsigned i=0; i<vs.size()-1; i++) {
            vs[i].add_reaction(Reaction(1.0, diffusion, {-1}, i+1));
            vs[i+1].add_reaction(Reaction(1.0, diffusion, {-1}, i));
        }

And finally, as in the previous example, we run the simulation with the Simulator class instance by
passing the vector of Voxel objects to it and calling Simulator class run function.

    .. code-block:: c++

        Simulator sim(vs);
        sim.run("example.dat", 0.01, 500);

We can plot the output of the simulation

    .. image:: images/rdme_example.svg
      :width: 800
      :align: center
      :alt: RDME Example


Python examples
===============

Now to show how to run simulations using python binding of StoSpa2, pystospa. For completeness and to show that
the interface is very similar, we will cover the C++ examples shown.

Chemical master equation
------------------------

Again, we consider the following chemical reaction

    .. math::

        A \rightarrow \emptyset

happening at rate `k`. The python code for this simulation (that is equivalent to the C++ example) is as follows

    .. code-block:: python

        import pystospa as ss

        v = ss.Voxel([100], 1.0)
        r = ss.Reaction(1.0, lambda x, y : x[0], [-1])
        v.add_reaction(r)

        s = ss.Simulator([v])
        s.run("example.dat", 0.01, 500)

This code is very similar to the C++ counterpart:

    1. we import StoSpa2,
    2. we create Voxel and Reaction objects,
    3. we add reaction,
    4. we create Simulator object and run the simulation.


Reaction-diffusion master equation
----------------------------------

For a simple example of a simulation within RDME framework, we again consider a one-dimensional domain :math:`[0, 10]`,
discretised into 10 voxels and we model the diffusion as a jump process

    .. math::

        A_1 \leftrightarrow A_2 \leftrightarrow A_3 \leftrightarrow ... \leftrightarrow A_{10}

where :math:`A_i` is the the number of moleules in voxel :math:`i`. Then the python code to simulate this system is as
follows

    .. code-block:: python

        import pystospa as ss

        domain = [ss.Voxel([10000], 1.0)] + [ss.Voxel([0], 1.0) for i in range(9)]

        for i in range(len(domain)-1):
            domain[i].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i+1))
            domain[i+1].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i))

        s = ss.Simulator(domain)
        s.run("example.dat", 0.01, 500)

which simulates the given system in the same way as the corresponding C++ example.

Schnakenberg
------------

For a more complicated example, let us consider a simulation of pattern formation. A common example of pattern
formation system is the Schnakenberg system of reactions given by

    .. math::

        A \rightarrow \emptyset, \emptyset \rightarrow A, 2A + B \rightarrow 3A, \emptyset \rightarrow B

as well as diffusion of species `A` with diffusion coefficient :math:`D_A = 0.00001` and diffusion of species B
with diffusion coefficient :math:`D_B = 0.001`

    .. code-block:: python

        import pystospa as ss

        # Define the simulation parameters
        n, l = 40, 1.0  # num voxels, domain size
        h = l/n  # voxel size
        da, da = 1e-5, 0.001  # diffusion coefficients
        k1, k2, k3, k4 = 0.02, 40.0, 6.25e-10, 120.0  # rate constants

        # Make a list of voxels
        domain = [ss.Voxel([200, 75], h) for i in range(n)]

        # Add diffusion of both species
        for i in range(n-1):
            domain[i].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[0], [-1, 0], i+1))
            domain[i+1].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[0], [-1, 0], i))
            domain[i].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[1], [0, -1], i+1))
            domain[i+1].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[1], [0, -1], i))

        # Add all the other reactions
        for i in range(n):
            domain[i].add_reaction(ss.Reaction(k1, lambda x, y : x[0], [-1, 0]))  # decay of A
            domain[i].add_reaction(ss.Reaction(k2, lambda x, y : y, [1, 0]))  # prod of A
            domain[i].add_reaction(ss.Reaction(k3, lambda x, y : x[0] * (x[0] - 1) * x[1] / (y**2), [1, -1]))  #schnakenberg
            domain[i].add_reaction(ss.Reaction(k4, lambda x, y : y, [0, 1]))  # prod of B

        # Runt the simulation
        s = ss.Simulator(domain)
        s.run("example.dat", 10, 180)

The code follows similar structure of the previous examples, except here we first define all the simulation
parameters

    .. code-block:: python

        n, l = 40, 1.0  # num voxels, domain size
        h = l/n  # voxel size
        da, da = 1e-5, 0.001  # diffusion coefficients
        k1, k2, k3, k4 = 0.02, 40.0, 6.25e-10, 120.0  # rate constants

then we create a list of Voxel objects

    .. code-block:: python

        domain = [ss.Voxel([200, 75], h) for i in range(n)]

and the we add the diffusion of both species

    .. code-block:: python

        for i in range(n-1):
            domain[i].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[0], [-1, 0], i+1))
            domain[i+1].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[0], [-1, 0], i))
            domain[i].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[1], [0, -1], i+1))
            domain[i+1].add_reaction(ss.Reaction(da/(h**2), lambda x, y : x[1], [0, -1], i))

After which we add all the other reactions present within this system

    .. code-block:: python

        for i in range(n):
            domain[i].add_reaction(ss.Reaction(k1, lambda x, y : x[0], [-1, 0]))  # decay of A
            domain[i].add_reaction(ss.Reaction(k2, lambda x, y : y, [1, 0]))  # prod of A
            domain[i].add_reaction(ss.Reaction(k3, lambda x, y : x[0] * (x[0] - 1) * x[1] / (y**2), [1, -1]))  #schnakenberg
            domain[i].add_reaction(ss.Reaction(k4, lambda x, y : y, [0, 1]))  # prod of B

Finally, we run the simulation until time `1800 s`, saving every `10 s`

    .. code-block:: python

        s = ss.Simulator(domain)
        s.run("example.dat", 10, 180)

which gives us the following output

    .. image:: images/schnakenberg_example.svg
      :width: 400
      :align: center
      :alt: Schnakenberg Example


Growing domain
--------------

StoSpa2 also allows for stochastic simulations on a growing domain.

Let us consider the system from reaction-diffusion master equation section, where we have a single species, `A`,
that is diffusing on a domain :math:`\Omega(t) = [0, L(t)]`, where :math:`L(t) = L_0 e^{rt}`. Then the python
code for this simulation, assuming that :math:`L_0 = 10` and :math:`r = 0.2`, is as follows

    .. code-block:: python

        from math import exp
        import pystospa as ss

        # Domain growth function
        growth = lambda t : exp(0.2 * t)

        # Set up a list of voxels
        domain = [ss.Voxel([10000], 1.0, growth)] + [ss.Voxel([0], 1.0, growth) for i in range(9)]

        # Add diffusion
        for i in range(len(domain)-1):
            domain[i].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i+1))
            domain[i+1].add_reaction(ss.Reaction(1.0, lambda x, y : x[0], [-1], i))

        # Run the simulation
        s = ss.Simulator(domain)
        s.run("example.dat", 0.01, 500)

The main difference between the above code and the code from reaction-diffusion master equation section
is that we add a lambda function for the growth of the domain to the constructor of the voxels

    .. code-block:: python

        ss.Voxel([10000], 1.0, growth)

where :code:`growth = lambda t : exp(0.2 * t)`.

Below, we show the difference between the a simulation on a static domain and a growing domain

    .. image:: images/growing_domain_example.svg
      :width: 400
      :align: center
      :alt: Growing domain example
