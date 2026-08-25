#include "../inc/board.hpp"

piece Board::get_piece(uint8_t index) const {
    assert(index < TILE_COUNT * TILE_COUNT);
    return board[index];
}

uint8_t Board::get_index(uint8_t file, uint8_t rank) {
    assert(rank < TILE_COUNT);
    assert(file < TILE_COUNT);
    return rank * TILE_COUNT + file;
}
