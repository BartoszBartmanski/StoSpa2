
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

namespace StoSpa2 {

/**
 * Simulator class - used to step in time using stochastic simulation algorithm (SSA). It uses next
 * subvolume method variation of the SSA to efficiently step through time.
 *
 */
class Simulator {
protected:
    /** Infinity - helper variable */
    double inf = std::numeric_limits<double>::infinity();

    /** Current time in a simulation */
    double m_time;

    /** Map of times until next reaction for each voxel to the corresponding indices of voxels */
    std::map<double, unsigned> next_reaction_times;

    /** Vector of times until the next reaction for each voxel ordered according to voxel indices */
    std::vector<double> lookup_times;

    /** Vector of Voxel class instances */
    std::vector<StoSpa2::Voxel> m_voxels;

    /** Seed used for generating a random number. */
    unsigned m_seed;

    /** For generating random numbers */
    std::mt19937 m_gen;

    /** Uniform distribution. */
    std::uniform_real_distribution<double> m_uniform;

    /**
     * Function that returns a random number from the exponential distribution.
     * @param propensity the total propensity
     * @return a random number from exponential distribution
     */
    double exponential(const double& propensity) {
        return (-1.0/propensity) * log(m_uniform(m_gen));
    }

    /**
     * Initialiases all the times until next reactions in all the containers
     */
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

    /**
     * Updates the time until the next reaction for a voxel with the given index
     * @param index the index of the voxel where time until the next reaction is to be updated
     */
    void update_next_reaction_time(const unsigned& index) {
        // Calculate the new time until the next reaction for this voxel
        double new_time = m_time + exponential(m_voxels[index].get_total_propensity());

        // Update next_reaction_times
        next_reaction_times.erase(lookup_times[index]);
        next_reaction_times.emplace(std::make_pair(new_time, index));

        // Update lookup_times
        lookup_times[index] = new_time;
    }

public:

    /**
     * Constructor for the Simulator class
     * @param voxels vector of Voxel class instances
     */
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

    /**
     * Sets the seed in the random number generator
     * @param seed the value of the seed
     */
    void set_seed(unsigned seed) {
        m_seed = seed;
        m_gen = std::mt19937(m_seed);
        initialise_next_reaction_times();
    }

    /**
     * Returns the number used to generate the random numbers
     */
    unsigned get_seed() {
        return m_seed;
    }

    /**
     * Returns the current time in the simulation
     */
    double get_time() {
        return m_time;
    }

    /**
     * Returns a copy of the vector of voxels used in the simulation
     */
    const std::vector<StoSpa2::Voxel>& get_voxels() {
        return m_voxels;
    }

    /**
     * Returns the number of molecules contained in each voxel as a single vector
     */
    std::vector<unsigned> get_molecules() {
        std::vector<unsigned> output;
        for (auto& vox : m_voxels) {
            auto mols = vox.get_molecules();
            output.insert(output.end(), mols.begin(), mols.end());
        }
        return output;
    }

    /**
     * Function to make a single step in the SSA
     */
    void step() {

        // Pick the smallest time from next_reaction_times
        m_time = (*next_reaction_times.begin()).first;
        auto voxel_idx = (*next_reaction_times.begin()).second;

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

    /**
     * Function to make multiple steps to reach the given point in time
     * @param time_point the point in time in simulation that is reached
     */
    void advance(double time_point) {
        while (m_time < time_point) {
            step();
        }
    }

    /**
     * Function that writes a given string that describes the simulation
     * @param handle reference to the output stream to a file
     * @param header string that is to be written to a file
     */
    void write_header(std::ofstream& handle, const std::string& header="# time voxels...\n") {
        handle << header;
    }

    /**
     * Function that writes a given string that describes the simulation
     * @param filename path to the file
     * @param header string that is to be written to a file
     */
    void write_header(const std::string& filename, const std::string& header="# time voxels...\n") {
        std::ofstream handle;
        handle.open(filename);
        write_header(handle, header);
        handle.close();
    }

    /**
     * Function that saves the number of molecules present in each voxel to a file
     * @param handle reference to the output stream to a file
     */
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

    /**
     * Function that saves the number of molecules present in each voxel to a file
     * @param filename path to the file
     */
    void save(const std::string& filename) {
        std::ofstream handle;
        handle.open(filename, std::ios_base::app);
        save(handle);
        handle.close();
    }

    /**
     * Function that runs a simulation and saves the number of molecules present in each
     * voxel at each time-point to the given file.
     * @param name path to the file
     * @param time_step value of the step in time
     * @param num_steps number of steps in time which to take
     * @param header string of information that describes the simulation
     */
    void run(const std::string& name, double time_step, unsigned num_steps, const std::string& header="# time voxels...\n") {
        std::ofstream handle;
        handle.open(name);

        write_header(handle, header);
        for (unsigned i=0; i<num_steps; i++) {
            advance(time_step * i);
            save(handle);
        }
    }
};

}

#endif // SIMULATOR_HPP
