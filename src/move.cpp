#include "../inc/move.hpp"

#include "../inc/board.hpp"

static inline void push_if_not_own_capture(const Board& board, const int8_t offset, const uint8_t index,
                                           const uint8_t color_mask, std::vector<int8_t>& vec) {
    if ((board.get_piece(index + offset) & COLOR_MASK) != color_mask) {
        vec.push_back(offset);
    }
}
// Important for pawn only allowing them to move diagonal if capture!
static inline void push_only_if_other_capture(const Board& board, const int8_t offset, const uint8_t index,
                                              const uint8_t color_mask, std::vector<int8_t>& vec) {
    if ((board.get_piece(index + offset) & COLOR_MASK) == (color_mask ^ (WHITE | BLACK))) {
        vec.push_back(offset);
    }
}

static std::vector<int8_t> pawn_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);
    if (color_mask == WHITE) {
        // Move forward
        push_if_not_own_capture(board, -Board::SIZE, index, color_mask, attack_offset);
        // Capture sideways
        if (Board::get_file(index) > 0)
            push_only_if_other_capture(board, -Board::SIZE - 1, index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::SIZE - 1)
            push_only_if_other_capture(board, -Board::SIZE + 1, index, color_mask, attack_offset);
    } else {
        // Move forward
        push_if_not_own_capture(board, Board::SIZE, index, color_mask, attack_offset);
        // Capture sideways
        if (Board::get_file(index) > 0)
            push_only_if_other_capture(board, Board::SIZE - 1, index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::SIZE - 1)
            push_only_if_other_capture(board, Board::SIZE + 1, index, color_mask, attack_offset);
    }

    return attack_offset;
}
static std::vector<int8_t> ferz_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    // Move Left-Up
    if (Board::get_file(index) > 0 and Board::get_rank(index) > 0)
        push_if_not_own_capture(board, -Board::SIZE - 1, index, color_mask, attack_offset);
    // Move Right-Up
    if (Board::get_file(index) < Board::SIZE - 1 and Board::get_rank(index) > 0)
        push_if_not_own_capture(board, -Board::SIZE + 1, index, color_mask, attack_offset);
    // Move Left-Down
    if (Board::get_file(index) > 0 and Board::get_rank(index) < Board::SIZE - 1)
        push_if_not_own_capture(board, Board::SIZE - 1, index, color_mask, attack_offset);
    // Move Right-Down
    if (Board::get_file(index) < Board::SIZE - 1 and Board::get_rank(index) < Board::SIZE - 1)
        push_if_not_own_capture(board, Board::SIZE + 1, index, color_mask, attack_offset);

    return attack_offset;
}
static std::vector<int8_t> hors_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    if (Board::get_file(index) > 1 and board.get_piece(index - 1) == const_piece::null_piece) {
        // Can move left (not blocking)
        if (Board::get_rank(index) > 0)
            push_if_not_own_capture(board, -2 - Board::SIZE, index, color_mask, attack_offset);
        if (Board::get_rank(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, -2 + Board::SIZE, index, color_mask, attack_offset);
    }
    if (Board::get_file(index) < Board::SIZE - 2 and board.get_piece(index + 1) == const_piece::null_piece) {
        // Can move right (not blocking)
        if (Board::get_rank(index) > 0)
            push_if_not_own_capture(board, +2 - Board::SIZE, index, color_mask, attack_offset);
        if (Board::get_rank(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, +2 + Board::SIZE, index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) > 1 and board.get_piece(index - Board::SIZE) == const_piece::null_piece) {
        // Can move up (not blocking)
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 - (2 * Board::SIZE), index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, +1 - (2 * Board::SIZE), index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) < Board::SIZE - 2 and board.get_piece(index + Board::SIZE) == const_piece::null_piece) {
        // Can move down (not blocking)
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 + (2 * Board::SIZE), index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, +1 + (2 * Board::SIZE), index, color_mask, attack_offset);
    }

    return attack_offset;
}
static std::vector<int8_t> wazir_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    // Move Up
    if (Board::get_rank(index) > 0) push_if_not_own_capture(board, -Board::SIZE, index, color_mask, attack_offset);
    // Move Down
    if (Board::get_rank(index) < Board::SIZE - 1)
        push_if_not_own_capture(board, Board::SIZE, index, color_mask, attack_offset);
    // Move Left
    if (Board::get_file(index) > 0) push_if_not_own_capture(board, -1, index, color_mask, attack_offset);
    // Move Right
    if (Board::get_file(index) < Board::SIZE - 1) push_if_not_own_capture(board, +1, index, color_mask, attack_offset);

    return attack_offset;
}
static std::vector<int8_t> king_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    if (Board::get_file(index) > 0) {
        // Move Left
        push_if_not_own_capture(board, -1, index, color_mask, attack_offset);
        // Move Left-Up
        if (Board::get_rank(index) > 0)
            push_if_not_own_capture(board, -1 - Board::SIZE, index, color_mask, attack_offset);
    }
    if (Board::get_file(index) < Board::SIZE - 1) {
        // Move Right
        push_if_not_own_capture(board, +1, index, color_mask, attack_offset);
        // Move Right-Down
        if (Board::get_rank(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, +1 + Board::SIZE, index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) > 0) {
        // Move Up
        push_if_not_own_capture(board, -Board::SIZE, index, color_mask, attack_offset);
        // Move Up-Right
        if (Board::get_file(index) < Board::SIZE - 1)
            push_if_not_own_capture(board, +1 - Board::SIZE, index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) < Board::SIZE - 1) {
        // Move Down
        push_if_not_own_capture(board, +Board::SIZE, index, color_mask, attack_offset);
        // Move Down-Left
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 + Board::SIZE, index, color_mask, attack_offset);
    }

    return attack_offset;
}

