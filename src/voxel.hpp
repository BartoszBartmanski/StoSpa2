
// To simulate growth of voxels, we implemented the extrande method (Voliotis M, Thomas P, Grima R, Bowsher CG (2016)
// Stochastic Simulation of Biomolecular Networks in Dynamic Environments. PLoS Comput Biol 12(6): e1004923.
// https://doi.org/10.1371/journal.pcbi.1004923)

#ifndef VOXEL_HPP
#define VOXEL_HPP

// stl
#include <vector>
#include <iostream>

// other header files
#include "reaction.hpp"

// Typedef for the growth function
typedef std::function<double (const double&)> g_f;

namespace StoSpa2 {

/**
 * Voxel class - represents a voxel (or compartment, i.e. a subinterval or subarea of a domain)
 * in stochastic modelling. A voxel in stochastic modelling contains some number of molecules of
 * different number of species and this information is stored as a vector and each element in this vector
 * represents the number of molecules of some species.
 */
class Voxel {
protected:
    /** Size of the voxel (length/area/volume) */
    double m_voxel_size;

    /** Total propensity for all the reactions within a voxel */
    double a_0;

    /** Vector of the number of molecules */
    std::vector<unsigned> m_molecules;

    /** Vector of reactions within a voxel */
    std::vector<StoSpa2::Reaction> m_reactions;

    /** Container for an extrande reaction if needed */
    std::vector<StoSpa2::Reaction> m_extrande_reaction;

    /** Ratio of the upper bound for total propensity and the total propensity */
    double m_extrande_ratio;

    /** Initial voxel size */
    double m_initial_voxel_size;

    /** Function for how voxel size changes */
    std::vector<std::function<double (const double&)>> m_growth_func;

    /** Whether the voxel is growing or not */
    bool m_growing;

public:

    /**
     * Constructor for the Voxel class
     * @param initial_num vector of the number of molecules initially for all species
     * @param voxel_size intial size of the voxel
     */
    Voxel(std::vector<unsigned> initial_num, double voxel_size) {
        // We define the appropriate member variables
        m_voxel_size = voxel_size;
        m_initial_voxel_size = voxel_size;
        m_molecules = std::move(initial_num);

        // Since no growth function is given the voxel is assumed to be of static size
        m_growing = false;
        auto one = [](const double& time) { return 1.0; };
        m_growth_func.emplace_back(one);
    }

    /**
     * Constructor for the Voxel class
     * @param initial_num vector of the number of molecules initially for all species
     * @param voxel_size intial size of the voxel
     * @param growth lambda function that describes how a voxel grows
     * @param extrande_ratio ratio between upper bound for total propensity and total propensity
     */
    Voxel(std::vector<unsigned> initial_num,  double voxel_size, g_f growth, double extrande_ratio=2.0) {
        // We define the appropriate member variables
        m_voxel_size = voxel_size;
        m_initial_voxel_size = voxel_size;
        m_molecules = std::move(initial_num);

        // Since growth function (argument growth) is given, the voxel size is changing,
        // hence we set the member variables associated with voxel growth
        m_growing = true;
        m_growth_func.emplace_back(std::move(growth));

        // Since the size of the voxel is changing, we need to use the extrande method in the stochastic simulations
        if (extrande_ratio < 1.0) {
            throw std::runtime_error("Voxel::Voxel: extrande_ratio needs to greater than 1.0");
        }
        m_extrande_ratio = extrande_ratio;
        add_extrande();
    }

    /**
     * Constructor for the Voxel class
     * @param initial_num vector of the number of molecules initially for all species
     * @param voxel_size intial size of the voxel
     * @param growth vector of lambda functions that describe how a voxel grows (one for each spatial dimension)
     * @param extrande_ratio ratio between upper bound for total propensity and total propensity
     */
    Voxel(std::vector<unsigned> initial_num,  double voxel_size, std::vector<g_f> growth, double extrande_ratio=2.0) {
        // We define the appropriate member variables
        m_voxel_size = voxel_size;
        m_initial_voxel_size = voxel_size;
        m_molecules = std::move(initial_num);

        // Since growth function (argument growth) is given, the voxel size is changing,
        // hence we set the member variables associated with voxel growth
        m_growing = true;
        m_growth_func = std::move(growth);

        // Since the size of the voxel is changing, we need to use the extrande method in the stochastic simulations
        if (extrande_ratio < 1.0) {
            throw std::runtime_error("Voxel::Voxel: extrande_ratio needs to greater than 1.0");
        }
        m_extrande_ratio = extrande_ratio;
        add_extrande();
    }

