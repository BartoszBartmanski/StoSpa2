
#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

// stl
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>

// other header files
#include "reaction.hpp"
#include "voxel.hpp"

template<typename T>
std::vector<T> operator*(const double& alpha, const std::vector<T>& vec) {
    std::vector<T> output(vec.size());
    for (unsigned i=0; i<vec.size(); i++) {
        output[i] = alpha * vec[i];
    }

    return output;
}

namespace StoSpa2 {

class Simulator {
protected:
    double inf = std::numeric_limits<double>::infinity();

    double m_time;

    std::map<double, unsigned> next_reaction_times;
    std::vector<double> lookup_times;

    std::vector<StoSpa2::Voxel> m_voxels;

    /** Seed used for generating a random number. */
    unsigned m_seed;

    /** For generating random numbers */
    std::mt19937 m_gen;

    /** Uniform distribution. */
    std::uniform_real_distribution<double> m_uniform;

    double exponential(const double& propensity) {
        return (-1.0/propensity) * log(m_uniform(m_gen));
    }

    void initialise_next_reaction_times() {
        // Reset the priority queue if not empty
        if (!next_reaction_times.empty()) {
            next_reaction_times.clear();
            lookup_times.clear();
        }

        // Populate nex reaction times
        for (unsigned i=0; i<m_voxels.size(); i++) {
            double new_time = exponential(m_voxels[i].get_total_propensity());
            next_reaction_times.emplace(std::make_pair(new_time, i));
            lookup_times.emplace_back(new_time);
        }
    }

    void update_next_reaction_time(const unsigned& index) {
        // Calculate the new time until the next reaction for this voxel
        double new_time = m_time + exponential(m_voxels[index].get_total_propensity());

        // Update next_reaction_times
        if (lookup_times[index] < inf) {
            auto nh = next_reaction_times.extract(next_reaction_times.find(lookup_times[index]));
            if (!nh.empty()) {
                nh.key() = new_time;
                next_reaction_times.insert(std::move(nh));
            }
            else {
                throw std::runtime_error("Simulator::update_next_reaction_time: could not find the time!");
            }
        }
        else {
            next_reaction_times.emplace(std::make_pair(new_time, index));
        }

        // Update lookup_times
        lookup_times[index] = new_time;
    }

public:

    explicit Simulator(std::vector<StoSpa2::Voxel> voxels) {
        // For generating random numbers from the uniform dist
        std::random_device rd;
        m_seed = rd();
        m_gen = std::mt19937(m_seed);
        m_uniform = std::uniform_real_distribution<double>(0.0, 1.0);

        // Set the initial time and move the container with the voxels
        m_time = 0;
        m_voxels = std::move(voxels);

        initialise_next_reaction_times();
    }

    void set_seed(unsigned seed) {
        m_seed = seed;
        m_gen = std::mt19937(m_seed);
        initialise_next_reaction_times();
    }

    unsigned get_seed() {
        return m_seed;
    }

    double get_time() {
        return m_time;
    }

    const std::vector<StoSpa2::Voxel>& get_voxels() {
        return m_voxels;
    }

    std::vector<unsigned> get_molecules() {
        std::vector<unsigned> output;
        for (auto& vox : m_voxels) {
            auto mols = vox.get_molecules();
            output.insert(output.end(), mols.begin(), mols.end());
        }
        return output;
    }

    void step() {

        // Pick the smallest time from next_reaction_times
        m_time = (*next_reaction_times.begin()).first;
        auto voxel_idx = (*next_reaction_times.begin()).second;

        m_voxels[voxel_idx].update_properties(m_time);

        if (m_time < inf) {
            // Pick a reaction with the corresponding voxel
            auto& r = m_voxels[voxel_idx].pick_reaction(m_uniform(m_gen));

            // Update the time until the next reaction for this voxel
            m_voxels[voxel_idx].update_molecules(r.stoichiometry);
            update_next_reaction_time(voxel_idx);

            //TODO: what if r.diffusion_idx is larger than number of voxels
            if (r.diffusion_idx >= 0) {
                // Update the number of molecules
                m_voxels[r.diffusion_idx].update_molecules(-1 * r.stoichiometry);
                update_next_reaction_time(r.diffusion_idx);
            }
        }
    }

    void advance(double time_point) {
        while (m_time < time_point) {
            step();
        }
    }

    void write_header(std::ofstream& handle) {
        handle << "# seed = " << m_seed << std::endl;
        handle << "# time voxels (" << m_voxels.size() << ")" << std::endl;
    }

    void write_header(const std::string& filename) {
        std::ofstream handle;
        handle.open(filename);
        write_header(handle);
        handle.close();
    }

    void save(std::ofstream& handle) {
        handle << m_time;
        for (auto& vox : m_voxels) {
            auto mols = vox.get_molecules();
            for (auto& mol : mols) {
                handle << " " << mol;
            }
        }
        handle << std::endl;
    }

    void save(const std::string& filename) {
        std::ofstream handle;
        handle.open(filename, std::ios_base::app);
        save(handle);
        handle.close();
    }

    void run(const std::string& name, double time_step, unsigned num_steps) {
        std::ofstream handle;
        handle.open(name);

        write_header(handle);
        for (unsigned i=0; i<num_steps; i++) {
            advance(time_step * i);
            save(handle);
        }
    }
};

}

#endif // SIMULATOR_HPP
