
// other header files
#include "reaction.hpp"
#include "voxel.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

namespace StoSpa2 {

void add_diffusion(std::vector<StoSpa2::Voxel>& voxels,
                   std::vector<std::vector<double>> jump_rates,
                   std::function<double (const std::vector<unsigned>&, const double&)> propensity,
                   std::vector<int> stoichiometry_vec) {
    for (unsigned i=0; i<voxels.size(); i++) {
        for (unsigned j=0; j<jump_rates[i].size(); j++) {
            if (i!=j) {
                voxels[i].add_reaction(StoSpa2::Reaction(jump_rates[i][j], propensity, stoichiometry_vec, j));
            }
        }
    }
}

}
