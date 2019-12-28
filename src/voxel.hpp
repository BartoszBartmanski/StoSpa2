
#ifndef VOXEL_HPP
#define VOXEL_HPP

// stl
#include <vector>
#include <iostream>

// other header files
#include "reaction.hpp"

auto constant_func = [](const std::vector<unsigned>& mols, const double& area) { return 1.0; };

namespace StoSpa2 {

class Voxel {
protected:
    double m_voxel_size;

    double a_0;

    std::vector<unsigned> m_molecules;

    std::vector<StoSpa2::Reaction> m_reactions;

    std::vector<StoSpa2::Reaction> m_extrande_reaction;

    double m_initial_voxel_size;
    std::function<double (const double&)> m_growth_func;
    bool m_growing;

public:

    Voxel(std::vector<unsigned> inital_num, double voxel_size) {
        m_voxel_size = voxel_size;
        m_initial_voxel_size = voxel_size;
        m_molecules = std::move(inital_num);

        m_growing = false;
        m_growth_func = [](const double& time) { return 1.0; };
    }

    Voxel(std::vector<unsigned> inital_num,  double voxel_size, std::function<double (const double&)> growth) {
        m_voxel_size = voxel_size;
        m_initial_voxel_size = voxel_size;
        m_molecules = std::move(inital_num);

        m_growing = true;
        m_growth_func = std::move(growth);
        add_extrande();
    }

    std::vector<unsigned> get_molecules() {
        return m_molecules;
    }

    double get_voxel_size() {
        return m_voxel_size;
    }

    bool is_growing() {
        return m_growing;
    }

    void update_properties(const double& time) {
        if (m_growing) {
            double new_factor = m_growth_func(time);
            m_voxel_size = new_factor * m_initial_voxel_size;
            double diff_factor = 1.0 / (new_factor * new_factor);

            for (auto& reaction : m_reactions) {
                reaction.update_properties(diff_factor);
            }
        }
    }

    void add_extrande() {
        if (m_extrande_reaction.empty()) {
            m_extrande_reaction.emplace_back(StoSpa2::Reaction(0.0, constant_func, {0}));
        }
    }

    void add_reaction(StoSpa2::Reaction r) {
        if (r.stoichiometry.size() != m_molecules.size()) {
            std::string m = "Simulator::add_reaction: r.stoichiometry.size() != m_molecules.size()";
            throw std::runtime_error(m);
        }

        if (r.get_rate() > 0) {
            m_reactions.push_back(r);
        }
    }

    std::vector<StoSpa2::Reaction> get_reactions() {
        return m_reactions;
    }

    void clear_reactions() {
        m_reactions.clear();
    }

    double get_total_propensity() {
        double total = 0;
        for (auto& reaction : m_reactions) {
            total += reaction.get_propensity(m_molecules, m_voxel_size);
        }

        if (m_extrande_reaction.size() == 1) {
            double extrande_propensity = m_extrande_reaction[0].get_rate();
            if (extrande_propensity < 0.5 * total) {
                m_extrande_reaction[0].set_rate(total);
            }
            total += extrande_propensity;
        }
        a_0 = total;

        return total;
    }

    StoSpa2::Reaction& pick_reaction(double random_num) {
        double r_a_0 = random_num * a_0;

        unsigned reaction_idx = 0;
        double lower_bound = 0;
        for (auto& reaction : m_reactions) {
            double propensity = reaction.get_propensity(m_molecules, m_voxel_size);
            if (r_a_0 > lower_bound and r_a_0 < lower_bound + propensity) {
                break;
            }
            else {
                reaction_idx += 1;
                lower_bound += propensity;
            }
        }
        if (reaction_idx < m_reactions.size()){
            return m_reactions[reaction_idx];
        }
        else if ((reaction_idx >= m_reactions.size()) and (m_extrande_reaction.size() == 1)) {
            return m_extrande_reaction[0];
        }
        else {
            throw std::runtime_error("Voxel::pick_reaction: Wrong reaction index!");
        }
    }

    void update_molecules(const std::vector<int>& stoichiometry_vec) {
        for (unsigned i=0; i<stoichiometry_vec.size(); i++) {
            // Need to be really careful here with not subtracting from zero
            if (m_molecules[i] + stoichiometry_vec[i] >= 0) {
                m_molecules[i] += stoichiometry_vec[i];
            }
        }
    }

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

    friend bool operator == (const Voxel& v1, const Voxel& v2) {
        if (v1.m_voxel_size != v2.m_voxel_size) { return false; }
        if (v1.a_0 != v2.a_0) { return false; }
        if (v1.m_molecules != v2.m_molecules) { return false; }
        if (v1.m_reactions != v2.m_reactions) { return false; }
        if (v1.m_extrande_reaction != v2.m_extrande_reaction) { return false; }
        return true;
    }

    friend bool operator != (const Voxel& v1, const Voxel& v2) {
        return !(v1 == v2);
    }
};

}

void add_reaction_to_voxels(const StoSpa2::Reaction& r, std::vector<StoSpa2::Voxel>& voxels) {
    for (auto& v : voxels) {
        v.add_reaction(r);
    }
}

#endif
