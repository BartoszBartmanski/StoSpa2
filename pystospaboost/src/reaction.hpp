
#ifndef REACTION_HPP
#define REACTION_HPP

// stl
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

/**
 * Overloaded == operator for std::vector class. This operator is used in
 * the reaction class to compare certain member variables
 * @param v1 first instance of std::vector
 * @param v2 second instance of std::vector
 * @return whether v1 is equal to v2
 */
template<typename T>
bool operator == (const std::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.size() != v2.size()) { return false; }
    for (unsigned i=0; i<v1.size(); i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Overloaded != operator for std::vector class. This operator is used in
 * the reaction class to compare certain member variables
 * @param v1 first instance of std::vector
 * @param v2 second instance of std::vector
 * @return whether v1 is not equal to v2
 */
template<typename T>
bool operator != (const std::vector<T>& v1, const std::vector<T>& v2) {
    return !(v1 == v2);
}

/**
 * Type alias \c p_f for function signature:
 * \code std::function<double (const std::vector<unsigned>&, const double&)> \endcode
 */
typedef std::function<double (const std::vector<unsigned>&, const double&)> p_f;

namespace StoSpa2 {

/**
 * Reaction class - represents a reaction within stochastic modelling. Reaction class contains
 * rate of a reaction, propensity and stoichiometry vector, all of which need to be given to the
 * constructor.
 */
class Reaction {
protected:
    /** Initial reaction rate */
    double m_initial_rate;

    /** Rate of reaction that can be updated, hence can be different from m_inital rate */
    double m_rate;

    /** Lambda function that returns propensity given the numebr of molecules and the area of a voxell */
    p_f m_propensity;

public:
    /** The stoichiometry vector i.e. how the number of molecules changes if this reaction happens */
    const std::vector<int> stoichiometry;

    /** Variable used to indicate if this reaction is a diffusion reaction */
    const int diffusion_idx;

    /**
     * Constructor for Reaction class
     * @param rate the rate of the reaction
     * @param propensity lambda function that returns propensity given number of molecules and voxel area
     * @param stoichiometry_vec stoichiometry vector
     * @param diffusion_index index of the voxel in a vector of voxels where a molecule would jump
     */
    Reaction(double rate, p_f propensity, std::vector<int> stoichiometry_vec, int diffusion_index=-1) :
        stoichiometry(std::move(stoichiometry_vec)),
        diffusion_idx(diffusion_index) {

        m_initial_rate = rate;
        m_rate = rate;
        m_propensity = std::move(propensity);
    }

    /**
     * Sets the rate of the reaction instance
     * @param rate the rate of reaction
     */
    void set_rate(double rate) {
        m_rate = rate;
    }

    /**
     * Returns the rate of reaction
     * @return copy of the value of m_rate
     */
    double get_rate() {
        return m_rate;
    }

    /**
     * Updates any properties of the reaction instance, such as the rate
     * @param factor value by which to mulpiply the initial reaction rate (m_initial_rate)
     */
    void update_properties(const double& factor) {
        if (diffusion_idx >=0) {
            m_rate = factor * m_initial_rate;
        }
    }

    /**
     * Returns the value of propensity given the number of molecules and voxel length/area
     * @param num_molecules number of molecules given as a vector
     * @param voxel_size length / area / volume of a voxel
     */
    double get_propensity(const std::vector<unsigned>& num_molecules, const double& voxel_size) {
        return m_rate * m_propensity(num_molecules, voxel_size);
    }

    /**
     * Overloaded operator to pass member variables to the standard output
     * @param os reference to the output stream
     * @param r reference to an instance of Reaction class
     * @return reference to the output stream
     */
    friend std::ostream& operator << (std::ostream& os, const Reaction& r)
    {
        os << "Reaction object: reaction rate = " << r.m_rate;
        os << "; diffusion index = " << r.diffusion_idx;
        os << "; stoichiometry_vec =";
        for (const auto& s : r.stoichiometry) {
            os << " " << s;
        }
        return os;
    }

    /**
     * Overloaded == operator for the Reaction class
     * @param r1 first instance of Reaction class
     * @param r2 second instance of Reaction class
     * @return whether two Reaction instances are equal
     */
    friend bool operator == (const Reaction& r1, const Reaction& r2) {

        if (r1.m_rate != r2.m_rate) { return false; }
        if (r1.diffusion_idx != r2.diffusion_idx) { return false; }
        if (r1.stoichiometry != r2.stoichiometry) { return false; }
        return true;
    }

    /**
     * Overloaded != operator for the Reaction class
     * @param r1 first instance of Reaction class
     * @param r2 second instance of Reaction class
     * @return whether two Reaction instances are not equal
     */
    friend bool operator != (const Reaction& r1, const Reaction& r2) {
        return !(r1 == r2);
    }
};

}


#endif // REACTION_HPP
