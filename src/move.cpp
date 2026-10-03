#include "../inc/move.hpp"

#include "../inc/board.hpp"

#define INDEX_SHIFT(index, base) (index - base)
#define SHIFT(bitmap, index, base) \
    (INDEX_SHIFT(index, base) < 0) ? bitmap >> -INDEX_SHIFT(index, base) : bitmap << INDEX_SHIFT(index, base)
#define FILE(i) Board::get_file(i)
#define RANK(i) Board::get_rank(i)

constexpr uint16_t reverse_16(uint16_t x) {
    x = (x & 0x5555) << 1 | (x & 0xAAAA) >> 1;
    x = (x & 0x3333) << 2 | (x & 0xCCCC) >> 2;
    x = (x & 0x0F0F) << 4 | (x & 0xF0F0) >> 4;
    x = (x & 0x00FF) << 8 | (x & 0xFF00) >> 8;
    return x;
}

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
    constexpr uint8_t piece_offset = Board::SIZE * Board::SIZE;
    std::array<uint16_t, piece_offset * 6> array{0};
    // assume default bitmap is centered on b2 (or c3)
    constexpr int8_t b2 = 9, c3 = 6;
    // Fill 1st 16 with WHITE PAWN
    constexpr uint16_t left_pawn_white_b2 = reverse_16(0b0000'1100'0000'0000);
    constexpr uint16_t right_pawn_white_b2 = reverse_16(0b0000'0110'0000'0000);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 0] |= FILE(i) > 0 ? SHIFT(left_pawn_white_b2, i, b2) : 0b0;
        array[i + piece_offset * 0] |= FILE(i) < (Board::SIZE - 1) ? SHIFT(right_pawn_white_b2, i, b2) : 0b0;
    }
    // Fill 2nd 16 with BLACK PAWN
    constexpr uint16_t left_pawn_black_b2 = reverse_16(0b0000'0000'0000'1100);
    constexpr uint16_t right_pawn_black_b2 = reverse_16(0b0000'0000'0000'0110);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 1] |= FILE(i) > 0 ? SHIFT(left_pawn_black_b2, i, b2) : 0b0;
        array[i + piece_offset * 1] |= FILE(i) < (Board::SIZE - 1) ? SHIFT(right_pawn_black_b2, i, b2) : 0b0;
    }
    // Fill 3rd 16 with FERZ
    constexpr uint16_t top_left_ferz_b2 = reverse_16(0b0000'1000'0000'0000);
    constexpr uint16_t bot_left_ferz_b2 = reverse_16(0b0000'0000'0000'1000);
    constexpr uint16_t top_right_ferz_b2 = reverse_16(0b0000'0010'0000'0000);
    constexpr uint16_t bot_right_ferz_b2 = reverse_16(0b0000'0000'0000'0010);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 2] |= FILE(i) > 0 and RANK(i) > 0 ? SHIFT(top_left_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) > 0 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_left_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) < Board::SIZE - 1 and RANK(i) > 0 ? SHIFT(top_right_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) < Board::SIZE - 1 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_right_ferz_b2, i, b2) : 0b0;
    }
    // Fill 4th 16 with HORS
    constexpr uint16_t top_left_hors_b2 = reverse_16(0b1000'0000'0000'0000);
    constexpr uint16_t top_right_hors_b2 = reverse_16(0b0010'0000'0000'0000);
    constexpr uint16_t right_top_hors_b2 = reverse_16(0b0000'0001'0000'0000);
    constexpr uint16_t right_bot_hors_b2 = reverse_16(0b0000'0000'0000'0001);
    constexpr uint16_t bot_right_hors_c3 = reverse_16(0b0000'0000'0000'0001);
    constexpr uint16_t bot_left_hors_c3 = reverse_16(0b0000'0000'0000'0100);
    constexpr uint16_t left_bot_hors_c3 = reverse_16(0b0000'0000'1000'0000);
    constexpr uint16_t left_top_hors_c3 = reverse_16(0b1000'0000'0000'0000);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 3] |= FILE(i) > 0 and RANK(i) > 1 ? SHIFT(top_left_hors_b2, i, b2) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) < Board::SIZE - 1 and RANK(i) > 1 ? SHIFT(top_right_hors_b2, i, b2) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) < Board::SIZE - 2 and RANK(i) > 0 ? SHIFT(right_top_hors_b2, i, b2) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) < Board::SIZE - 2 and RANK(i) < Board::SIZE - 1 ? SHIFT(right_bot_hors_b2, i, b2) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) < Board::SIZE - 1 and RANK(i) < Board::SIZE - 2 ? SHIFT(bot_right_hors_c3, i, c3) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) > 0 and RANK(i) < Board::SIZE - 2 ? SHIFT(bot_left_hors_c3, i, c3) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) > 1 and RANK(i) < Board::SIZE - 1 ? SHIFT(left_bot_hors_c3, i, c3) : 0b0;
        array[i + piece_offset * 3] |= FILE(i) > 1 and RANK(i) > 0 ? SHIFT(left_top_hors_c3, i, c3) : 0b0;
    }
    // Fill 5th 16 with WAZIR
    constexpr uint16_t top_wazir_b2 = reverse_16(0b0000'0100'0000'0000);
    constexpr uint16_t right_wazir_b2 = reverse_16(0b0000'0000'0010'0000);
    constexpr uint16_t bot_wazir_b2 = reverse_16(0b0000'0000'0000'0100);
    constexpr uint16_t left_wazir_b2 =reverse_16( 0b0000'0000'1000'0000);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 4] |= RANK(i) > 0 ? SHIFT(top_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= FILE(i) < Board::SIZE - 1 ? SHIFT(right_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= RANK(i) < Board::SIZE - 1 ? SHIFT(bot_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= FILE(i) > 0 ? SHIFT(left_wazir_b2, i, b2) : 0b0;
    }
    // Fill 6th 16 with KING
    constexpr uint16_t top_left_king_b2 = reverse_16(0b0000'1100'1000'0000);
    constexpr uint16_t bot_left_king_b2 = reverse_16(0b0000'0000'1000'1100);
    constexpr uint16_t top_right_king_b2 = reverse_16(0b0000'0110'0010'0000);
    constexpr uint16_t bot_right_king_b2 = reverse_16(0b0000'0000'0010'0110);
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 5] |= FILE(i) > 0 and RANK(i) > 0 ? SHIFT(top_left_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) > 0 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_left_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) < Board::SIZE - 1 and RANK(i) > 0 ? SHIFT(top_right_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) < Board::SIZE - 1 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_right_king_b2, i, b2) : 0b0;
    }
    return array;
}();

static uint8_t to_bitmap_index(piece piece, uint8_t piece_index) {
    uint8_t offset = 0;
    switch(piece & PIECE_MASK) {
        case BLACK | PAWN:                  offset = 1; break;
        case WHITE | FERZ:  case BLACK | FERZ:  offset = 2; break;
        case WHITE | HORS:  case BLACK | HORS:  offset = 3; break;
        case WHITE | WAZIR: case BLACK | WAZIR: offset = 4; break;
        case WHITE | KING:  case BLACK | KING:  offset = 5; break;
    }
    return offset * Board::SIZE * Board::SIZE + piece_index;
}

static uint16_t valid_pawn_move_offset(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t file_bitmap = reverse_16(0b1000'1000'1000'1000) << FILE(piece_index);
    const uint16_t move_bitmap = file_bitmap & ~board.get_bitmap() & bitmap;
    const uint8_t other_color = (board.get_piece(piece_index) & COLOR_MASK )^ COLOR_MASK;
    const uint16_t other_color_bitmap = board.get_bitmap(other_color);
    const uint16_t capture_bitmap = ((file_bitmap << 1) | (file_bitmap >> 1)) & other_color_bitmap & bitmap;

    return capture_bitmap | move_bitmap;
}

static uint16_t valid_hors_move_offset(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t piece_bitmap = Board::index_to_bitmap(piece_index);
    const uint16_t all_bitmap = board.get_bitmap();
    const uint16_t left_block = (piece_bitmap >> 1) & all_bitmap;
    if(left_block != 0) { // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap >> 6) | (piece_bitmap << 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t top_block = (piece_bitmap >> Board::SIZE) & all_bitmap;
    if(top_block != 0) { // Can not move to the top
        const uint16_t possible_targets = (piece_bitmap >> 9) | (piece_bitmap >> 7);
        bitmap &= ~possible_targets;

    }
    const uint16_t rigtht_block = (piece_bitmap << 1) & all_bitmap;
    if(rigtht_block != 0) { // Can not move to the right
        const uint16_t possible_targets = (piece_bitmap << 6) | (piece_bitmap >> 2);
        bitmap &= ~possible_targets;
    }
    const uint16_t bottom_block = (piece_bitmap << Board::SIZE) & all_bitmap;
    if(bottom_block != 0) { // Can not move to the left
        const uint16_t possible_targets = (piece_bitmap << 9) | (piece_bitmap << 7);
        bitmap &= ~possible_targets;
    }
    return bitmap;
}

uint16_t valid_move_bitmap(const Board& board, uint8_t piece_index) {
    // Get all possible moves
    const piece piece = board.get_piece(piece_index);
    if (piece == const_piece::null_piece) return 0;
    const uint8_t bitmap_index= to_bitmap_index(piece, piece_index);
    uint16_t valid_moves = possible_move_bitmap[bitmap_index];

    // Filter out those that would be a self capture:
    uint16_t own_pieces_bitmap = board.get_bitmap(piece & COLOR_MASK);
    valid_moves &= ~ own_pieces_bitmap;

    // Filter extra rules:
    if((piece & ID_MASK) == PAWN) return valid_pawn_move_offset(board,piece_index,valid_moves);
    if((piece & ID_MASK) == HORS) return valid_hors_move_offset(board,piece_index,valid_moves);

    return valid_moves;
}

std::vector<move> valid_moves(const Board& board, uint8_t piece_index) {
    std::vector<move> moves;
   board.get_piece(piece_index);

    return moves;
}
