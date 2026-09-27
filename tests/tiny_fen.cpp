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

        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_ferz)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_wazir)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_pawn)) == 0);

        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_pawn)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_wazir)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_ferz)) == 0);

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
        REQUIRE(game.board().get_piece(2) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(6) == const_piece::black_pawn);
        REQUIRE(game.board().get_piece(7) == const_piece::black_wazir);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Board Test - Different piece positions 2") {
    try {
        Game game("4/fFhH/wWkK/p1P1\\- w - 0 0");
        REQUIRE(game.board().get_piece(0) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(3) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(4) == const_piece::black_ferz);
        REQUIRE(game.board().get_piece(5) == const_piece::white_ferz);
        REQUIRE(game.board().get_piece(12) == const_piece::black_pawn);
        REQUIRE(game.board().get_piece(13) == const_piece::null_piece);
        REQUIRE(game.board().get_piece(14) == const_piece::white_pawn);
        REQUIRE(game.board().get_piece(15) == const_piece::null_piece);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 1") {
    try {
        Game game("4/fFhH/w2K/p1P1\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 2") {
    try {
        Game game("4/fFhH/wWkK/ppP1\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 3") {
    try {
        Game game("2w1/fFhH/wWkK/2PW\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 4") {
    try {
        Game game("2w1/fFhH/wWpK/2W\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong piece count 5") {
    try {
        Game game("2w1/4/hHkK/2pW\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong King count 1") {
    try {
        Game game("fhwk/2kp/P3/KWHF\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong King count 2") {
    try {
        Game game("fhwk/2kp/P3/1WHF\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Board Test - Wrong King count 3") {
    try {
        Game game("fhw1/2kp/P3/1WHF\\- w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

// House Test

TEST_CASE("TinyFen Test - House Test - Correct filling 1") {
    try {
        Game game("fhwk/3p/P3/KW1F\\H w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 0);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Correct filling 2") {
    try {
        Game game("fhwk/3p/P3/KW1F\\h w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 1);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Correct filling 3") {
    try {
        Game game("f2k/3p/P3/KW1F\\Hhw w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_wazir)) == 1);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Correct filling 4") {
    try {
        Game game("1h1k/4/4/KWHF\\PFPW w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_pawn)) == 2);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_ferz)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_wazir)) == 1);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Correct filling 5") {
    try {
        Game game("1k1K/4/4/4\\fFhHwWpP w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_ferz)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_wazir)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_pawn)) == 1);

        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_ferz)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_wazir)) == 1);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_pawn)) == 1);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Correct filling 6") {
    try {
        Game game("1k1K/4/4/4\\FFHHWWPP w - 0 0");
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_ferz)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_hors)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_wazir)) == 0);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::black_pawn)) == 0);

        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_ferz)) == 2);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_hors)) == 2);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_wazir)) == 2);
        REQUIRE(game.house().count(House::piece_to_index(const_piece::white_pawn)) == 2);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - House Test - Wrong filling 1") {
    try {
        Game game("1k1K/4/4/4\\FFfHHWWPP w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - House Test - Wrong filling 2") {
    try {
        Game game("fhwk/3p/P3/KWHF\\H w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - House Test - Wrong filling 3") {
    try {
        Game game("fhwk/3p/P3/KWHF\\K w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - House Test - Wrong filling 4") {
    try {
        Game game("fh1k/3p/P3/K1HF\\wWW w - 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

// Promotion Test

TEST_CASE("TinyFen Test - Promotion Test - Correct Promotion 1 ") {
    try {
        Game game("fhwk/3f/P3/KWHF\\- w a4 0 0");
        REQUIRE((game.board().get_piece(Board::get_index(0, 0)) & PROMOTED) != 0);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Promotion Test - Correct Promotion 2 ") {
    try {
        Game game("fhwk/3w/F3/KWHF\\- w d3 a2 0 0");
        REQUIRE((game.board().get_piece(Board::get_index(1, 3)) & PROMOTED) != 0);
        REQUIRE((game.board().get_piece(Board::get_index(2, 0)) & PROMOTED) != 0);
    } catch (const std::invalid_argument& iae) {
        std::cerr << iae.what() << "\n";
        REQUIRE(false);
    }
}

TEST_CASE("TinyFen Test - Promotion Test - Wrong Promotion, too many ") {
    try {
        Game game("fhwk/3w/F3/KWHF\\- w d3 a2 b4 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Promotion Test - Wrong Promotion, king ") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w a1 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Promotion Test - Wrong Promotion, empty tile ") {
    try {
        Game game("fhwk/3p/P3/KWHF\\- w b2 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}

TEST_CASE("TinyFen Test - Promotion Test - Wrong Promotion, double promotion ") {
    try {
        Game game("fhwk/3w/P3/KWHF\\- w d3 d3 0 0");
        REQUIRE(false);
    } catch (const std::invalid_argument& iae) {
        REQUIRE(true);
    }
}
