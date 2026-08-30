#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <cassert>
#include <string>

class Board;

#include "move.hpp"
#include "piece.hpp"

class Board {
    std::array<piece, 16> board{
        const_piece::black_ferz, const_piece::black_hors,  const_piece::black_wazir, const_piece::black_king,  // rank 4
        const_piece::null_piece, const_piece::null_piece,  const_piece::null_piece,  const_piece::black_pawn,  // rank 3
        const_piece::white_pawn, const_piece::null_piece,  const_piece::null_piece,  const_piece::null_piece,  // rank 2
        const_piece::white_king, const_piece::white_wazir, const_piece::white_hors,  const_piece::white_ferz   // rank 1
    };

   public:

    static constexpr uint8_t SIZE = 4;

    void overwrite(piece piece, uint8_t index);

    piece get_piece(uint8_t index) const;

    void make_move(move move);
    void undo_move(move move);

    static constexpr uint8_t get_index(uint8_t rank, uint8_t file) {
        assert(rank < SIZE);
        assert(file < SIZE);
        return rank * SIZE + file;
    }
    static constexpr uint8_t get_file(uint8_t index) {
        assert(index < SIZE * SIZE);
        return index % SIZE;
    }
    static constexpr uint8_t get_rank(uint8_t index) {
        assert(index < SIZE * SIZE);
        return index / SIZE;
    }
};

#endif
