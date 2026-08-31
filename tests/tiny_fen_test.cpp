#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>

#include "../inc/game.hpp"

// Base Test

TEST_CASE("TinyFen Test - Starter Fen") {
    try {
        Game game;
        // Test is game variables are set correctly
        REQUIRE(game.is_whites_turn());
        REQUIRE(game.halfmove_counter() == 0);
        REQUIRE(game.fullmove_counter() == 0);

        // Test if board is setup correctly
        REQUIRE(game.board().get_piece(0) == const_piece::black_ferz);
        REQUIRE(game.board().get_piece(1) == const_piece::black_hors);
        REQUIRE(game.board().get_piece(2) == const_piece::black_wazir);
        REQUIRE(game.board().get_piece(3) == const_piece::black_king);

        REQUIRE(game.board().get_piece(4) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(5) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(6) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(7) == const_piece::black_pawn);

        REQUIRE(game.board().get_piece(8) == const_piece::white_pawn);
        REQUIRE(game.board().get_piece(9) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(10) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(11) == const_piece::null_piece);

        REQUIRE(game.board().get_piece(12) == const_piece::white_king);
        REQUIRE(game.board().get_piece(13) == const_piece::white_wazir);
        REQUIRE(game.board().get_piece(14) == const_piece::white_hors);
        REQUIRE(game.board().get_piece(15) == const_piece::white_ferz);

        // Test is house is setup correctly (empty)
        REQUIRE(game.house().is_empty());

    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

// Counter Test

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 1") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 0 3");
        REQUIRE(game.halfmove_counter() == 0);
        REQUIRE(game.fullmove_counter() == 3);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 2") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 1 5");
        REQUIRE(game.halfmove_counter() == 1);
        REQUIRE(game.fullmove_counter() == 5);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 3") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 7 9");
        REQUIRE(game.halfmove_counter() == 7);
        REQUIRE(game.fullmove_counter() == 9);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter 4") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 4 8");
        REQUIRE(game.halfmove_counter() == 4);
        REQUIRE(game.fullmove_counter() == 8);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Altered Starter Fen - Counter Fail 1") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 4 4");
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(true);
    }
}
TEST_CASE("TinyFen Test - Altered Starter Fen - Counter Fail 2") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w - 5 2");
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

// Turn Test

TEST_CASE("TinyFen Test - Altered Starter Fen - Black Turn") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- b - 0 0");
        REQUIRE(!game.is_whites_turn());
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

// Board Test

TEST_CASE("TinyFen Test - Board Test - Different piece positions 1") {
    try {
        Game game("fh1k/2pw/P3/KWHF\\- w - 0 0");
        REQUIRE(game.board().get_piece(2) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(6) == const_piece:: black_pawn);
        REQUIRE(game.board().get_piece(7) == const_piece:: black_wazir);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Board Test - Different piece positions 2") {
    try {
        Game game("4/fFhH/wWkK/p1P1\\- w - 0 0");
        REQUIRE(game.board().get_piece(0) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(3) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(4) == const_piece:: black_ferz);
        REQUIRE(game.board().get_piece(5) == const_piece:: white_ferz);
        REQUIRE(game.board().get_piece(12) == const_piece:: black_pawn);
        REQUIRE(game.board().get_piece(13) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(14) == const_piece:: white_pawn);
        REQUIRE(game.board().get_piece(15) == const_piece:: null_piece);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 2") {
    try {
        Game game("4/fFhH/wWkK/p1P1\\- w - 0 0");
        REQUIRE(game.board().get_piece(0) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(3) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(4) == const_piece:: black_ferz);
        REQUIRE(game.board().get_piece(5) == const_piece:: white_ferz);
        REQUIRE(game.board().get_piece(12) == const_piece:: black_pawn);
        REQUIRE(game.board().get_piece(13) == const_piece:: null_piece);
        REQUIRE(game.board().get_piece(14) == const_piece:: white_pawn);
        REQUIRE(game.board().get_piece(15) == const_piece:: null_piece);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}


// House Test