std::vector<int8_t> valid_move_offsets(const Board& board, const char str_index[3]) {
    assert((str_index[0] >= 'a' and str_index[0] <= 'd') or (str_index[0] >= 'A' and str_index[0] <= 'D'));
    assert((str_index[1] >= '0' and str_index[1] <= '4'));
    assert(str_index[2] == '\0');
    uint8_t piece_index = (str_index[0] - 'a') + (Board::SIZE - (str_index[1] - '0')) * Board::SIZE;
    return valid_move_offsets(board, piece_index);
}

std::vector<int8_t> valid_move_offsets(const Board& board, uint8_t piece_index) {
    const piece p = board.get_piece(piece_index);
    if (p == const_piece::null_piece) return {};
    const uint8_t color_mask = p & COLOR_MASK;
    switch (p & ID_MASK) {
        case PAWN:
            return pawn_offsets(color_mask, board, piece_index);
        case FERZ:
            return ferz_offsets(color_mask, board, piece_index);
        case HORS:
            return hors_offsets(color_mask, board, piece_index);
        case WAZIR:
            return wazir_offsets(color_mask, board, piece_index);
        case KING:
            return king_offsets(color_mask, board, piece_index);
        default:
            return {};
    }
}

uint8_t find_king(const Board& board, uint8_t color) {
    const piece king = KING | color;
    for (uint8_t i = 0; i < Board::SIZE * Board::SIZE; ++i) {
        if (board.get_piece(i) == king) return i;
    }
    return 255;  // error
}

#define INDEX_SHIFT(index, base) (index - base)
#define SHIFT(bitmap, index, base) \
    (INDEX_SHIFT(index, base) < 0) ? bitmap << -INDEX_SHIFT(index, base) : bitmap >> INDEX_SHIFT(index, base)
