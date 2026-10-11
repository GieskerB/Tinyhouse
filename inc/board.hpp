#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <cassert>
#include <string>

class Board;

#include "piece.hpp"

class Board {
   public:
    static constexpr uint8_t SIZE = 4;

   private:
    std::array<piece_t, SIZE * SIZE> board{
        piece::black_ferz, piece::black_hors,  piece::black_wazir, piece::black_king,  // rank 4
        piece::null,       piece::null,        piece::null,        piece::black_pawn,  // rank 3
        piece::white_pawn, piece::null,        piece::null,        piece::null,        // rank 2
        piece::white_king, piece::white_wazir, piece::white_hors,  piece::white_ferz   // rank 1
    };

   public:
    void overwrite(piece_t piece, uint8_t index);

    void make_move(uint8_t from, uint8_t to);

    piece_t get_piece(uint8_t index) const;

    uint8_t find_king(uint8_t color) const;

    uint16_t get_bitmap(piece_t piece, uint8_t color) const;
    uint16_t get_bitmap(uint8_t color) const;
    uint16_t get_bitmap() const;

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
    static constexpr uint16_t index_to_bitmap(uint8_t index) {
        assert(index < SIZE * SIZE);
        return 0b1 << (index);
    }
};

#endif
