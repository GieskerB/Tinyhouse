#ifndef HOUSE_HPP
#define HOUSE_HPP

#include <array>
#include <cassert>

class House;

#include "piece.hpp"

class House {
   public:
    static constexpr uint8_t HOUSE_SIZE = 8;

   private:
    // house cant hold the king, so only 8 instead of 10 places.
    std::array<uint8_t, HOUSE_SIZE> m_house{0};

   public:
    void push(piece);
    piece pop(uint8_t);

    bool is_empty() const;

    uint8_t count(uint8_t) const;

    static uint8_t piece_to_index(piece);
    static piece index_to_piece(uint8_t);
};

#endif
