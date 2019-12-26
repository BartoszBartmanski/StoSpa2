
#include <chrono>
#include "simulator.hpp"

namespace ss = StoSpa2;

int main(int argc, char **argv) {
    auto diffusion = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

    // First we create an array of voxels with 10000 molecules in the leftmost one
    std::vector<ss::Voxel> vs(999, ss::Voxel({0}, 0.01));
    vs.insert(vs.begin(), ss::Voxel({10000}, 0.01));

    // We add diffusion reactions to all the voxels
    for (unsigned i=0; i<vs.size()-1; i++) {
        vs[i].add_reaction(ss::Reaction(1.0, diffusion, {-1}, i+1));
        vs[i+1].add_reaction(ss::Reaction(1.0, diffusion, {-1}, i));
    }

    // We create the file for outputting time taken to finish one simulation
    std::ofstream outfile;
    outfile.open(argc > 1 ? std::string(argv[1]) : "benchmarks_diffusion.dat");
    outfile << "# time_taken_in_miliseconds" << std::endl;

    // We run the simulation 10 times and save the time taken each time
    for (unsigned i=0; i<10; i++)
    {
        auto start = std::chrono::system_clock::now();

        ss::Simulator sim(vs);
        sim.advance(100);

        auto end = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        outfile << elapsed.count() << std::endl;
    }
}
