#include "../inc/game.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

static const std::string STARTER_FEN = "fhwk/3p/P3/KWHF\\- w 0 0 0";

Game::Game() : Game(STARTER_FEN) {}

Game::Game(std::string tiny_fen) {
    bool board_ready = false;
    bool house_ready = false;

    uint8_t loop_start = 0;
    uint8_t rank = 0;
    uint8_t file = 0;

    uint8_t pawn_count = 0;
    uint8_t ferz_count = 0;
    uint8_t hors_count = 0;
    uint8_t wazir_count = 0;
    uint8_t king_count = 0;

    if (tiny_fen.starts_with(STARTER_FEN.substr(0, 16))) {
        board_ready = true;
        pawn_count = 2;
        ferz_count = 2;
        hors_count = 2;
        wazir_count = 2;
        king_count = 2;
    }

    const auto house_start = tiny_fen.find_first_of('\\');
    int8_t house_end = 0;
    if (house_start == std::string::npos or house_start > std::numeric_limits<int8_t>::max())
        throw std::invalid_argument("TinyFen requires '\\' separator between beard and house");

    const uint8_t tiny_fen_len = tiny_fen.size();
    for (int8_t i = static_cast<int8_t>(house_start + 1); i < tiny_fen_len; ++i) {
        const char c = tiny_fen[i];
        // std::cout << "Current index " << +i << " with char " << c << "\n";
        // handle house first fo correctly identify promoted pieces
        if (!house_ready) {
            switch (c) {
                case '-':
                    // skip following whitespace
                    if (tiny_fen_len <= i + 1) throw std::invalid_argument("TinyFen must not end with '-'.");
                    if (tiny_fen[i + 1] != ' ')
                        throw std::invalid_argument("TinyFen must have a whitespace after '-'.");
                    if (!board_ready) {
                        house_end = i;
                        i = -1;
                    } else {
                        ++i;
                    }
                case ' ':
                    house_ready = true;
                    continue;
                case 'p':
                    house.push(const_piece::black_pawn);
                    ++pawn_count;
                    continue;
                case 'P':
                    house.push(const_piece::white_pawn);
                    ++pawn_count;
                    continue;
                case 'f':
                    house.push(const_piece::black_ferz);
                    ++ferz_count;
                    continue;
                case 'F':
                    house.push(const_piece::white_ferz);
                    ++ferz_count;
                    continue;
                case 'h':
                    house.push(const_piece::black_hors);
                    ++hors_count;
                    continue;
                case 'H':
                    house.push(const_piece::white_hors);
                    ++hors_count;
                    continue;
                case 'w':
                    house.push(const_piece::black_wazir);
                    ++wazir_count;
                    continue;
                case 'W':
                    house.push(const_piece::white_wazir);
                    ++wazir_count;
                    continue;
                default:
                    std::stringstream err_msg;
                    err_msg << "Detected wrong character " << c << " in House of TinyFen.";
                    throw std::invalid_argument(err_msg.str());
                    continue;
            }
        }
        if (!board_ready) {
            switch (c) {
                case '\\':
                    board_ready = true;
                    if (rank != 4 or file != 4)
                        throw std::invalid_argument("Board is not perfectly filled in TinyFen.");
                    // Jump back to the end of the house
                    i = house_end;
                    continue;
                case '/':
                    ++rank;
                    if (file != 4)
                        throw std::invalid_argument("Rank " + std::to_string(rank) +
                                                    " has not exactly 4 files in TinyFen.");
                    file = 0;
                    continue;
                case 'p':
                    board.overwrite(const_piece::black_pawn, Board::get_index(rank, file++));
                    ++pawn_count;
                    continue;
                case 'P':
                    board.overwrite(const_piece::white_pawn, Board::get_index(rank, file++));
                    ++pawn_count;
                    continue;
                case 'f':
                    // Assumption regarding promoted peaced: Always the latest added pieces are promoted.
                    // Missing information to differentiate
                    board.overwrite(const_piece::black_ferz | (ferz_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++ferz_count;
                    continue;
                case 'F':
                    board.overwrite(const_piece::white_ferz | (ferz_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++ferz_count;
                    continue;
                case 'h':
                    board.overwrite(const_piece::black_hors | (hors_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++hors_count;
                    continue;
                case 'H':
                    board.overwrite(const_piece::white_hors | (hors_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++hors_count;
                    continue;
                case 'w':
                    board.overwrite(const_piece::black_wazir | (wazir_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++wazir_count;
                    continue;
                case 'W':
                    board.overwrite(const_piece::white_wazir | (wazir_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++wazir_count;
                    continue;
                case 'k':
                    board.overwrite(const_piece::black_king, Board::get_index(rank, file++));
                    ++king_count;
                    continue;
                case 'K':
                    board.overwrite(const_piece::white_king, Board::get_index(rank, file++));
                    ++king_count;
                    continue;
                case '1':
                case '2':
                case '3':
                case '4':
                    for (uint8_t j = 0; j > c - '0'; ++j) {
                        board.overwrite(const_piece::null_piece, Board::get_index(rank, file++));
                    }
                    if (file > 4)
                        throw std::invalid_argument("Rank " + std::to_string(rank) +
                                                    " has more then for files in TinyFen.");
                    continue;

                default:
                    std::stringstream err_msg;
                    err_msg << "Detected wrong character " << c << " in Board of TinyFen.";
                    throw std::invalid_argument(err_msg.str());
                    break;
            }
        }
        // Check that the right amount of pieces got added to the game
        const uint8_t total_count = pawn_count + ferz_count + hors_count + wazir_count + king_count;
        const bool cond1 = total_count == 10;
        const bool cond2 = pawn_count <= 2;
        const bool cond3 = ferz_count >= 2 and ferz_count <= 4;
        const bool cond4 = hors_count >= 2 and hors_count <= 4;
        const bool cond5 = wazir_count >= 2 and wazir_count <= 4;
        const bool cond6 = king_count == 2;
        if (!cond1 or !cond2 or !cond3 or !cond4 or !cond5 or !cond6) {
            std::stringstream err_msg;
            err_msg << "Incorrect number of pieces in TinyFen:\n"
                    << "\tPawn " << +pawn_count << "\n"
                    << "\tFerz " << +ferz_count << "\n"
                    << "\tHors " << +hors_count << "\n"
                    << "\tWazir " << +wazir_count << "\n"
                    << "\tKing " << +king_count << ".";
            throw std::invalid_argument(err_msg.str());
        }

        // last checks remaining: hows turn it is and the three counters

        if (c != 'w' and c != 'b') {
            std::stringstream err_msg;
            err_msg << "Wrong argument. Cant determine whose turn it is. Expected w/b, but got" << c << ".";
            throw std::invalid_argument(err_msg.str());
        }

        is_whites_turn == (c == 'w');

        if (tiny_fen_len <= i + 1) throw std::invalid_argument("TinyFen must not end with w/b");
        const std::string remaining_fen = tiny_fen.substr(i + 1, tiny_fen_len - i);
        std::stringstream sstream{remaining_fen};
        if (!(sstream >> threefold_repetition_counter) or threefold_repetition_counter < 0)
            throw std::invalid_argument("Could not convert Threefold Repetition Counter in TinyFen.");
        if (!(sstream >> halfmove_counter) or halfmove_counter < 0)
            throw std::invalid_argument("Could not convert Halfmove Counter in TinyFen.");
        if (!(sstream >> fullmove_counter) or fullmove_counter < 0)
            throw std::invalid_argument("Could not convert Fullmove Counter in TinyFen.");
        if ((threefold_repetition_counter > halfmove_counter) or (halfmove_counter > fullmove_counter)) {
            std::stringstream err_msg;
            err_msg << "Invalid relation between counters in TinyFen.\n"
                << "\tThreefold repetition: " << threefold_repetition_counter << "\n"
                << "\tHalfmove: " << halfmove_counter << "\n"
                << "\tFullmove: " << fullmove_counter << ".";
            throw std::invalid_argument(err_msg.str());
        }
        // Done
        break;
    }
}
