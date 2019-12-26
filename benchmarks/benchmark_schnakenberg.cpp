
#include <chrono>
#include "simulator.hpp"

namespace ss = StoSpa2;

int main(int argc, char** argv) {
    auto decay = [](const std::vector<unsigned>& mols, const double& area) { return (double)mols[0]; };
    auto prod = [](const std::vector<unsigned>& mols, const double& area) { return area; };
    auto diffusion1 = [](const std::vector<unsigned>& mols, const double& area) { return (double)mols[0]; };
    auto diffusion2 = [](const std::vector<unsigned>& mols, const double& area) { return (double)mols[1]; };
    auto schnakenberg = [](const std::vector<unsigned>& mols, const double& area) { return mols[0] * (mols[0] - 1) * mols[1] / (area*area); };

    double du = 1e-5;
    double dv = 0.001;
    double k1 = 0.02;  // decay of species 1
    double k2 = 40.0;  // production of species 1
    double k3 = 6.25e-10;  // schnakenberg reaction
    double k4 = 120.0;  // production of species 2
    unsigned n = 40;  // number of voxels
    double h = 1.0 / n;

    std::vector<ss::Voxel> vs(40, ss::Voxel({200, 75}, h));

    for (unsigned i=0; i<vs.size()-1; i++) {
        vs[i].add_reaction(ss::Reaction(du/(h*h), diffusion1, {-1, 0}, i+1));
        vs[i+1].add_reaction(ss::Reaction(du/(h*h), diffusion1, {-1, 0}, i));
        vs[i].add_reaction(ss::Reaction(dv/(h*h), diffusion2, {0, -1}, i+1));
        vs[i+1].add_reaction(ss::Reaction(dv/(h*h), diffusion2, {0, -1}, i));
    }

    for (auto& v : vs) {
        v.add_reaction(ss::Reaction(k1, decay, {-1, 0}));
        v.add_reaction(ss::Reaction(k2, prod, {1, 0}));
        v.add_reaction(ss::Reaction(k3, schnakenberg, {1, -1}));
        v.add_reaction(ss::Reaction(k4, prod, {0, 1}));
    }

    // We create the file for outputting time taken to finish one simulation
    std::ofstream outfile;
    outfile.open(argc > 1 ? std::string(argv[1]) : "benchmarks_schnakenberg.dat");
    outfile << "# time_taken_in_miliseconds" << std::endl;

    // We run the simulation 10 times and save the time taken each time
    for (unsigned i=0; i<10; i++)
    {
        auto start = std::chrono::system_clock::now();

        ss::Simulator sim(vs);
        sim.advance(2000);

        auto end = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        outfile << elapsed.count() << std::endl;
    }

}
