#include "../inc/board.hpp"

void Board::overwrite(piece p, uint8_t index) {
    assert(index < SIZE * SIZE);
    board[index] = p;
}

piece Board::get_piece(uint8_t index) const {
    assert(index < SIZE * SIZE);
    return board[index];
}
