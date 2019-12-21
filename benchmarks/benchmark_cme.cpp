
#include <chrono>
#include "simulator.hpp"

namespace ss = StoSpa2;

int main(int argc, char **argv) {
    // We define the std::functions for reactions
    auto decay = [](const std::vector<unsigned>& mols, const double& area) { return (double)mols[0]; };
    auto prod = [](const std::vector<unsigned>& mols, const double& area) { return area; };

    ss::Voxel vox({100}, 1.0);
    vox.add_reaction(ss::Reaction(0.01, decay, {-1}));  // decay reaction
    vox.add_reaction(ss::Reaction(1.0, prod, {1}));  // production reaction

    // We create the file for outputting time taken to finish one simulation
    std::ofstream outfile;
    outfile.open(argc > 1 ? std::string(argv[1]) : "benchmarks_cme.dat");
    outfile << "# time_taken_in_miliseconds" << std::endl;

    // We run the simulation 10 times and save the time taken each time
    for (unsigned i=0; i<10; i++)
    {
        auto start = std::chrono::system_clock::now();

        ss::Simulator sim({vox});
        sim.advance(1000000);

        auto end = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        outfile << elapsed.count() << std::endl;
    }
}
