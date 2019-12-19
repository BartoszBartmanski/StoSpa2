
// catch2 includes
#include "catch.hpp"

// StoSpa2 includes
#include "voxel.hpp"

TEST_CASE("Testing Voxel class") {
    auto decay = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };

    Voxel v({10}, 1.0);

    SECTION("Testing Constructor") {
        auto vec = v.get_molecules();
        REQUIRE(vec.size() == 1);
        REQUIRE(vec[0] == 10);

        auto size = v.get_voxel_size();
        REQUIRE(size == 1.0);
    }

    SECTION("Testing member functions") {
        Reaction r(1.5, decay, {-1});
        v.add_reaction(r);

        std::vector<Reaction> reactions = v.get_reactions();
        REQUIRE(reactions.size() == 1);
        REQUIRE(reactions[0] == r);

        reactions[0].set_rate(20);

        std::vector<Reaction> reactions2 = v.get_reactions();
        REQUIRE(reactions[0] != reactions2[0]);

        REQUIRE(v.get_total_propensity() == 15);

        REQUIRE(v.pick_reaction(0.5) == r);

        v.update_molecules(r.stoichiometry);
        REQUIRE(v.get_molecules()[0] == 9);

        v.clear_reactions();
        REQUIRE(v.get_reactions().size() == 0);

    }
}
