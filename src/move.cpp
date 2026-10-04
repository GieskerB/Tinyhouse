#include "../inc/move.hpp"
#include "../inc/board.hpp"

#define INDEX_SHIFT(index, base) (index - base)
#define SHIFT(bitmap, index, base) \
    (INDEX_SHIFT(index, base) < 0) ? bitmap >> -INDEX_SHIFT(index, base) : bitmap << INDEX_SHIFT(index, base)

/**
 * BITMAP ORDERING from msb to lsb (bottom right to top left - reversed):
 * d1,c1,b1,a1'd2,c2,b2,a2'd3,c3,b3,a3'd4,c4,b4,a4
 */

/**
 * List of all bitmaps for each piece on every tile to see where it can move / capture.
 * Every 16 values build a group for one piece. Index 0 is top left and index 15 is bottom right.
 * Each original bitmap represents the possible tiles to move to / capture on, assuming the piece is
 * located on b2 (or c3). Then the bitmap is shifted depending on the current pieces distance to b2.
 */

constexpr auto precomputed_white_pawn_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9;
    constexpr uint16_t right_b2 = 0b0000'0000'0110'0000;
    constexpr uint16_t left_b2 = 0b0000'0000'0011'0000;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_b2, i, b2) : 0b0;
    }
    return array;
}();

constexpr auto precomputed_black_pawn_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9;
    constexpr uint16_t right_b2 = 0b0110'0000'0000'0000;
    constexpr uint16_t left_b2 = 0b0011'0000'0000'0000;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_b2, i, b2) : 0b0;
    }
    return array;
}();

constexpr auto precomputed_ferz_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9;
    constexpr uint16_t right_b2 = 0b0100'0000'0100'0000;
    constexpr uint16_t left_b2 = 0b0001'0000'0001'0000;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_b2, i, b2) : 0b0;
    }
    return array;
}();

constexpr auto precomputed_hors_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9, c3 = 6;
    constexpr uint16_t right1_b2 = 0b0000'0000'0000'0100;
    constexpr uint16_t right2_b2 = 0b1000'0000'1000'0100;
    constexpr uint16_t left_b2 = 0b0000'0000'0000'0001;
    constexpr uint16_t right_c3 = 0b1000'0000'0000'0000;
    constexpr uint16_t left1_c3 = 0b0010'0000'0000'0000;
    constexpr uint16_t left2_c3 = 0b0010'0001'0000'0001;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right1_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 2 ? SHIFT(right2_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_c3, i, c3) : 0b0;
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left1_c3, i, c3) : 0b0;
        array[i] |= Board::get_file(i) > 1 ? SHIFT(left2_c3, i, c3) : 0b0;
    }
    return array;
}();

constexpr auto precomputed_wazir_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9;
    constexpr uint16_t right_b2 = 0b0010'0100'0010'0000;
    constexpr uint16_t left_b2 = 0b0010'0001'0010'0000;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_b2, i, b2) : 0b0;
    }
    return array;
}();

constexpr auto precomputed_king_moves = [] {
    std::array<uint16_t, Board::SIZE * Board::SIZE> array{0};
    constexpr int8_t b2 = 9;
    constexpr uint16_t right_b2 = 0b0110'0100'0110'0000;
    constexpr uint16_t left_king_b2 = 0b0011'0001'0011'0000;
    for (uint8_t i = 0; i < array.size(); ++i) {
        array[i] |= Board::get_file(i) > 0 ? SHIFT(left_king_b2, i, b2) : 0b0;
        array[i] |= Board::get_file(i) < Board::SIZE - 1 ? SHIFT(right_b2, i, b2) : 0b0;
    }
    return array;
}();

