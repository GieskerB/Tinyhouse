#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "../inc/game.hpp"
#include "../inc/move.hpp"

static inline void quick_tester(const char* str_index, const std::vector<int8_t> valid_offsets) {
    Game game;
    auto offsets = valid_move_offsets(game.board(), str_index);
    std::sort(offsets.begin(), offsets.end());
    REQUIRE(offsets == valid_offsets);
}
static inline void quick_tester(const char* tiny_fen, const char* str_index, const std::vector<int8_t> valid_offsets) {
    Game game(tiny_fen);
    auto offsets = valid_move_offsets(game.board(), str_index);
    std::sort(offsets.begin(), offsets.end());
    REQUIRE(offsets == valid_offsets);
}

// King Offset Test
TEST_CASE("Move Offset Test - King Starter") {
    quick_tester("a1", {-3});
    quick_tester("d4", {3});
}

TEST_CASE("Move Offset Test - King Border") {
    quick_tester("3k/4/K3/4\\FHWPfhwp w - 0 1", "a2", {-4,-3,1,4,5});
}

TEST_CASE("Move Offset Test - King Open") {
    quick_tester("3k/4/1K2/4\\FHWPfhwp w - 0 1", "b2", {-5,-4,-3,-1,1,3,4,5});
}

TEST_CASE("Move Offset Test - King Capture") {
    quick_tester("3k/1W2/PKp1/f3\\FHhw w - 0 1", "b2", {-5,-3,1,3,4,5});
}

// Pawn Offset Test
TEST_CASE("Move Offset Test - Pawn Starter") {
    quick_tester("a2", {-4});
    quick_tester("d3", {4});
}

TEST_CASE("Move Offset Test - Pawn Open") {
    quick_tester("3k/4/1P2/3K\\FHWfhwp w - 0 1","b2", {-4});
    quick_tester("3k/4/1p2/3K\\FHWfhwP w - 0 1","b2", {4});
}

TEST_CASE("Move Offset Test - Pawn Capture") {
    quick_tester("3k/F1w1/1P2/K3\\HfhWp w - 0 1","b2", {-4,-3});
    quick_tester("3k/F1w1/1p2/K3\\HfhWp w - 0 1","b2", {3,4});
}

// Ferz Offset Test
TEST_CASE("Move Offset Test - Ferz Starter") {
    quick_tester("d1", {-5});
    quick_tester("a4", {5});
}

TEST_CASE("Move Offset Test - Ferz Border") {
    quick_tester("3k/4/F3/3K\\HWPfhwp w - 0 1", "a2", {-3,5});
}

TEST_CASE("Move Offset Test - Ferz Open") {
    quick_tester("3k/4/1F2/3K\\HWPfhwp w - 0 1", "b2", {-5,-3,3,5});
}

TEST_CASE("Move Offset Test - Ferz Capture") {
    quick_tester("3k/P1p1/1F2/w1WK\\Hfh w - 0 1", "b2", {-3,3});
}

// Wazir Offset Test
TEST_CASE("Move Offset Test - Wazir Starter") {
    quick_tester("a2", {-4});
    quick_tester("d3", {4});
}

TEST_CASE("Move Offset Test - Wazir Border") {
    quick_tester("3k/4/W3/3K\\FHPfhwp w - 0 1", "a2", {-4,1,4});
}

TEST_CASE("Move Offset Test - Wazir Open") {
    quick_tester("3k/4/1W2/3K\\FHPfhwp w - 0 1", "b2", {-4,-1,1,4});
}

TEST_CASE("Move Offset Test - Wazir Capture") {
    quick_tester("3k/1p2/FWP1/1f1K\\Hhw w - 0 1", "b2", {-4,4});
}

// Hors Offset Test
TEST_CASE("Move Offset Test - Hors Starter") {
    quick_tester("a2", {-4});
    quick_tester("d3", {4});
}

TEST_CASE("Move Offset Test - Hors Border") {
    quick_tester("3k/4/H3/3K\\FWPfhwp w - 0 1", "a2", {-7,-2,6});
}

TEST_CASE("Move Offset Test - Hors Open") {
    quick_tester("3k/4/1H1K/4\\FWPfhwp w - 0 1", "b2", {-9,-7,-2,6});
    quick_tester("k3/2H1/1K2/4\\FWPfhwp w - 0 1", "c3", {-6,2,7,9});
}

TEST_CASE("Move Offset Test - Hors Capture") {
    quick_tester("3k/1p1P/1H2/KW1w\\Ffh w - 0 1", "b2", {6});
    quick_tester("3k/1P1p/1H2/KW1w\\Ffh w - 0 1", "b2", {-2,6});
}
