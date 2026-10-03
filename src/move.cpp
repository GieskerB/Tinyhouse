#include "../inc/move.hpp"

#include "../inc/board.hpp"

#define INDEX_SHIFT(index, base) (index - base)
#define SHIFT(bitmap, index, base) \
    (INDEX_SHIFT(index, base) < 0) ? bitmap >> -INDEX_SHIFT(index, base) : bitmap << INDEX_SHIFT(index, base)
#define FILE(i) Board::get_file(i)
#define RANK(i) Board::get_rank(i)
#define BSIZE Board::SIZE

/**
 * BITMAP ORDERING d1,c1,b1,a1'd2,c2,b2,a2'd3,c3,b3,a3'd4,c4,b4,a4'
 */

/**
 * List of all bitmaps for each piece on every tile to see where it can move / capture
 * Every 16 values build a group for one piece. Index 0 is top left and index 15 is bottom right
 * This is the list of the pieces to access
 * - white pawn
 * - black pawn
 * - ferz
 * - hors
 * - wazir
 * - king
 */
constexpr auto possible_move_bitmap = [] {
    constexpr uint8_t size_sq = BSIZE * BSIZE;
    std::array<uint16_t, size_sq * 6> array{0};
    // assume king is positioned on b2 (or c3)
    constexpr int8_t b2 = 9, c3 = 6;
    // Fill 1st 16 slots with WHITE PAWN
    constexpr uint16_t right_pawn_white_b2 = 0b0000'0000'0110'0000;
    constexpr uint16_t left_pawn_white_b2 = 0b0000'0000'0011'0000;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 0] |= FILE(i) > 0 ? SHIFT(left_pawn_white_b2, i, b2) : 0b0;
        array[i + size_sq * 0] |= FILE(i) < BSIZE - 1 ? SHIFT(right_pawn_white_b2, i, b2) : 0b0;
    }
    // Fill 2nd 16 slots with BLACK PAWN
    constexpr uint16_t right_pawn_black_b2 = 0b0110'0000'0000'0000;
    constexpr uint16_t left_pawn_black_b2 = 0b0011'0000'0000'0000;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 1] |= FILE(i) > 0 ? SHIFT(left_pawn_black_b2, i, b2) : 0b0;
        array[i + size_sq * 1] |= FILE(i) < BSIZE - 1 ? SHIFT(right_pawn_black_b2, i, b2) : 0b0;
    }
    // Fill 3rd 16 slots with FERZ
    constexpr uint16_t right_ferz_b2 = 0b0100'0000'0100'0000;
    constexpr uint16_t left_ferz_b2 = 0b0001'0000'0001'0000;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 2] |= FILE(i) > 0 ? SHIFT(left_ferz_b2, i, b2) : 0b0;
        array[i + size_sq * 2] |= FILE(i) < BSIZE - 1 ? SHIFT(right_ferz_b2, i, b2) : 0b0;
    }
    // Fill 4th 16 slots with HORS
    constexpr uint16_t right1_hors_b2 =  0b0000'0000'0000'0100;
    constexpr uint16_t right2_hors_b2 =  0b1000'0000'1000'0100;
    constexpr uint16_t left_hors_b2 =  0b0000'0000'0000'0001;
    constexpr uint16_t right_hors_c3 =   0b1000'0000'0000'0000;
    constexpr uint16_t left1_hors_c3 = 0b0010'0000'0000'0000;
    constexpr uint16_t left2_hors_c3 = 0b0010'0001'0000'0001;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 3] |= FILE(i) < BSIZE - 1 ? SHIFT(right1_hors_b2, i, b2) : 0b0;
        array[i + size_sq * 3] |= FILE(i) < BSIZE - 2 ? SHIFT(right2_hors_b2, i, b2) : 0b0;
        array[i + size_sq * 3] |= FILE(i) > 0 ? SHIFT(left_hors_b2, i, b2) : 0b0;
        array[i + size_sq * 3] |= FILE(i) < BSIZE -1 ? SHIFT(right_hors_c3, i, c3) : 0b0;
        array[i + size_sq * 3] |= FILE(i) > 0  ? SHIFT(left1_hors_c3, i, c3) : 0b0;
        array[i + size_sq * 3] |= FILE(i) > 1  ? SHIFT(left2_hors_c3, i, c3) : 0b0;
    }
    // Fill 5th 16 slots with WAZIR
    constexpr uint16_t right_wazir_b2 = 0b0010'0100'0010'0000;
    constexpr uint16_t left_wazir_b2 = 0b0010'0001'0010'0000;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 4] |= FILE(i) > 0 ? SHIFT(left_wazir_b2, i, b2) : 0b0;
        array[i + size_sq * 4] |= FILE(i) < BSIZE - 1 ? SHIFT(right_wazir_b2, i, b2) : 0b0;
    }
    // Fill 6th 16 slots with KING
    constexpr uint16_t right_king_b2 = 0b0110'0100'0110'0000;
    constexpr uint16_t left_king_b2 = 0b0011'0001'0011'0000;
    for (uint8_t i = 0; i < size_sq; ++i) {
        array[i + size_sq * 5] |= FILE(i) > 0 ? SHIFT(left_king_b2, i, b2) : 0b0;
        array[i + size_sq * 5] |= FILE(i) < BSIZE - 1 ? SHIFT(right_king_b2, i, b2) : 0b0;
    }
    return array;
}();


