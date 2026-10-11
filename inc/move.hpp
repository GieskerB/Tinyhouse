#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include <vector>

#include "board.hpp"

typedef uint32_t move_t;
// Bitmap {
//   P = promoted piece,
//   C = captured piece,
//   F = flag of move,
//   D = destination of move,
//   S = source of move
// }
// -> 00000000 PPPPPPCC CCCCFFFF DDDDSSSS

namespace move {

namespace flag {
constexpr move_t FROM_HOUSE = 0b0001'0000'0000;
constexpr move_t CAPTURE = 0b0010'0000'0000;
constexpr move_t PROMOTION = 0b0100'0000'0000;
constexpr move_t CHECK = 0b1000'0000'0000;
}  // namespace flag

namespace mask {
constexpr move_t SOURCE = 0b0000'0000'0000'0000'0000'1111;
constexpr move_t DESTINATION = 0b0000'0000'0000'0000'1111'0000;
constexpr move_t FLAGS = 0b0000'0000'0000'1111'0000'0000;
constexpr move_t CAPTURED_PIECE = 0b0000'0011'1111'0000'0000'0000;
constexpr move_t PROMOTPED_PIECE = 0b1111'1100'0000'0000'0000'0000;
}  // namespace mask

}  // namespace move

uint16_t pseudo_legal_move_bitmap(const Board& board, uint8_t piece_index);
std::vector<move_t> legal_moves(const Board& board, uint8_t piece_index);

#endif
