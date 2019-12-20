
#ifndef REACTION_HPP
#define REACTION_HPP

// stl
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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

template<typename T>
bool operator != (const std::vector<T>& v1, const std::vector<T>& v2) {
    return !(v1 == v2);
}

namespace StoSpa2 {

class Reaction {
protected:

    double m_rate;

    std::function<double (const std::vector<unsigned>&, const double&)> m_propensity;

public:
    const std::vector<int> stoichiometry;

    const int diffusion_idx;

    Reaction(double rate,
        std::function<double (const std::vector<unsigned>&, const double&)> propensity,
        std::vector<int> stoichiometry_vec, int diffusion_index=-1) :
        stoichiometry(std::move(stoichiometry_vec)),
        diffusion_idx(diffusion_index) {

        m_rate = rate;
        m_propensity = std::move(propensity);
    }

    void set_rate(double rate) {
        m_rate = rate;
    }

    double get_rate() {
        return m_rate;
    }

    double get_propensity(const std::vector<unsigned>& num_molecules, const double& voxel_size) {
        return m_rate * m_propensity(num_molecules, voxel_size);
    }

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

    friend bool operator == (const Reaction& r1, const Reaction& r2) {

        if (r1.m_rate != r2.m_rate) { return false; }
        if (r1.diffusion_idx != r2.diffusion_idx) { return false; }
        if (r1.stoichiometry != r2.stoichiometry) { return false; }
        return true;
    }

    friend bool operator != (const Reaction& r1, const Reaction& r2) {
        return !(r1 == r2);
    }
};

}


#endif // REACTION_HPP
