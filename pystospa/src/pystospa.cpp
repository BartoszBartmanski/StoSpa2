
// Pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

// StoSpa2 includes
#include "reaction.hpp"
#include "simulator.hpp"
#include "voxel.hpp"
#include "version.hpp"

namespace py = pybind11;
namespace ss = StoSpa2;

PYBIND11_MODULE(pystospa, m) {
    m.attr("__version__") = PROJECT_VERSION;
    py::class_<ss::Reaction>(m, "Reaction", R"pbdoc(
        pystospa.Reaction(rate, propensity_func, stoichimetry, diff_idx=-1)

        Reaction class constructor

        Parameters:

        - rate = rate of the reaction
        - propensity_func = lambda function that takes number of molecules and voxel are and returns propensity
        - stoichiometry = vector on how number of molecules are going to change if this reaction happens
        - diff_idx = index of a voxel in an list of voxels to which the molecule should jump to
    )pbdoc")
        .def(py::init<double, p_f, std::vector<int>>())
        .def(py::init<double, p_f, std::vector<int>, int>())
        .def("set_rate", &ss::Reaction::set_rate, py::arg("rate"), R"pbdoc(
            Sets the rate of the reaction

            Parameters:

            - rate = value to which rate needs to be set
        )pbdoc")
        .def("get_rate", &ss::Reaction::get_rate, R"pbdoc(
            Returns the rate of the reaction

            Returns:

            - value of the reaction rate
        )pbdoc")
        .def("get_propensity", &ss::Reaction::get_propensity, py::arg("num_molecules"), py::arg("voxel_size"),
        R"pbdoc(
            Returns propensity of the reaction given number of molecules and voxel size

            Parameters:

            - num_molecules = list of number of molecules
            - voxel_size = size (length/area/volume) of a voxel

            Returns:

            - propensity
        )pbdoc");

    py::class_<ss::Voxel>(m, "Voxel", R"pbdoc(
        pystospa.Voxel(num_molecules, voxel_size, growth_func=None, extrande_ratio=2.0)

        Voxel class constructor

        Parameters:

        - num_molecules = array of number of molecules of each species
        - voxel_size = size (len/area/volume) of a voxel
        - growth_func = lambda function that takes value of time and returns ratio by which voxel size has grown
        - extrande_ratio = by how much the total propensity needs to be multiplied to get an upper bound
    )pbdoc")
        .def(py::init<std::vector<unsigned>, double>())
        .def(py::init<std::vector<unsigned>, double, g_f>())
        .def(py::init<std::vector<unsigned>, double, g_f, double>())
        .def(py::init<std::vector<unsigned>, double, std::vector<g_f>>())
        .def(py::init<std::vector<unsigned>, double, std::vector<g_f>, double>())
        .def("get_molecules", &ss::Voxel::get_molecules, R"pbdoc(
            Returns the number of molecules present in the voxel

            Returns:

            - number of molecules
        )pbdoc")
        .def("get_voxel_size", &ss::Voxel::get_voxel_size, R"pbdoc(
            Returns size of the voxel

            Returns:

            - voxel size
        )pbdoc")
        .def("get_extrande_ratio", &ss::Voxel::get_extrande_ratio, R"pbdoc(
            Returns value of the extrande ratio

            Returns:

            - extrande ratio
        )pbdoc")
        .def("add_reaction", &ss::Voxel::add_reaction, py::arg("reaction"),
        R"pbdoc(
            Adds the given reaction to the list of reactions contained within the voxel

            Parameters:

            - reaction = an instance of Reaction class
        )pbdoc")
        .def("get_reactions", &ss::Voxel::get_reactions, R"pbdoc(
            Returns list of reactions contained within the voxel

            Returns:

            - list of reactions
        )pbdoc")
        .def("get_total_propensity", &ss::Voxel::get_total_propensity, py::arg("update")=true,
        R"pbdoc(
            Returns the total propensity given all the reactions within the voxel

            Parameters:

            - update = whether to update Voxel attribute for total propensity

            Returns:

            - total propensity
        )pbdoc")
        .def("clear_reactions", &ss::Voxel::clear_reactions,
        R"pbdoc(
            Empties the list of reactions contained within the voxel
        )pbdoc");

   py::class_<ss::Simulator>(m, "Simulator", R"pbdoc(
       pystospa.Simulator(voxels)

       Simulator class constructor

       Parameters:

       - voxels = list of voxel objects already populated with molecules
   )pbdoc")
       .def(py::init<std::vector<ss::Voxel>>())
       .def("set_seed", &ss::Simulator::set_seed, py::arg("seed"),
       R"pbdoc(
           Sets the number used as the seed for random number generation

           Parameters:

           - seed = a number
       )pbdoc")
       .def("get_seed", &ss::Simulator::get_seed,
       R"pbdoc(
           Returns the number used as the seed for random number generation

           Returns:

           - seed
       )pbdoc")
       .def("get_time", &ss::Simulator::get_time,
       R"pbdoc(
           Returns the current time of the simulation

           Returns:

           - current time of the simulation
       )pbdoc")
       .def("get_voxels", &ss::Simulator::get_voxels,
       R"pbdoc(
           Returns list of voxels that comprise the simulation domain

           Returns:

           - list of voxel object present in the Simulator object
       )pbdoc")
       .def("get_molecules", &ss::Simulator::get_molecules,
       R"pbdoc(
           Returns a concatenated list of number of molecules present in each voxel

           Returns:

           - number of molecules present in the whole simulation
       )pbdoc")
       .def("step", &ss::Simulator::step,
       R"pbdoc(
           Makes a single step in the stochastic simulation algorithm
       )pbdoc")
       .def("advance", &ss::Simulator::advance, py::arg("time_point"),
       R"pbdoc(
           Advances the simulation to the specified time point
       )pbdoc")
       .def("run", &ss::Simulator::run,
            py::arg("name"),
            py::arg("time_step"),
            py::arg("num_steps"),
            py::arg("header")="# time voxels...\n",
        R"pbdoc(
            Runs the simulation given and saves the output in a file

            Parameters:

            - name = name of the file where to save the output of the simulation
            - time_step = how far to advance in time before saving the state of the simulation
            - num_steps = number of steps in time to take
            - header = the string which to write at the top of the file
        )pbdoc");
}
