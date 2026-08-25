#include "../inc/board.hpp"

piece Board::get_piece(uint8_t index) const {
    assert(index < TILE_COUNT * TILE_COUNT);
    return board[index];
}

bool Board::is_highlighted(uint8_t index) const {
    assert(index < TILE_COUNT * TILE_COUNT);
    for(uint8_t i = 0; i< highlighted_tile.size(); ++i) {
        if (highlighted_tile[i] == index) return true;
    }
    return false;
}

void Board::select_piece(uint8_t index) {
    assert(index < TILE_COUNT * TILE_COUNT);
    highlighted_tile[0] = static_cast<int8_t>(index);
}
void Board::unselect_piece() {
    highlighted_tile[0] = -1;
}

void Board::make_move(move move) {
    highlighted_tile[1] = move; // PLACEHOLDER
}

uint8_t Board::get_index(uint8_t file, uint8_t rank) {
    assert(rank < TILE_COUNT);
    assert(file < TILE_COUNT);
    return rank * TILE_COUNT + file;
}

uint8_t Board::get_file(uint8_t index) {
    assert(index < TILE_COUNT * TILE_COUNT);
    return index % TILE_COUNT;
}

uint8_t Board::get_rank(uint8_t index) {
    assert(index < TILE_COUNT * TILE_COUNT);
    return index / TILE_COUNT;
}