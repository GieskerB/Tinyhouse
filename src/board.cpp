#include "../inc/board.hpp"

void Board::overwrite(piece p, uint8_t index) {
    assert(index < SIZE * SIZE);
    board[index] = p;
}

piece Board::get_piece(uint8_t index) const {
    assert(index < SIZE * SIZE);
    return board[index];
}

uint16_t Board::get_bitmap(piece piece, uint8_t color) {
    assert(color == WHITE or color == BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if(((board[i] & PIECE_MASK) == (piece & piece_mask)) and ((board[i] & COLOR_MASK) == color)) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap(uint8_t color) {
    assert(color == WHITE or color == BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if((board[i] & COLOR_MASK) == color) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap() {
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if(board[i] != const_piece::null_piece) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}