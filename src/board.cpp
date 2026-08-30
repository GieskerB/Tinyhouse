#include "../inc/board.hpp"

void Board::overwrite(piece p, uint8_t index) {
    assert(index < SIZE * SIZE);
    board[index] = p;
}

piece Board::get_piece(uint8_t index) const {
    assert(index < SIZE * SIZE);
    return board[index];
}

bool Board::is_highlighted(uint8_t index) const {
    assert(index < SIZE * SIZE);
    for (uint8_t i = 0; i < highlighted_tile.size(); ++i) {
        if (highlighted_tile[i] == index) return true;
    }
    return false;
}

void Board::select_piece(uint8_t index) {
    assert(index < SIZE * SIZE);
    highlighted_tile[0] = static_cast<int8_t>(index);
}
void Board::unselect_piece() { highlighted_tile[0] = -1; }

void Board::make_move(move move) {
    highlighted_tile[1] = move;  // PLACEHOLDER
}
