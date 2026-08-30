#ifndef HOUSE_HPP
#define HOUSE_HPP

#include <array>
#include <cassert>

class House;

#include "piece.hpp"

class House {
    // house cant hold to king, so only 8 instead of 10 places.
    std::array<uint8_t, 8> m_house{0};

   public:
    static constexpr uint8_t HOUSE_SIZE = 8;

    void push(piece);
    piece pop(uint8_t);

    bool is_empty() const;

    uint8_t count(uint8_t) const;

    static uint8_t piece_to_index(piece);
    static piece index_to_piece(uint8_t);
};

#endif
