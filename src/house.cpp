#include "../inc/house.hpp"

void House::push(piece p) {
    uint8_t index = piece_to_index(p);
    assert(m_house[index] == 0 or m_house[index] == 1);
    ++m_house[index];
}

piece House::pop(uint8_t index) {
    assert(index < HOUSE_SIZE);
    assert(m_house[index] == 1 or m_house[index] == 2);
    --m_house[index];
    return index_to_piece(index);
}

bool House::is_empty() const {
    for (const uint8_t count: m_house ) {
        if(count > 0 )return false;
    }
    return true;
}

uint8_t House::count(uint8_t index) const {
    assert(index < HOUSE_SIZE);
    return m_house[index];
}

uint8_t House::piece_to_index(piece p) {
    assert(p != const_piece::white_king and p != const_piece::black_king);
    uint8_t index = PIECE_INDEX(p);
    if (index > 3)
        return index - 1;  // close gape of king.
    else
        return index;
}
piece House::index_to_piece(uint8_t index) {
    assert(index < HOUSE_SIZE);
    if (index > 3)
        return static_cast<piece>(index + 1 + 3);  // Fill in the king gap and (5,6,7)
    else
        return static_cast<piece>(index);
}
