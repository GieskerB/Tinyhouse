#include <catch2/catch_test_macros.hpp>

// Example 1: Basic assertions
TEST_CASE("Engine basic rules initialization", "[variant][core]") {
    REQUIRE(2 + 2 == 4); // Placeholder sanity check
}

// Example 2: Perft validation test structure
TEST_CASE("Perft node count checks", "[perft]") {
    SECTION("Depth 1 starting position") {

        unsigned long long expected_nodes = 20; 
        unsigned long long calculated_nodes = 20;
        
        REQUIRE(calculated_nodes == expected_nodes);
    }
}