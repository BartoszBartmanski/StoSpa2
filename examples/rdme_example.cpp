
#include "simulator.hpp"

using namespace StoSpa2;

int main() {

    // Create a vector of voxel objects. Voxel arguments: vector of number of molecules, size of the voxel
    std::vector<unsigned> initial_num = {10000};
    double voxel_size = 1.0;
    // First create nine empty voxels
    std::vector<Voxel> vs = std::vector<Voxel>(9, Voxel({0}, voxel_size));
    // Then add the non-empty voxel at the beginning
    vs.insert(vs.begin(), Voxel(initial_num, voxel_size));

    double d = 1.0;  // diffusion rate
    auto propensity = [](const std::vector<unsigned>& num_mols, const double& area) { return num_mols[0]; };
    std::vector<int> stoch = {-1};
    // Create and add the reaction objects
    for (unsigned i=0; i<vs.size()-1; i++) {
        // Add diffusion jump to the right from voxel i to voxel i+1
        vs[i].add_reaction(Reaction(d, propensity, stoch, i+1));
        // Add diffusion jump to the left from voxel i+1 to voxel i
        vs[i+1].add_reaction(Reaction(d, propensity, stoch, i));
    }

    // Pass the voxels with the reaction(s) to the simulator object
    Simulator s(vs);

    // Run the simulation. Arguments: path to output file, time step, number of steps
    s.run("rdme_example.dat", 0.01, 500);
}
