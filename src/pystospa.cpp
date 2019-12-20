
// Pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

// StoSpa2 includes
#include "reaction.hpp"
#include "simulator.hpp"
#include "voxel.hpp"

namespace py = pybind11;
namespace ss = StoSpa2;

// Define a shortcut for defining the propensity function (p_f)
typedef std::function<double (const std::vector<unsigned>&, const double&)> p_f;

PYBIND11_MODULE(pystospa, m) {
    m.doc() = "pybind11 stospa2 plugin"; // optional module docstring
    py::class_<ss::Reaction>(m, "Reaction")
        .def(py::init<double, p_f, std::vector<int>>())
        .def(py::init<double, p_f, std::vector<int>, int>())
        .def("set_rate", &ss::Reaction::set_rate)
        .def("get_rate", &ss::Reaction::get_rate)
        .def("get_propensity", &ss::Reaction::get_propensity);

    py::class_<ss::Voxel>(m, "Voxel")
        .def(py::init<std::vector<unsigned>, double>())
        .def(py::init<std::vector<unsigned>, double, bool>())
        .def("get_molecules", &ss::Voxel::get_molecules)
        .def("get_voxel_size", &ss::Voxel::get_voxel_size)
        .def("add_reaction", &ss::Voxel::add_reaction)
        .def("get_total_propensity", &ss::Voxel::get_total_propensity)
        .def("pick_reaction", &ss::Voxel::pick_reaction)
        .def("update_molecules", &ss::Voxel::update_molecules)
        .def("clear_reactions", &ss::Voxel::clear_reactions);

   py::class_<ss::Simulator>(m, "Simulator")
       .def(py::init<std::vector<ss::Voxel>>())
       .def("set_seed", &ss::Simulator::set_seed)
       .def("get_seed", &ss::Simulator::get_seed)
       .def("get_time", &ss::Simulator::get_time)
       .def("get_voxels", &ss::Simulator::get_voxels)
       .def("get_molecules", &ss::Simulator::get_molecules)
       .def("step", &ss::Simulator::step)
       .def("advance", &ss::Simulator::advance)
       .def("run", &ss::Simulator::run);
}
