
// Pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

// StoSpa2 includes
#include "reaction.hpp"
#include "simulator.hpp"
#include "voxel.hpp"

namespace py = pybind11;

// Define a shortcut for defining the propensity function (p_f)
typedef std::function<double (const std::vector<unsigned>&, const double&)> p_f;

PYBIND11_MODULE(stospa2py, m) {
    m.doc() = "pybind11 stospa2 plugin"; // optional module docstring
    py::class_<Reaction>(m, "Reaction")
        .def(py::init<double, p_f, std::vector<int>>())
        .def(py::init<double, p_f, std::vector<int>, int>())
        .def("set_rate", &Reaction::set_rate)
        .def("get_rate", &Reaction::get_rate)
        .def("get_propensity", &Reaction::get_propensity)
        .def("__repr__", [](const Reaction &r) { return "<reaction.Reaction object>"; });

    py::class_<Voxel>(m, "Voxel")
        .def(py::init<std::vector<unsigned>, double>())
        .def(py::init<std::vector<unsigned>, double, bool>())
        .def("get_molecules", &Voxel::get_molecules)
        .def("get_voxel_size", &Voxel::get_voxel_size)
        .def("add_reaction", &Voxel::add_reaction)
        .def("get_total_propensity", &Voxel::get_total_propensity)
        .def("pick_reaction", &Voxel::pick_reaction)
        .def("update_molecules", &Voxel::update_molecules)
        .def("clear_reactions", &Voxel::clear_reactions);

   py::class_<Simulator>(m, "Simulator")
       .def(py::init<std::vector<Voxel>>())
       .def("set_seed", &Simulator::set_seed)
       .def("get_seed", &Simulator::get_seed)
       .def("get_time", &Simulator::get_time)
       .def("get_voxels", &Simulator::get_voxels)
       .def("step", &Simulator::step)
       .def("advance", &Simulator::advance)
       .def("run", &Simulator::run);
}
