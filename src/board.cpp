#include "../inc/board.hpp"

void Board::overwrite(piece p, uint8_t index) {
    assert(index < Board::SIZE * Board::SIZE);
    board[index] = p;
}

void Board::make_move(uint8_t from, uint8_t to) {
    assert(from < Board::SIZE * Board::SIZE);
    assert(to < Board::SIZE * Board::SIZE);
    assert(from != to);
    assert(get_piece(from) != const_piece::null_piece);
    board[to] = board[from];
    board[from] = const_piece::null_piece;
}

piece Board::get_piece(uint8_t index) const {
    assert(index < Board::SIZE * Board::SIZE);
    return board[index];
}

uint8_t Board::find_king(uint8_t color) const {
    assert(color == WHITE or color == BLACK);
    const piece king = KING | color;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if(board[i] == king) return i;
    }
    return -1;
}

uint16_t Board::get_bitmap(piece piece, uint8_t color) const{
    assert(color == WHITE or color == BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if(((board[i] & PIECE_MASK) == (piece & piece_mask)) and ((board[i] & COLOR_MASK) == color)) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap(uint8_t color) const{
    assert(color == WHITE or color == BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if((board[i] & COLOR_MASK) == color) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap() const{
    uint16_t bitmap = 0, piece_mask = 0b1;
    for(uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if(board[i] != const_piece::null_piece) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}