static uint16_t precomputed_bitmap(piece piece, uint8_t piece_index) {
    switch (piece & PIECE_MASK) {
        case WHITE | PAWN:
            return precomputed_white_pawn_moves[piece_index];
        case BLACK | PAWN:
            return precomputed_black_pawn_moves[piece_index];
        case WHITE | FERZ:
        case BLACK | FERZ:
            return precomputed_ferz_moves[piece_index];
        case WHITE | HORS:
        case BLACK | HORS:
            return precomputed_hors_moves[piece_index];
        case WHITE | WAZIR:
        case BLACK | WAZIR:
            return precomputed_wazir_moves[piece_index];
        case WHITE | KING:
        case BLACK | KING:
            return precomputed_king_moves[piece_index];
        default:
            return 0;
    }
}

static uint16_t pseudo_legal_pawn_move_bitmap(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t file_bitmap = 0b0001'0001'0001'0001 << Board::get_file(piece_index);
    const uint16_t move_bitmap = file_bitmap & ~board.get_bitmap() & bitmap;
    const uint8_t other_color = (board.get_piece(piece_index) & COLOR_MASK) ^ COLOR_MASK;
    const uint16_t other_color_bitmap = board.get_bitmap(other_color);
    const uint16_t capture_bitmap = ((file_bitmap << 1) | (file_bitmap >> 1)) & other_color_bitmap & bitmap;

    return capture_bitmap | move_bitmap;
}

static uint16_t pseudo_legal_hors_move_bitmap(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t piece_bitmap = Board::index_to_bitmap(piece_index);
    const uint16_t all_bitmap = board.get_bitmap();
    const uint16_t left_block = (piece_bitmap >> 1) & all_bitmap;
    if (left_block != 0) {  // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap >> 6) | (piece_bitmap << 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t top_block = (piece_bitmap >> Board::Board::SIZE) & all_bitmap;
    if (top_block != 0) {  // Can not move to the top
        const uint16_t possible_targets = (piece_bitmap >> 9) | (piece_bitmap >> 7);
        bitmap &= ~possible_targets;
    }
    const uint16_t right_block = (piece_bitmap << 1) & all_bitmap;
    if (right_block != 0) {  // Can not move to the right
        const uint16_t possible_targets = (piece_bitmap << 6) | (piece_bitmap >> 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t bottom_block = (piece_bitmap << Board::Board::SIZE) & all_bitmap;
    if (bottom_block != 0) {  // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap << 9) | (piece_bitmap << 7);
        bitmap &= ~possible_targets;
    }
    return bitmap;
}

uint16_t direct_check_check(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const piece piece = board.get_piece(piece_index);
    if ((piece & ID_MASK) == KING) return bitmap;
    // const uint8_t king_index = board.find_king(piece & COLOR_MASK);
    return bitmap;
}

uint16_t pseudo_legal_move_bitmap(const Board& board, uint8_t piece_index) {
    // Get all possible moves
    const piece piece = board.get_piece(piece_index);
    if (piece == const_piece::null_piece) return 0;
    uint16_t pseudo_legal_moves = precomputed_bitmap(piece, piece_index);

    // Filter out those that would be a self capture:
    uint16_t own_pieces_bitmap = board.get_bitmap(piece & COLOR_MASK);
    pseudo_legal_moves &= ~own_pieces_bitmap;

    // Filter extra rules:
    if ((piece & ID_MASK) == PAWN) return pseudo_legal_pawn_move_bitmap(board, piece_index, pseudo_legal_moves);
    if ((piece & ID_MASK) == HORS) return pseudo_legal_hors_move_bitmap(board, piece_index, pseudo_legal_moves);

    return pseudo_legal_moves;
}

static move make_move(uint16_t destination, uint16_t source) { return (destination << 4) | source; }

std::vector<move> legal_moves(const Board& board, uint8_t piece_index) {
    std::vector<move> moves;
    // board.get_piece(piece_index);
    const uint16_t move_bitmap = pseudo_legal_move_bitmap(board, piece_index);

    uint16_t current_bitmap = 0b1;
    for (uint8_t i = 0; i < 16; ++i) {
        if ((current_bitmap & move_bitmap) != 0) {
            moves.push_back(make_move(i, piece_index));
        }
        current_bitmap <<= 1;
    }

    return moves;
}
