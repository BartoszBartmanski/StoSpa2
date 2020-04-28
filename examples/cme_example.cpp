
#include "simulator.hpp"

int main() {
    // Create voxel object. Arguments: vector of number of molecules, size of the voxel
    std::vector<unsigned> initial_num = {100};  // number of molecules of species A
    double domain_size = 10.0;  // size of the domain in cm
    StoSpa2::Voxel v(initial_num, domain_size);

    // Create reaction object.
    // Arguments: reaction rate, propensity func, stoichiometry vector
    double k = 1.0;
    auto propensity = [](const std::vector<unsigned>& num_mols, const double& area) { return num_mols[0]; };
    std::vector<int> stoch = {-1};
    StoSpa2::Reaction r(k, propensity, stoch);

    // Add a reaction to a voxel
    v.add_reaction(r);

    // Pass the voxel with the reaction(s) to the simulator object
    StoSpa2::Simulator s({v});

    // Run the simulation. Arguments: path to output file, time step, number of steps
    s.run("cme_example.dat", 0.01, 500);
}