#define FILE(i) Board::get_file(i)
#define RANK(i) Board::get_rank(i)

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
constexpr auto move_bitmasks = [] {
    constexpr uint8_t piece_offset = Board::SIZE * Board::SIZE;
    std::array<uint16_t, piece_offset * 6> array{0};
    // assume default bitmap is centered on b2 (or c3)
    constexpr int8_t b2 = 9, c3 = 6;
    // Fill 1st 16 with WHITE PAWN
    constexpr uint16_t left_pawn_white_b2 = 0b0000'1100'0000'0000;
    constexpr uint16_t right_pawn_white_b2 = 0b0000'0110'0000'0000;
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 0] |= FILE(i) > 0 ? SHIFT(left_pawn_white_b2, i, b2) : 0b0;
        array[i + piece_offset * 0] |= FILE(i) < (Board::SIZE - 1) ? SHIFT(right_pawn_white_b2, i, b2) : 0b0;
    }
    // Fill 2nd 16 with BLACK PAWN
    constexpr uint16_t left_pawn_black_b2 = 0b0000'0000'0000'1100;
    constexpr uint16_t right_pawn_black_b2 = 0b0000'0000'0000'0110;
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 1] |= FILE(i) > 0 ? SHIFT(left_pawn_black_b2, i, b2) : 0b0;
        array[i + piece_offset * 1] |= FILE(i) < (Board::SIZE - 1) ? SHIFT(right_pawn_black_b2, i, b2) : 0b0;
    }
    // Fill 3rd 16 with FERZ
    constexpr uint16_t top_left_ferz_b2 = 0b0000'1000'0000'0000;
    constexpr uint16_t bot_left_ferz_b2 = 0b0000'0000'0000'1000;
    constexpr uint16_t top_right_ferz_b2 = 0b0000'0010'0000'0000;
    constexpr uint16_t bot_right_ferz_b2 = 0b0000'0000'0000'0010;
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 2] |= FILE(i) > 0 and RANK(i) > 0 ? SHIFT(top_left_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) > 0 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_left_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) < Board::SIZE - 1 and RANK(i) > 0 ? SHIFT(top_right_ferz_b2, i, b2) : 0b0;
        array[i + piece_offset * 2] |= FILE(i) < Board::SIZE - 1 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_right_ferz_b2, i, b2) : 0b0;
    }
    // Fill 4th 16 with HORS
    constexpr uint16_t top_left_hors_b2 = 0b1000'0000'0000'0000;
    constexpr uint16_t top_right_hors_b2 = 0b0010'0000'0000'0000;
    constexpr uint16_t right_top_hors_b2 = 0b0000'0001'0000'0000;
    constexpr uint16_t right_bot_hors_b2 = 0b0000'0000'0000'0001;
    constexpr uint16_t bot_right_hors_c3 = 0b0000'0000'0000'0001;
    constexpr uint16_t bot_left_hors_c3 = 0b0000'0000'0000'0100;
    constexpr uint16_t left_bot_hors_c3 = 0b0000'0000'1000'0000;
    constexpr uint16_t left_top_hors_c3 = 0b1000'0000'0000'0000;
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
    constexpr uint16_t top_wazir_b2 = 0b0000'0100'0000'0000;
    constexpr uint16_t right_wazir_b2 = 0b0000'0000'0010'0000;
    constexpr uint16_t bot_wazir_b2 = 0b0000'0000'0000'0100;
    constexpr uint16_t left_wazir_b2 = 0b0000'0000'1000'0000;
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 4] |= RANK(i) > 0 ? SHIFT(top_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= FILE(i) < Board::SIZE - 1 ? SHIFT(right_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= RANK(i) < Board::SIZE - 1 ? SHIFT(bot_wazir_b2, i, b2) : 0b0;
        array[i + piece_offset * 4] |= FILE(i) > 0 ? SHIFT(left_wazir_b2, i, b2) : 0b0;
    }
    // Fill 6th 16 with KING
    constexpr uint16_t top_left_king_b2 = 0b0000'1100'1000'0000;
    constexpr uint16_t bot_left_king_b2 = 0b0000'0000'1000'1100;
    constexpr uint16_t top_right_king_b2 = 0b0000'0110'0010'0000;
    constexpr uint16_t bot_right_king_b2 = 0b0000'0000'0010'0110;
    for (uint8_t i = 0; i < piece_offset; ++i) {
        array[i + piece_offset * 5] |= FILE(i) > 0 and RANK(i) > 0 ? SHIFT(top_left_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) > 0 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_left_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) < Board::SIZE - 1 and RANK(i) > 0 ? SHIFT(top_right_king_b2, i, b2) : 0b0;
        array[i + piece_offset * 5] |= FILE(i) < Board::SIZE - 1 and RANK(i) < Board::SIZE - 1 ? SHIFT(bot_right_king_b2, i, b2) : 0b0;
    }
    return array;
}();

#include <iostream>

void print_array() {
    for (size_t i = 0; i < move_bitmasks.size(); ++i) {
        if (i == 16 * 3) std::cout << "HORS: ";
        std::cout << "i: " << i << ":\n";
        auto map = move_bitmasks[i];
        for (uint16_t mask = 0b1000000000000000; mask != 0; mask >>= 1) {
            if ((mask & map) != 0) {
                std::cout << '#';
            } else if (mask == (0b1 << (((16 - i - 1) % 16)))) {
                std::cout << '+';
            } else {
                std::cout << '.';
            }
            if ((mask == 0b10000) or (mask == 0b100000000) or (mask == 0b1000000000000)) std::cout << '\n';
        }
        std::cout << "\n";
    }
}

