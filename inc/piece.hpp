#ifndef PIECE_HPP
#define PIECE_HPP

#include <cstdint>

// Bitmap [I = id of piece, C = color of piece]
// 000CCIII
typedef uint8_t piece;

// piece id
constexpr uint8_t PAWN = 0b000;
constexpr uint8_t FERZ = 0b001;
constexpr uint8_t HORS = 0b010;
constexpr uint8_t WAZIR = 0b011;
constexpr uint8_t KING = 0b100;
// piece color
constexpr uint8_t WHITE = 0b1000, BLACK = 0b10000;

constexpr uint8_t TYPE_MASKE=0b00111, COLOR_MASK=0b11000;

inline uint8_t PIECE_INDEX(piece p) {
    const uint8_t piece_index = (p & TYPE_MASKE);
    const uint8_t color_index = (p & COLOR_MASK);
    const uint8_t index = piece_index | ((color_index & BLACK) >> 1);
    return index > 4 ? index - 3 : index;  // Close the gape of (5,6,7) without texture
}

namespace const_piece {

static constexpr piece null_piece = 0b0;

static constexpr piece white_ferz = FERZ | WHITE;
static constexpr piece white_hors = HORS | WHITE;
static constexpr piece white_wazir = WAZIR | WHITE;
static constexpr piece white_pawn = PAWN | WHITE;
static constexpr piece white_king = KING | WHITE;

static constexpr piece black_ferz = FERZ | BLACK;
static constexpr piece black_hors = HORS | BLACK;
static constexpr piece black_wazir = WAZIR | BLACK;
static constexpr piece black_pawn = PAWN | BLACK;
static constexpr piece black_king = KING | BLACK;
}  // namespace const_piece

#endif