
// catch2 includes
#include "catch.hpp"

// StoSpa2 includes
#include "simulator.hpp"

namespace ss = StoSpa2;

TEST_CASE("Testing Simulator class") {
    auto decay = [](const std::vector<unsigned>& mols, const double& area) { return mols[0]; };
    ss::Voxel v({10}, 1.0);
    v.add_reaction(ss::Reaction(1.5, decay, {-1}));
    ss::Simulator s({v});

    SECTION("Testing Constructor") {
        REQUIRE(s.get_time() == 0);
        auto vs = s.get_voxels();
        REQUIRE(vs[0].get_molecules()[0] == 10);
    }

    SECTION("Testing member functions") {
        // Check setting seed for generating random number
        s.set_seed(153);
        REQUIRE(s.get_seed() == 153);

        // Check a single step in SSA works
        s.step();
        REQUIRE(s.get_time() > 0);
        auto vs2 = s.get_voxels();
        REQUIRE(vs2[0].get_molecules()[0] == 9);

        // Check that getting to specific time point works
        s.advance(1.0);
        REQUIRE(s.get_time() > 1.0);
    }
}
