#include "../inc/game.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>

static const std::string STARTER_FEN = "fhwk/3p/P3/KWHF\\- w - 0 0";

static void throw_general_error(std::string msg) {
    std::stringstream sstream;
    sstream << "Tinyfen error: " << msg << ".\n";
    throw std::invalid_argument(sstream.str());
}
static void throw_number_error(std::string msg, int number) {
    std::stringstream sstream;
    sstream << "Tinyfen error: " << msg << ": " << number << ".\n";
    throw std::invalid_argument(sstream.str());
}
static void throw_character_error(std::string msg, std::string expected, char got) {
    std::stringstream sstream;
    sstream << "Tinyfen error: " << msg << ". Expected to get : '" << expected << "' but got '" << got << "'.\n";
    throw std::invalid_argument(sstream.str());
}
static void throw_missing_section_error(std::string section) {
    std::stringstream sstream;
    sstream << "Tinyfen incomplete: Must not end after " << section << "'.\n";
    throw std::invalid_argument(sstream.str());
}

Game::Game() : Game(STARTER_FEN) {}
Game::Game(std::string tiny_fen) {
    // Decode the TinyFen

    bool board_ready = false;
    bool house_ready = false;
    bool turn_ready = false;
    bool promotion_ready = false;

    uint8_t temp;
    uint8_t loop_start = 0;
    uint8_t rank = 0;
    uint8_t file = 0;

    uint8_t pawn_count = 0;
    uint8_t ferz_count = 0;
    uint8_t hors_count = 0;
    uint8_t wazir_count = 0;
    uint8_t king_count = 0;
    uint8_t promotion_count = 0;

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
        throw_general_error("requires '\\' separator between board and house");

    const uint8_t tiny_fen_len = tiny_fen.size();
    for (int8_t i = static_cast<int8_t>(house_start + 1); i < tiny_fen_len; ++i) {
        const char c = tiny_fen[i];
        // handle house first fo correctly identify promoted pieces
        if (!house_ready) {
            switch (c) {
                case '-':
                    // skip following whitespace
                    if (tiny_fen_len <= i + 1) throw_missing_section_error("Board");
                    if (tiny_fen[i + 1] != ' ')
                        throw_character_error("House section must end with space", " ", tiny_fen[i + 1]);
                    if (!board_ready) {
                        house_end = i+1;
                        i = -1;
                    } else {
                        ++i;
                    }
                case ' ':
                    house_ready = true;
                    continue;
                case 'p':
                    m_house.push(const_piece::black_pawn);
                    ++pawn_count;
                    continue;
                case 'P':
                    m_house.push(const_piece::white_pawn);
                    ++pawn_count;
                    continue;
                case 'f':
                    m_house.push(const_piece::black_ferz);
                    ++ferz_count;
                    continue;
                case 'F':
                    m_house.push(const_piece::white_ferz);
                    ++ferz_count;
                    continue;
                case 'h':
                    m_house.push(const_piece::black_hors);
                    ++hors_count;
                    continue;
                case 'H':
                    m_house.push(const_piece::white_hors);
                    ++hors_count;
                    continue;
                case 'w':
                    m_house.push(const_piece::black_wazir);
                    ++wazir_count;
                    continue;
                case 'W':
                    m_house.push(const_piece::white_wazir);
                    ++wazir_count;
                    continue;
                default:
                    throw_character_error("Invalid character in House section", "p/f/h/w/P/F/H/W", c);
            }
        }
        if (!board_ready) {
            switch (c) {
                case '\\':
                    ++rank;
                    board_ready = true;
                    if (rank != 4) throw_number_error("Wrong number of ranks in board, it was", rank);
                    if (file != 4) throw_number_error("Wrong number of files in board, it was", file);
                    i = house_end;
                    continue;
                case '/':
                    ++rank;
                    if (file != 4) throw_number_error("A Line ended with wrong size, it was", file);
                    file = 0;
                    continue;
                case 'p':
                    m_board.overwrite(const_piece::black_pawn, Board::get_index(rank, file++));
                    ++pawn_count;
                    continue;
                case 'P':
                    m_board.overwrite(const_piece::white_pawn, Board::get_index(rank, file++));
                    ++pawn_count;
                    continue;
                case 'f':
                    // Assumption regarding promoted peaced: Always the latest added pieces are promoted.
                    // Missing information to differentiate
                    m_board.overwrite(const_piece::black_ferz | (ferz_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++ferz_count;
                    continue;
                case 'F':
                    m_board.overwrite(const_piece::white_ferz | (ferz_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++ferz_count;
                    continue;
                case 'h':
                    m_board.overwrite(const_piece::black_hors | (hors_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++hors_count;
                    continue;
                case 'H':
                    m_board.overwrite(const_piece::white_hors | (hors_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++hors_count;
                    continue;
                case 'w':
                    m_board.overwrite(const_piece::black_wazir | (wazir_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++wazir_count;
                    continue;
                case 'W':
                    m_board.overwrite(const_piece::white_wazir | (wazir_count >= 2 ? PROMOTED : 0),
                                    Board::get_index(rank, file++));
                    ++wazir_count;
                    continue;
                case 'k':
                    m_board.overwrite(const_piece::black_king, Board::get_index(rank, file++));
                    ++king_count;
                    continue;
                case 'K':
                    m_board.overwrite(const_piece::white_king, Board::get_index(rank, file++));
                    ++king_count;
                    continue;
                case '1':
                case '2':
                case '3':
                case '4':
                    for (uint8_t j = 0; j < c - '0'; ++j) {
                        m_board.overwrite(const_piece::null_piece, Board::get_index(rank, file++));
                    }
                    if (file > 4) throw_number_error("A Rank has too much files! It should be", Board::SIZE);
                    continue;

                default:
                    throw_character_error("Invalid character in Board section", "p/f/h/w/k/P/F/H/W/K", c);
            }
        }
        if (!turn_ready) {
            // 1. check correct turn character
            if (tiny_fen_len <= i + 1) throw_missing_section_error("House");
            if (tiny_fen[i + 1] != ' ') throw_character_error("Turn section must end with space", " ", tiny_fen[i + 1]);
            ++i;
            if (c != 'w' and c != 'b')
                throw_character_error("Wrong argument. Cant determine whose turn it is", "w/b", c);
            m_is_whites_turn = (c == 'w');
            turn_ready = true;

            // 2. Check that the right amount of pieces got added to the game
            const uint8_t total_count = pawn_count + ferz_count + hors_count + wazir_count + king_count;
            const bool cond1 = total_count == 10;
            const bool cond2 = pawn_count <= 2;
            const bool cond3 = ferz_count >= 2 and ferz_count <= 4;
            const bool cond4 = hors_count >= 2 and hors_count <= 4;
            const bool cond5 = wazir_count >= 2 and wazir_count <= 4;
            const bool cond6 = king_count == 2;
            if (!cond1 or !cond2 or !cond3 or !cond4 or !cond5 or !cond6) {
                std::stringstream err_msg;
                err_msg << "Incorrect number of pieces:\n"
                        << "\tPawn " << +pawn_count << "\n"
                        << "\tFerz " << +ferz_count << "\n"
                        << "\tHors " << +hors_count << "\n"
                        << "\tWazir " << +wazir_count << "\n"
                        << "\tKing " << +king_count << ".";
                throw_general_error(err_msg.str());
            }
            continue;
        }
        if (!promotion_ready) {
            switch (c) {
                case '-':
                    // skip following whitespace
                    if (tiny_fen_len <= i + 1) throw_missing_section_error("Turn");
                    if (tiny_fen[i + 1] != ' ')
                        throw_character_error("Promotion section must end with space", " ", tiny_fen[i + 1]);
                    ++i;
                    promotion_ready = true;
                    continue;
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                    if (tiny_fen_len <= i + 2) throw_missing_section_error("Promotion");
                    if (tiny_fen[i + 1] < '1' or tiny_fen[i + 1] > '4')
                        throw_character_error("Promotion location incorrect (example: 'a3')", "1/2/3/4", c);
                    temp = Board::get_index(tiny_fen[i + 1] - '1', tiny_fen[i] - 'a');
                    m_board.overwrite(m_board.get_piece(temp) | PROMOTED, temp);
                    if ((++promotion_count + pawn_count) > 2)
                        throw_general_error("Only two pawns (max) can be promoted!");
                    if (tiny_fen[i + 2] != ' ')
                        throw_character_error("Promotion section must end with space", " ", tiny_fen[i + 2]);
                    ++i;  // skip space character
                    continue;
                default:
                    throw_character_error("Invalid character in Promotion section", "a1/a2/../d3/d4", c);
            }
        }

        if (tiny_fen_len <= i + 1) throw_missing_section_error("Promotion");
        const std::string remaining_fen = tiny_fen.substr(i, tiny_fen_len - i);
        std::stringstream sstream{remaining_fen};
        if (!(sstream >> m_halfmove_counter) or m_halfmove_counter < 0)
            throw_character_error("Could ot convert Halfmove Counter to number", "NUMBER", sstream.str()[0]);
        if (!(sstream >> m_fullmove_counter) or m_fullmove_counter < 0)
            throw_character_error("Could ot convert Fullmove Counter to number", "NUMBER", sstream.str()[0]);
        if (m_halfmove_counter / 2 > m_fullmove_counter) {
            throw_general_error("Invalid relation between counters. There cant be more Halfmoves then 2 * Fullmoves");
        }
        // Done
        break;
    }
}
