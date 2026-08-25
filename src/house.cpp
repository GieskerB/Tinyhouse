#include "../inc/house.hpp"

void House::push(piece p) {
    p ^= WHITE | BLACK;  // Switch color
    uint8_t index = get_index(p);
    assert(house[index] == 0 or house[index] == 1);
    ++house[index];
}

piece House::pop(uint8_t index) {
    assert(index < HOUSE_SIZE);
    assert(house[index] == 1 or house[index] == 2);
    --house[index];
    return index_to_piece(index);
}

uint8_t House::count(uint8_t index) const {
    assert(index < HOUSE_SIZE);
    return house[index];
}

void House::select(uint8_t index) {
    assert(index < HOUSE_SIZE);
    selected_index = index;
}
void House::unselect() {
    selected_index = -1;
}

bool House::is_highlighted(uint8_t index) const { return index == selected_index; }

uint8_t House::get_index(piece p) {
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