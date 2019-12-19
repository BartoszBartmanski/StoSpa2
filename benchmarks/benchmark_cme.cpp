
#include <chrono>
#include "simulator.hpp"

int main(int argc, char **argv) {
    // We define the std::functions for reactions
    auto decay = [](const std::vector<unsigned>& mols, const double& area) { return (double)mols[0]; };
    auto prod = [](const std::vector<unsigned>& mols, const double& area) { return area; };

    // We create the file for outputting time taken to finish one simulation
    std::string fname = "benchmarks.dat";
    if (argc > 1) {
        fname = std::string(argv[1]);
    }
    std::ofstream outfile;
    outfile.open(fname);
    outfile << "# time_taken_in_miliseconds";

    // We run the simulation 10 times and save the time taken each time
    for (unsigned i=0; i<10; i++)
    {
        auto start = std::chrono::system_clock::now();

        Voxel vox({100}, 1.0);
        vox.add_reaction(Reaction(0.01, decay, {-1}));  // decay reaction
        vox.add_reaction(Reaction(1.0, prod, {1}));  // production reaction
        Simulator sim({vox});
        sim.advance(100000000);

        auto end = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        outfile << elapsed.count() << std::endl;
    }
}
