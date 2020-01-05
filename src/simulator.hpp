
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

// boost
#include <boost/heap/fibonacci_heap.hpp>

// other header files
#include "reaction.hpp"
#include "voxel.hpp"

typedef std::pair<double, unsigned> d_u;

typedef boost::heap::fibonacci_heap<d_u, boost::heap::compare<std::greater<d_u>>> fib_heap;

namespace StoSpa2 {

class Simulator {
protected:
    double inf = std::numeric_limits<double>::infinity();

    double m_time;

    fib_heap next_reaction_times;
    std::vector<fib_heap::handle_type> handles;

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
            handles.clear();
        }

        // Populate nex reaction times
        for (unsigned i=0; i<m_voxels.size(); i++) {
            double new_time = exponential(m_voxels[i].get_total_propensity());
            handles.emplace_back(next_reaction_times.emplace(std::make_pair(new_time, i)));
        }
    }

    void update_next_reaction_time(const unsigned& index) {
        // Calculate the new time until the next reaction for this voxel
        double new_time = m_time + exponential(m_voxels[index].get_total_propensity());

        // Update next_reaction_times
        *handles[index] = std::make_pair(new_time, index);
        next_reaction_times.update(handles[index]);
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
        m_time = next_reaction_times.top().first;
        auto voxel_idx = next_reaction_times.top().second;

        m_voxels[voxel_idx].update_properties(m_time);

        if (m_time < inf) {
            // Pick a reaction with the corresponding voxel
            auto& r = m_voxels[voxel_idx].pick_reaction(m_uniform(m_gen));

            // Update the time until the next reaction for this voxel
            m_voxels[voxel_idx].add_vector(r.stoichiometry);
            update_next_reaction_time(voxel_idx);

            //TODO: what if r.diffusion_idx is larger than number of voxels
            if (r.diffusion_idx >= 0) {
                // Update the number of molecules
                m_voxels[r.diffusion_idx].subtract_vector(r.stoichiometry);
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
