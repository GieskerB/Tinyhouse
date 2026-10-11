#include "../inc/move.hpp"

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

static uint16_t precomputed_bitmap(piece_t piece, uint8_t piece_index) {
    switch (piece & piece::mask::PIECE) {
        case piece::white_pawn:
            return precomputed_white_pawn_moves[piece_index];
        case piece::black_pawn:
            return precomputed_black_pawn_moves[piece_index];
        case piece::white_ferz:
        case piece::black_ferz:
            return precomputed_ferz_moves[piece_index];
        case piece::white_hors:
        case piece::black_hors:
            return precomputed_hors_moves[piece_index];
        case piece::white_wazir:
        case piece::black_wazir:
            return precomputed_wazir_moves[piece_index];
        case piece::white_king:
        case piece::black_king:
            return precomputed_king_moves[piece_index];
        default:
            return 0;
    }
}

static uint16_t pseudo_legal_pawn_move_bitmap(const Board& board, uint8_t piece_index, uint16_t bitmap) {
    const uint16_t file_bitmap = 0b0001'0001'0001'0001 << Board::get_file(piece_index);
    const uint16_t move_bitmap = file_bitmap & ~board.get_bitmap() & bitmap;
    const uint8_t other_color = (board.get_piece(piece_index) & piece::mask::COLOR) ^ piece::mask::COLOR;
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
    const piece_t piece = board.get_piece(piece_index);
    if ((piece & piece::mask::ID) == piece::id::KING) return bitmap;
    // const uint8_t king_index = board.find_king(piece & COLOR_MASK);
    return bitmap;
}

uint16_t pseudo_legal_move_bitmap(const Board& board, uint8_t piece_index) {
    // Get all possible moves
    const piece_t piece = board.get_piece(piece_index);
    if (piece == piece::null) return 0;
    uint16_t pseudo_legal_moves = precomputed_bitmap(piece, piece_index);

    // Filter out those that would be a self capture:
    uint16_t own_pieces_bitmap = board.get_bitmap(piece & piece::mask::COLOR);
    pseudo_legal_moves &= ~own_pieces_bitmap;

    // Filter extra rules:
    if ((piece & piece::mask::ID) == piece::id::PAWN)
        return pseudo_legal_pawn_move_bitmap(board, piece_index, pseudo_legal_moves);
    if ((piece & piece::mask::ID) == piece::id::HORS)
        return pseudo_legal_hors_move_bitmap(board, piece_index, pseudo_legal_moves);

    return pseudo_legal_moves;
}

static move_t make_move(uint8_t destination, uint8_t source, move_t flags, piece_t captured, piece_t promoted = 0) {
    return (promoted << 18) | (captured << 12) | flags | (destination << 4) | source;
}

std::vector<move_t> legal_moves(const Board& board, uint8_t piece_index) {
    std::vector<move_t> moves;
    const uint16_t move_bitmap = pseudo_legal_move_bitmap(board, piece_index);

    uint16_t current_bitmap = 0b1;

    for (uint8_t i = 0; i < 16; ++i) {
        move_t flags = 0;
        piece_t captured_piece = piece::null;
        if ((current_bitmap & move_bitmap) != 0) {
            const auto current_piece = board.get_piece(i);
            if (current_piece != piece::null) {
                flags |= move::flag::CAPTURE;
                captured_piece = current_piece;
            }
            moves.push_back(make_move(i, piece_index, flags, captured_piece));
        }
        current_bitmap <<= 1;
    }

    return moves;
}

std::vector<move_t> legal_moves(const Board& board, const House& house, uint8_t house_index) {
    std::vector<move_t> moves;

    const piece_t color = house.index_to_piece(house_index) & piece::mask::COLOR;

    const uint16_t move_bitmap = board.get_bitmap(color);

    uint16_t current_bitmap = 0b1;

    for (uint8_t i = 0; i < 16; ++i) {
        move_t flags = move::flag::FROM_HOUSE;
        if ((current_bitmap & move_bitmap) != 0) {
            moves.push_back(make_move(i, house_index, flags, 0));
        }
        current_bitmap <<= 1;
    }

    return moves;
}