    /**
     * Returns number of molecules for each species present in a voxel
     * @return copy of m_molecules member variable
     */
    std::vector<unsigned> get_molecules() {
        return m_molecules;
    }

    /**
     * Returns current voxel size
     * @return copy of m_voxel_size member variable
     */
    double get_voxel_size() {
        return m_voxel_size;
    }

    /**
     * Returns whether the voxel is growing or not
     * @return copy of m_growing member variable
     */
    bool is_growing() {
        return m_growing;
    }

    /**
     * Returns the ratio that is used in the extrande method
     * @return copy of m_extrande_ratio member variable
     */
    double get_extrande_ratio() {
        return m_extrande_ratio;
    }

    /**
     * Updates any properties that need to updated due to growth of the voxel
     * @param time current time of the simulation
     */
    void update_properties(const double& time) {
        // If the voxel is growing, then we need to update some properties
        if (m_growing) {
            // For growth in each dimension, multiply together the factors
            // to get how much the length / area has increased
            double new_factor = 1.0;
            for (auto& growth_func : m_growth_func) {
                new_factor *= growth_func(time);
            }
            m_voxel_size = new_factor * m_initial_voxel_size;

            // Then need to update the rate of diffusion of molecules
            double diff_factor = 1.0 / (m_growth_func.size() == 1 ? new_factor * new_factor : new_factor);

            for (auto& reaction : m_reactions) {
                reaction.update_properties(diff_factor);
            }
        }
    }

    /**
     * Adds a reaction (none -> none) that is essential in the extrande method
     */
    void add_extrande() {
        // Define the lambda function that will be the propensity function for for the extrande reaction
        auto constant_func = [](const std::vector<unsigned>& mols, const double& area) { return 1.0; };
        // Then if the container for the extrande reaction is empty add the extrande reaction
        if (m_extrande_reaction.empty()) {
            m_extrande_reaction.emplace_back(StoSpa2::Reaction(0.0, constant_func, {0}));
        }
    }

    /**
     * Adds a Reaction object to a vector of reactions (m_reactions member variable)
     */
    void add_reaction(StoSpa2::Reaction r) {
        // Check that the stoichiometry vector for this reaction is of the right size
        if (r.stoichiometry.size() != m_molecules.size()) {
            std::string m = "Voxel::add_reaction: r.stoichiometry.size() != m_molecules.size()";
            throw std::runtime_error(m);
        }

        // If the reaction rate is greater than zero add it to the vector of reactions
        // Reactions with zero rate are not added for efficiency purposes
        if (r.get_rate() > 0) {
            m_reactions.push_back(r);
        }
    }

    /**
     * Returns the vector of Reaction objects
     * @return copy of the m_reactions member variable
     */
    std::vector<StoSpa2::Reaction> get_reactions() {
        return m_reactions;
    }

    /**
     * Clears the vector of Reaction objects
     */
    void clear_reactions() {
        m_reactions.clear();
    }

    /**
     * Returns the current total propensity
     * @param update determines whether member variable a_0 is updated
     */
    double get_total_propensity(bool update=true) {
        // First loop over all the reactions to get the maximum
        double total = 0;
        for (auto& reaction : m_reactions) {
            total += reaction.get_propensity(m_molecules, m_voxel_size);
        }

        // If a_0 does not need to be updated, then return the total propensity calculated thus far
        if (!update) { return total; }

        // If extrande method is used, then multiply the total propensity by the member variable m_extrande_ratio
        if (m_extrande_reaction.size() == 1) {
            total = m_extrande_ratio * total;
        }

        // Assign this total to a_0 member variable
        a_0 = total;

        return total;
    }

