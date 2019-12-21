
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
    sim.run("example.dat", 0.01, 1000);
}
