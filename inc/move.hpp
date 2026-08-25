#ifndef MOVE_HPP
#define MOVE_HPP

#include <vector>
#include <cstdint>

// Bitmap [F = flag of move, D = destination of move, S = source of move]
// 000000FF DDDDSSSS
typedef uint16_t move;

#include "board.hpp"

constexpr uint16_t FROM_HOUSE = 0b000100000000;
constexpr uint16_t CAPTURE = 0b001000000000;
constexpr uint16_t PROMOTION = 0b010000000000;
constexpr uint16_t CHECK = 0b100000000000;

std::vector<int8_t> MOVES(const Board& board, uint8_t piece_index);

std::vector<int8_t> ATTACKS(const Board& board, uint8_t piece_index);

// std::vector<move> moves;
// if (!is_black) {
//     // No bound checking since pawns cant be on last
//     uint8_t straight_up = index - Board::TILE_COUNT;
//     if (board.get_piece(straight_up) == const_piece::null_piece) {
//         uint8_t flags = 0;
//         if (Board::get_rank(straight_up) == 0)
//             flags |= PROMOTION;
//         else {
//             const bool black_king_left_check =
//                 Board::get_file(index) > 0 and
//                 board.get_piece(straight_up - Board::TILE_COUNT - 1) == const_piece::black_king;
//             const bool black_king_right_check =
//                 Board::get_file(index) < Board::TILE_COUNT - 1 and
//                 board.get_piece(straight_up - Board::TILE_COUNT + 1) == const_piece::black_king;
//             if (black_king_left_check or black_king_right_check) flags |= CHECK;
//         }
//         moves.push_back(create_move(index, straight_up,flags));
//     }
//     if (Board::get_file(index) > 0 and board.get_piece(straight_up) & COLOR_MASK == BLACK) {

//     }
// } else {

// }

// return moves;

// TODO Check for pinned pieces
// inline std::vector<move> LEGAL_MOVES(const Board& board, uint8_t piece_index) {
//     const bool is_black = p & BLACK;
//     switch (p & TYPE_MASKE) {
//         case PAWN:
//             return pawn_moves(is_black, board, piece_index);
//         case FERZ:
//             return ferz_moves(is_black, board, piece_index);
//         case HORS:
//             return horsg_moves(is_black, board, piece_index);
//         case WAZIR:
//             return wazir_moves(is_black, board, piece_index);
//         case KING:
//             return king_moves(is_black, board, piece_index);
//         default:
//             return {};
//     }
// }

#endif