#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>

#include "../inc/game.hpp"

// void log(std::string msg) {
//     static std::ofstream file{"log.txt"};
//     if(file.is_open()) {
//         file << msg << "\n";
//     }
// }

TEST_CASE("TinyFen Test - Starter Fen" /*, "[variant][core]"*/) {
    bool working = false;
    try {
        Game game;
        working = true;
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
    }
    REQUIRE(working);
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 1") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 0 0 3");
        REQUIRE(game.get_threefold_repetition_counter() == 0);
        REQUIRE(game.get_halfmove_counter() == 0);
        REQUIRE(game.get_fullmove_counter() == 3);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 2") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 1 1 5");
        REQUIRE(game.get_threefold_repetition_counter() == 1);
        REQUIRE(game.get_halfmove_counter() == 1);
        REQUIRE(game.get_fullmove_counter() == 5);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 3") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 0 7 9");
        REQUIRE(game.get_threefold_repetition_counter() == 0);
        REQUIRE(game.get_halfmove_counter() == 7);
        REQUIRE(game.get_fullmove_counter() == 9);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 4") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 2 4 8");
        REQUIRE(game.get_threefold_repetition_counter() == 2);
        REQUIRE(game.get_halfmove_counter() == 4);
        REQUIRE(game.get_fullmove_counter() == 8);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Fail Counter 1") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 3 2 2");
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(true);
    }
}
TEST_CASE("TinyFen Test - Altered Starter Fen - Fail Counter 2") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w 1 5 2");
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Turn 1") {
    try {
        Game game;
        REQUIRE(!game.is_blacks_turn());
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Turn 2") {
    try {
        Game game;
        REQUIRE(!game.is_blacks_turn());
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}


// // Example 2: Perft validation test structure
// TEST_CASE("Perft node count checks", "[perft]") {
//     SECTION("Depth 1 starting position") {

//         unsigned long long expected_nodes = 20;
//         unsigned long long calculated_nodes = 20;

//         REQUIRE(calculated_nodes == expected_nodes);
//     }
// }
