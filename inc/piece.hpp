#ifndef PIECE_HPP
#define PIECE_HPP

#include <cstdint>

typedef uint8_t piece_t;
// Bitmap {
//   F = Flags,
//   I = id of piece,
//   C = color of piece
// }
// -> 00FCCIII

namespace piece {

namespace id {
constexpr piece_t PAWN = 0b000;
constexpr piece_t FERZ = 0b001;
constexpr piece_t HORS = 0b010;
constexpr piece_t WAZIR = 0b011;
constexpr piece_t KING = 0b100;
}  // namespace id
namespace color {
constexpr piece_t WHITE = 0b1000;
constexpr piece_t BLACK = 0b10000;
}  // namespace color

namespace flag {
constexpr piece_t PROMOTED = 0b100000;
}

namespace mask {
constexpr piece_t ID = 0b00111;
constexpr piece_t COLOR = 0b11000;
constexpr piece_t PIECE = ID | COLOR;
}  // namespace mask

static constexpr piece_t null = 0b0;

static constexpr piece_t white_ferz = id::FERZ | color::WHITE;
static constexpr piece_t white_hors = id::HORS | color::WHITE;
static constexpr piece_t white_wazir = id::WAZIR | color::WHITE;
static constexpr piece_t white_pawn = id::PAWN | color::WHITE;
static constexpr piece_t white_king = id::KING | color::WHITE;

static constexpr piece_t black_ferz = id::FERZ | color::BLACK;
static constexpr piece_t black_hors = id::HORS | color::BLACK;
static constexpr piece_t black_wazir = id::WAZIR | color::BLACK;
static constexpr piece_t black_pawn = id::PAWN | color::BLACK;
static constexpr piece_t black_king = id::KING | color::BLACK;

// dirty fix for PIECE_INDEX function...
static constexpr piece_t overlay_dot = 0b1000000;
static constexpr piece_t overlay_circle = 0b10000000;

}  // namespace piece

inline uint8_t PIECE_INDEX(piece_t p) {
    // TODO: Dirty fix FIX
    if (p == 0b1000000) return 10;
    if (p == 0b10000000) return 11;

    const uint8_t piece_index = (p & piece::mask::ID);
    const uint8_t color_index = (p & piece::mask::COLOR);
    const uint8_t index = piece_index | ((color_index & piece::color::BLACK) >> 1);
    return index > 4 ? index - 3 : index;  // Close the gape of (5,6,7) without texture
}

#endif