    /**
     * Picks a reaction from the vector of reactions (m_reactions) and returns a reference to this reaction
     * @param random_num a random number generated from a unfiform distribution
     * @return reference to the reaction that has been chosen
     */
    StoSpa2::Reaction& pick_reaction(double random_num) {
        // Scale the randomly chose number to the total propensity
        double r_a_0 = random_num * a_0;

        // Check that current total propensity is not higher than previously calculated one
        // (important for growing voxels)
        if ((m_extrande_reaction.size() == 1) and (a_0 - get_total_propensity(false) < 0)) {
            std::string m = "Voxel::pick_reaction: extrande ratio (" + std::to_string(m_extrande_ratio) + ") is too low ";
            m += "resulting in total propensity at current time (" + std::to_string(get_total_propensity(false)) + ") ";
            m += "to be greater than at previous time (" + std::to_string(a_0) + ")";
            throw std::runtime_error(m);
        }

        // Initialise some values imprtant for the loop below
        unsigned reaction_idx = 0;
        double lower_bound = 0;

        // Loop over all the reactions
        for (auto& reaction : m_reactions) {
            double propensity = reaction.get_propensity(m_molecules, m_voxel_size);
            // If the randomly chosen value is in the current interval, then break the loop,
            // otherwise move to the next interval
            if (r_a_0 > lower_bound and r_a_0 < lower_bound + propensity) {
                break;
            }
            else {
                reaction_idx += 1;
                lower_bound += propensity;
            }
        }

        // Return the correct reference to a reaction, if index is greater than size of reactions vector,
        // then it must be either extrande reaction (none -> None) or something went wrong
        if (reaction_idx < m_reactions.size()) {
            return m_reactions[reaction_idx];
        }
        else if ((reaction_idx >= m_reactions.size()) and (m_extrande_reaction.size() == 1)) {
            return m_extrande_reaction[0];
        }
        else {
            throw std::runtime_error("Voxel::pick_reaction: Wrong reaction index!");
        }
    }

    /**
     * Adds the given vector to the m_molecules member variables
     * @param stoichiometry_vec vector to be added to m_molecules
     */
    void add_vector(const std::vector<int>& stoichiometry_vec) {
        for (unsigned i=0; i<stoichiometry_vec.size(); i++) {
            // Need to be really careful here with not subtracting from zero
            if (m_molecules[i] + stoichiometry_vec[i] >= 0) {
                m_molecules[i] += stoichiometry_vec[i];
            }
        }
    }

    /**
     * Subtracts the given vector to the m_molecules member variables
     * @param stoichiometry_vec vector that is subtracted from m_molecules
     */
    void subtract_vector(const std::vector<int>& stoichiometry_vec) {
        for (unsigned i=0; i<stoichiometry_vec.size(); i++) {
            // Need to be really careful here with not subtracting from zero
            if (m_molecules[i] - stoichiometry_vec[i] >= 0) {
                m_molecules[i] -= stoichiometry_vec[i];
            }
        }
    }


    /**
     * Friend function for outputting information to stdout
     * @param os reference to output stream
     * @param v voxel that is passed to the output stream
     * @return reference to the output stream
     */
    friend std::ostream& operator << (std::ostream& os, const Voxel& v) {
        os << "Voxel object: molecules =";
        for (const auto& mol : v.m_molecules) {
            os << " " << mol;
        }
        os << "; voxel_size = " << v.m_voxel_size;
        os << "; growing = " << (v.m_growing ? "true" : "false");
        os << "; total_propensity = " << v.a_0 << ";";

        for (const auto& r : v.m_reactions) {
            os << std::endl << "\t" << r;
        }
        return os;
    }

    /**
     * Overloaded == operator for the voxel class
     * @param v1 first instance of Voxel class
     * @param v2 second instance of Voxel class
     * @return whether two voxel instances are equal
     */
    friend bool operator == (const Voxel& v1, const Voxel& v2) {
        if (v1.m_voxel_size != v2.m_voxel_size) { return false; }
        if (v1.a_0 != v2.a_0) { return false; }
        if (v1.m_molecules != v2.m_molecules) { return false; }
        if (v1.m_reactions != v2.m_reactions) { return false; }
        if (v1.m_extrande_reaction != v2.m_extrande_reaction) { return false; }
        return true;
    }

    /**
     * Overloaded != operator for the voxel class
     * @param v1 first instance of Voxel class
     * @param v2 second instance of Voxel class
     * @return whether two voxel instances are not equal
     */
    friend bool operator != (const Voxel& v1, const Voxel& v2) {
        return !(v1 == v2);
    }
};

}

#endif
