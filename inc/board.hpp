#ifndef BOARD_HPP
#define BOARD_HPP

#include <SDL3/SDL_pixels.h>

#include <array>
#include <cassert>

#include "piece.hpp"

class Board {
    std::array<piece, 16> board{
        const_piece::black_ferz, const_piece::black_hors,  const_piece::black_wazir, const_piece::black_king,  // rank 4
        const_piece::null_piece, const_piece::null_piece,  const_piece::null_piece,  const_piece::black_pawn,  // rank 3
        const_piece::white_pawn, const_piece::null_piece,  const_piece::null_piece,  const_piece::null_piece,  // rank 2
        const_piece::white_king, const_piece::white_wazir, const_piece::white_hors,  const_piece::white_ferz   // rank 1
    };

   public:
    static constexpr uint8_t TILE_COUNT = 4;

    piece get_piece(uint8_t index) const;

    static uint8_t get_index(uint8_t file, uint8_t rank);
};

#endif