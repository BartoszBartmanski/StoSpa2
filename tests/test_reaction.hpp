
// catch2 includes
#include "catch.hpp"

// StoSpa2 includes
#include "reaction.hpp"

TEST_CASE("Testing Reacton class") {
    auto constant_func = [](const std::vector<unsigned>& mols, const double& area) { return 1.0; };

    Reaction r(0.0, constant_func, {0});

    SECTION("Testing Constructor") {
        REQUIRE(r.get_rate() == 0.0);
        REQUIRE(r.diffusion_idx == -1);
        REQUIRE(r.stoichiometry.size() == 1);
        REQUIRE(r.stoichiometry[0] == 0);
    }

    SECTION("Testing member functions") {
        r.set_rate(1.55);
        REQUIRE(r.get_rate() == 1.55);

        double propensity = r.get_propensity({10}, 1.0);
        REQUIRE(propensity == 1.55);
    }

    SECTION("Testing member operators") {
        Reaction r2(0.0, constant_func, {0});
        REQUIRE(r == r2);
    }
}
