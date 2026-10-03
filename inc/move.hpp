#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include <vector>

// Bitmap [
// P = promoted piece,
// C = captured piece,
// F = flag of move,
// D = destination of move,
// S = source of move
// -> 00000000 PPPPPPCC CCCCFFFF DDDDSSSS
typedef uint32_t move;

#include "board.hpp"

constexpr uint16_t FROM_HOUSE = 0b000100000000;
constexpr uint16_t CAPTURE = 0b001000000000;
constexpr uint16_t PROMOTION = 0b010000000000;
constexpr uint16_t CHECK = 0b100000000000;

uint16_t valid_move_bitmap(const Board& board, uint8_t piece_index);
std::vector<move> valid_moves(const Board& board, uint8_t piece_index);

#endif