static uint8_t to_bitmap_index(piece piece, uint8_t piece_index) {
    uint8_t offset = 0;
    switch (piece & PIECE_MASK) {
        case BLACK | PAWN:
            offset = 1;
            break;
        case WHITE | FERZ:
        case BLACK | FERZ:
            offset = 2;
            break;
        case WHITE | HORS:
        case BLACK | HORS:
            offset = 3;
            break;
        case WHITE | WAZIR:
        case BLACK | WAZIR:
            offset = 4;
            break;
        case WHITE | KING:
        case BLACK | KING:
            offset = 5;
            break;
    }
    return offset * Board::BSIZE * Board::BSIZE + piece_index;
}

static uint16_t valid_pawn_move_offset(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t file_bitmap = 0b0001'0001'0001'0001 << FILE(piece_index);
    const uint16_t move_bitmap = file_bitmap & ~board.get_bitmap() & bitmap;
    const uint8_t other_color = (board.get_piece(piece_index) & COLOR_MASK) ^ COLOR_MASK;
    const uint16_t other_color_bitmap = board.get_bitmap(other_color);
    const uint16_t capture_bitmap = ((file_bitmap << 1) | (file_bitmap >> 1)) & other_color_bitmap & bitmap;

    return capture_bitmap | move_bitmap;
}

static uint16_t valid_hors_move_offset(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t piece_bitmap = Board::index_to_bitmap(piece_index);
    const uint16_t all_bitmap = board.get_bitmap();
    const uint16_t left_block = (piece_bitmap >> 1) & all_bitmap;
    if (left_block != 0) {  // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap >> 6) | (piece_bitmap << 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t top_block = (piece_bitmap >> Board::BSIZE) & all_bitmap;
    if (top_block != 0) {  // Can not move to the top
        const uint16_t possible_targets = (piece_bitmap >> 9) | (piece_bitmap >> 7);
        bitmap &= ~possible_targets;
    }
    const uint16_t right_block = (piece_bitmap << 1) & all_bitmap;
    if (right_block != 0) {  // Can not move to the right
        const uint16_t possible_targets = (piece_bitmap << 6) | (piece_bitmap >> 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t bottom_block = (piece_bitmap << Board::BSIZE) & all_bitmap;
    if (bottom_block != 0) {  // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap << 9) | (piece_bitmap << 7);
        bitmap &= ~possible_targets;
    }
    return bitmap;
}

uint16_t direct_check_check(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const piece piece = board.get_piece(piece_index);
    if((piece & ID_MASK) == KING) return bitmap;
    // const uint8_t king_index = board.find_king(piece & COLOR_MASK);
    return bitmap;
}

uint16_t valid_move_bitmap(const Board& board, uint8_t piece_index) {
    // Get all possible moves
    const piece piece = board.get_piece(piece_index);
    if (piece == const_piece::null_piece) return 0;
    const uint8_t bitmap_index = to_bitmap_index(piece, piece_index);
    uint16_t valid_moves = possible_move_bitmap[bitmap_index];

    // Filter out those that would be a self capture:
    uint16_t own_pieces_bitmap = board.get_bitmap(piece & COLOR_MASK);
    valid_moves &= ~own_pieces_bitmap;

    // Filter extra rules:
    if ((piece & ID_MASK) == PAWN) return valid_pawn_move_offset(board, piece_index, valid_moves);
    if ((piece & ID_MASK) == HORS) return valid_hors_move_offset(board, piece_index, valid_moves);

    return valid_moves;
}

static move make_move(uint16_t destination, uint16_t source) {
    return (destination << 4) | source;
}


std::vector<move> valid_moves(const Board& board, uint8_t piece_index) {
    std::vector<move> moves;
    // board.get_piece(piece_index);
    const uint16_t move_bitmap = valid_move_bitmap(board, piece_index);

    uint16_t current_bitmap = 0b1;
    for(uint8_t i = 0; i< 16; ++i) {
        if((current_bitmap & move_bitmap) != 0) {
            moves.push_back(make_move(i, piece_index));
        }
        current_bitmap <<= 1;
    }

    return moves;
}