bool in_check(const Board& board, uint8_t king_index) {
    // TODO:
    // - For efficient first order checking. Check only the tiles that can be reached by the kings positions as if king
    // that that piece (of example only check of hors checks a hors distance away)

    // use attacking bit masks for quick and easy checking

    const uint8_t king_color = board.get_piece(king_index) & COLOR_MASK;
    const uint8_t other_color = king_color ^ COLOR_MASK;
    // Cant check with King
    // Check for Wazir Check
    const piece other_wazir = WAZIR | other_color;
    if (Board::get_rank(king_index) > 0 and board.get_piece(king_index - Board::SIZE) == other_wazir) return true;
    if (Board::get_rank(king_index) < Board::SIZE - 1 and board.get_piece(king_index + Board::SIZE) == other_wazir)
        return true;
    if (Board::get_file(king_index) > 0 and board.get_piece(king_index - 1) == other_wazir) return true;
    if (Board::get_file(king_index) < Board::SIZE - 1 and board.get_piece(king_index + 1) == other_wazir) return true;
    // Check for Hors Check
    const piece other_horse = HORS | other_color;
    if (Board::get_rank(king_index) > 1) {
        if (Board::get_file(king_index) > 0 and
            board.get_piece(king_index - 1 - Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index - 1 - 2 * Board::SIZE) == other_horse)
            return true;
        if (Board::get_file(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index + 1 - Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index + 1 - 2 * Board::SIZE) == other_horse)
            return true;
    }
    if (Board::get_rank(king_index) < Board::SIZE - 2) {
        if (Board::get_file(king_index) > 0 and
            board.get_piece(king_index - 1 + Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index - 1 + 2 * Board::SIZE) == other_horse)
            return true;
        if (Board::get_file(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index + 1 + Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index + 1 + 2 * Board::SIZE) == other_horse)
            return true;
    }
    if (Board::get_file(king_index) > 1) {
        if (Board::get_rank(king_index) > 0 and
            board.get_piece(king_index - 1 - Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index - 2 - Board::SIZE) == other_horse)
            return true;
        if (Board::get_rank(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index - 1 + Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index - 2 + Board::SIZE) == other_horse)
            return true;
    }
    if (Board::get_file(king_index) < Board::SIZE - 2) {
        if (Board::get_rank(king_index) > 0 and
            board.get_piece(king_index + 1 - Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index + 2 - Board::SIZE) == other_horse)
            return true;
        if (Board::get_rank(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index + 1 + Board::SIZE) == const_piece::null_piece and
            board.get_piece(king_index + 2 + Board::SIZE) == other_horse)
            return true;
    }
    // Check for Ferz Check
    const piece other_ferz = FERZ | other_color;
    if (Board::get_file(king_index) > 0 and Board::get_rank(king_index) > 0 and
        board.get_piece(king_index - 1 - Board::SIZE) == other_ferz)
        return true;
    if (Board::get_file(king_index) > 0 and Board::get_rank(king_index) < Board::SIZE - 1 and
        board.get_piece(king_index + 1 - Board::SIZE) == other_ferz)
        return true;
    if (Board::get_file(king_index) < Board::SIZE - 1 and Board::get_rank(king_index) > 0 and
        board.get_piece(king_index - 1 + Board::SIZE) == other_ferz)
        return true;
    if (Board::get_file(king_index) < Board::SIZE - 1 and Board::get_rank(king_index) < Board::SIZE - 1 and
        board.get_piece(king_index + 1 + Board::SIZE) == other_ferz)
        return true;
    // Check for Pawn Check
    const piece other_pawn = PAWN | other_color;
    if (other_color == WHITE and Board::get_rank(king_index) < Board::SIZE - 1) {
        if (Board::get_file(king_index) > 0 and board.get_piece(king_index - 1 + Board::SIZE) == other_pawn)
            return true;
        if (Board::get_file(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index + 1 + Board::SIZE) == other_pawn)
            return true;
    } else if (other_color == BLACK and Board::get_rank(king_index) > 0) {
        if (Board::get_file(king_index) > 0 and board.get_piece(king_index - 1 - Board::SIZE) == other_pawn)
            return true;
        if (Board::get_file(king_index) < Board::SIZE - 1 and
            board.get_piece(king_index + 1 - Board::SIZE) == other_pawn)
            return true;
    }
    return false;
}

inline static move create_move(uint8_t from, uint8_t to, uint16_t flags) { return from | (to << 4) | flags; }

std::vector<move> valid_moves(const Board& board, uint8_t piece_index) {
    print_array();
    const std::vector<int8_t> move_offsets = valid_move_offsets(board, piece_index);
    const piece p = board.get_piece(piece_index);
    std::vector<move> moves{};
    moves.reserve(move_offsets.size() + 2);  // space for all 3 promotions
    for (const auto& offset : move_offsets) {
        const uint8_t target = piece_index + offset;
        uint16_t flags = 0;
        // MOVE FROM HOUSE NOT IMPLEMENTED YET
        if (board.get_piece(piece_index) != const_piece::null_piece) {
            const piece captured_piece = board.get_piece(piece_index);
            flags |= CAPTURE;
            flags |= captured_piece << 12;
        }
        if ((p & ID_MASK) == PAWN and (Board::get_rank(target) == 0 or Board::get_rank(target) == Board::SIZE - 1)) {
            flags |= PROMOTION;
            // create 3 moves with all 3 promotions
        }
        // TODO CHECK check (own and other!!!)
        moves.push_back(create_move(piece_index, target, flags));
    }
    return moves;
}