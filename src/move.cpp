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
        push_if_not_own_capture(board, -Board::TILE_COUNT, index, color_mask, attack_offset);
        // Capture sideways
        if (Board::get_file(index) > 0)
            push_only_if_other_capture(board, -Board::TILE_COUNT - 1, index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::TILE_COUNT - 1)
            push_only_if_other_capture(board, -Board::TILE_COUNT + 1, index, color_mask, attack_offset);
    } else {
        // Move forward
        push_if_not_own_capture(board, Board::TILE_COUNT, index, color_mask, attack_offset);
        // Capture sideways
        if (Board::get_file(index) > 0)
            push_only_if_other_capture(board, Board::TILE_COUNT - 1, index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::TILE_COUNT - 1)
            push_only_if_other_capture(board, Board::TILE_COUNT + 1, index, color_mask, attack_offset);
    }

    return attack_offset;
}
static std::vector<int8_t> ferz_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    // Move Left-Up
    if (Board::get_file(index) > 0 and Board::get_rank(index) > 0)
        push_if_not_own_capture(board, -Board::TILE_COUNT - 1, index, color_mask, attack_offset);
    // Move Right-Up
    if (Board::get_file(index) < Board::TILE_COUNT - 1 and Board::get_rank(index) > 0)
        push_if_not_own_capture(board, -Board::TILE_COUNT + 1, index, color_mask, attack_offset);
    // Move Left-Down
    if (Board::get_file(index) > 0 and Board::get_rank(index) < Board::TILE_COUNT - 1)
        push_if_not_own_capture(board, Board::TILE_COUNT - 1, index, color_mask, attack_offset);
    // Move Right-Down
    if (Board::get_file(index) < Board::TILE_COUNT - 1 and Board::get_rank(index) < Board::TILE_COUNT - 1)
        push_if_not_own_capture(board, Board::TILE_COUNT + 1, index, color_mask, attack_offset);

    return attack_offset;
}
static std::vector<int8_t> hors_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    if (Board::get_file(index) > 1 and board.get_piece(index - 1) == const_piece::null_piece) {
        // Can move left (not blocking)
        if (Board::get_rank(index) > 0)
            push_if_not_own_capture(board, -2 - (Board::TILE_COUNT - 1), index, color_mask, attack_offset);
        if (Board::get_rank(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, -2 - (Board::TILE_COUNT + 1), index, color_mask, attack_offset);
    }
    if (Board::get_file(index) < Board::TILE_COUNT - 2 and board.get_piece(index + 1) == const_piece::null_piece) {
        // Can move right (not blocking)
        if (Board::get_rank(index) > 0)
            push_if_not_own_capture(board, +2 - (Board::TILE_COUNT - 1), index, color_mask, attack_offset);
        if (Board::get_rank(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, +2 - (Board::TILE_COUNT + 1), index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) > 1 and board.get_piece(index - Board::TILE_COUNT) == const_piece::null_piece) {
        // Can move up (not blocking)
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 - (2 * Board::TILE_COUNT), index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, +1 - (2 * Board::TILE_COUNT), index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) < Board::TILE_COUNT - 2 and
        board.get_piece(index + Board::TILE_COUNT) == const_piece::null_piece) {
        // Can move down (not blocking)
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 + (2 * Board::TILE_COUNT), index, color_mask, attack_offset);
        if (Board::get_file(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, +1 + (2 * Board::TILE_COUNT), index, color_mask, attack_offset);
    }

    return attack_offset;
}
static std::vector<int8_t> wazir_offsets(const uint8_t color_mask, const Board& board, const uint8_t index) {
    std::vector<int8_t> attack_offset{};
    attack_offset.reserve(8);

    // Move Up
    if (Board::get_rank(index) > 0)
        push_if_not_own_capture(board, -Board::TILE_COUNT, index, color_mask, attack_offset);
    // Move Down
    if (Board::get_rank(index) < Board::TILE_COUNT - 1)
        push_if_not_own_capture(board, Board::TILE_COUNT, index, color_mask, attack_offset);
    // Move Left
    if (Board::get_file(index) > 0) push_if_not_own_capture(board, -1, index, color_mask, attack_offset);
    // Move Right
    if (Board::get_file(index) < Board::TILE_COUNT - 1)
        push_if_not_own_capture(board, +1, index, color_mask, attack_offset);

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
            push_if_not_own_capture(board, -1 - Board::TILE_COUNT, index, color_mask, attack_offset);
    }
    if (Board::get_file(index) < Board::TILE_COUNT - 1) {
        // Move Right
        push_if_not_own_capture(board, +1, index, color_mask, attack_offset);
        // Move Right-Down
        if (Board::get_rank(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, +1 + Board::TILE_COUNT, index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) > 0) {
        // Move Up
        push_if_not_own_capture(board, -Board::TILE_COUNT, index, color_mask, attack_offset);
        // Move Up-Right
        if (Board::get_file(index) < Board::TILE_COUNT - 1)
            push_if_not_own_capture(board, +1 - Board::TILE_COUNT, index, color_mask, attack_offset);
    }
    if (Board::get_rank(index) < Board::TILE_COUNT - 1) {
        // Move Down
        attack_offset.push_back(+Board::TILE_COUNT);
        push_if_not_own_capture(board, +Board::TILE_COUNT, index, color_mask, attack_offset);
        // Move Down-Left
        if (Board::get_file(index) > 0)
            push_if_not_own_capture(board, -1 + Board::TILE_COUNT, index, color_mask, attack_offset);
    }

    return attack_offset;
}

std::vector<int8_t> valid_move_offsets(const Board& board, uint8_t piece_index) {
    const piece p = board.get_piece(piece_index);
    if (p == const_piece::null_piece) return {};
    const uint8_t color_mask = p & COLOR_MASK;
    switch (p & TYPE_MASKE) {
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

inline static move create_move(uint8_t from, uint8_t to, uint16_t flags) { return from | (to << 4) | flags; }


std::vector<move> valid_moves(const Board& board, uint8_t piece_index) {
    const std::vector<int8_t> move_offsets = valid_move_offsets(board, piece_index);
    const piece p = board.get_piece(piece_index);
    std::vector<move> moves{};
    moves.reserve(move_offsets.size());
    for (const auto& offset : move_offsets) {
        const uint8_t target = piece_index + offset;
        uint16_t flags = 0;
        // MOVE FROM HOUSE NOT IMPLEMENTED YET
        if (board.get_piece(piece_index) != const_piece::null_piece) flags |= CAPTURE;
        if ((p & TYPE_MASKE) == PAWN and
            (Board::get_rank(target) == 0 or Board::get_rank(target) == Board::TILE_COUNT - 1))
            flags |= PROMOTION;
        // TODO CHECK check (own and other!!!)
        moves.push_back(create_move(piece_index, target, flags));
    }
    return moves;
}