#include "../inc/board.hpp"

void Board::overwrite(piece_t piece, uint8_t index) {
    assert(index < Board::SIZE * Board::SIZE);
    board[index] = piece;
}

void Board::make_move(uint8_t from, uint8_t to) {
    assert(from < Board::SIZE * Board::SIZE);
    assert(to < Board::SIZE * Board::SIZE);
    assert(from != to);
    assert(get_piece(from) != piece::null);
    board[to] = board[from];
    board[from] = piece::null;
}

piece_t Board::get_piece(uint8_t index) const {
    assert(index < Board::SIZE * Board::SIZE);
    return board[index];
}

uint8_t Board::find_king(uint8_t color) const {
    assert(color == piece::color::WHITE or color == piece::color::BLACK);
    const piece_t king = piece::id::KING | color;
    for (uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if (board[i] == king) return i;
    }
    return -1;
}

uint16_t Board::get_bitmap(piece_t piece, uint8_t color) const {
    assert(color == piece::color::WHITE or color == piece::color::BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for (uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if (((board[i] & piece::mask::PIECE) == (piece & piece_mask)) and ((board[i] & piece::mask::COLOR) == color))
            bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap(uint8_t color) const {
    assert(color == piece::color::WHITE or color == piece::color::BLACK);
    uint16_t bitmap = 0, piece_mask = 0b1;
    for (uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if ((board[i] & piece::mask::COLOR) == color) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}

uint16_t Board::get_bitmap() const {
    uint16_t bitmap = 0, piece_mask = 0b1;
    for (uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if (board[i] != piece::null) bitmap |= piece_mask;
        piece_mask <<= 1;
    }
    return bitmap;
}
