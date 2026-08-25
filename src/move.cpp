#include "../inc/move.hpp"

#include "../inc/board.hpp"

inline static move create_move(uint8_t from, uint8_t to, uint16_t flags) { return from | (to << 4) | flags; }

#include <iostream>

static void clear_own_captures(const Board& board, uint8_t piece_index, std::vector<int8_t>& offsets) {
    const piece p = board.get_piece(piece_index);
    const uint8_t  team_color = p & COLOR_MASK;
    for (int8_t i = offsets.size() -1; i >= 0; --i) {
        if ((board.get_piece(piece_index + offsets[i]) & COLOR_MASK) == team_color ) {
            offsets.erase(std::next(offsets.begin(), i));
        }
    }
}

static std::vector<int8_t> pawn_attack(bool is_black, uint8_t index) {
    std::vector<int8_t> attack_offset{};

    if (!is_black) {
        if (Board::get_file(index) > 0) attack_offset.push_back(-Board::TILE_COUNT - 1);
        if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(-Board::TILE_COUNT + 1);
    } else {
        if (Board::get_file(index) > 0) attack_offset.push_back(Board::TILE_COUNT - 1);
        if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(Board::TILE_COUNT + 1);
    }

    return attack_offset;
}
static std::vector<int8_t> ferz_attack(uint8_t index) {
    std::vector<int8_t> attack_offset{};

    // Move Left-Up
    if (Board::get_file(index) > 0 and Board::get_rank(index) > 0) attack_offset.push_back(-Board::TILE_COUNT - 1);
    // Move Right-Up
    if (Board::get_file(index) < Board::TILE_COUNT - 1 and Board::get_rank(index) > 0)
        attack_offset.push_back(-Board::TILE_COUNT + 1);
    // Move Left-Down
    if (Board::get_file(index) > 0 and Board::get_rank(index) < Board::TILE_COUNT - 1)
        attack_offset.push_back(Board::TILE_COUNT - 1);
    // Move Right-Down
    if (Board::get_file(index) < Board::TILE_COUNT - 1 and Board::get_rank(index) < Board::TILE_COUNT - 1)
        attack_offset.push_back(Board::TILE_COUNT + 1);

    return attack_offset;
}
static std::vector<int8_t> hors_attack(const Board& board, uint8_t index) {
    std::vector<int8_t> attack_offset{};

    if (Board::get_file(index) > 1 and board.get_piece(index - 1) == const_piece::null_piece) {
        // Can move left (not blocking)
        if (Board::get_rank(index) > 0) attack_offset.push_back(-2 - (Board::TILE_COUNT - 1));
        if (Board::get_rank(index) < Board::TILE_COUNT - 1) attack_offset.push_back(-2 - (Board::TILE_COUNT + 1));
    }
    if (Board::get_file(index) < Board::TILE_COUNT - 2 and board.get_piece(index + 1) == const_piece::null_piece) {
        // Can move right (not blocking)
        if (Board::get_rank(index) > 0) attack_offset.push_back(+2 - (Board::TILE_COUNT - 1));
        if (Board::get_rank(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+2 - (Board::TILE_COUNT + 1));
    }
    if (Board::get_rank(index) > 1 and board.get_piece(index - Board::TILE_COUNT) == const_piece::null_piece) {
        // Can move up (not blocking)
        if (Board::get_file(index) > 0) attack_offset.push_back(-1 - (2 * Board::TILE_COUNT));
        if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+1 - (2 * Board::TILE_COUNT));
    }
    if (Board::get_rank(index) < Board::TILE_COUNT - 2 and
        board.get_piece(index + Board::TILE_COUNT) == const_piece::null_piece) {
        // Can move down (not blocking)
        if (Board::get_file(index) > 0) attack_offset.push_back(-1 + (2 * Board::TILE_COUNT));
        if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+1 + (2 * Board::TILE_COUNT));
    }

    return attack_offset;
}
static std::vector<int8_t> wazir_attack(uint8_t index) {
    std::vector<int8_t> attack_offset{};

    // Move Up
    if (Board::get_rank(index) > 0) attack_offset.push_back(-Board::TILE_COUNT);
    // Move Down
    if (Board::get_rank(index) < Board::TILE_COUNT - 1) attack_offset.push_back(Board::TILE_COUNT);
    // Move Left
    if (Board::get_file(index) > 0) attack_offset.push_back(-1);
    // Move Right
    if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+1);

    return attack_offset;
}
static std::vector<int8_t> king_attack(uint8_t index) {
    std::vector<int8_t> attack_offset{};

    if (Board::get_file(index) > 0) {
        // Move Left
        attack_offset.push_back(-1);
        // Move Left-Up
        if (Board::get_rank(index) > 0) attack_offset.push_back(-1 - Board::TILE_COUNT);
    }
    if (Board::get_file(index) < Board::TILE_COUNT - 1) {
        // Move Right
        attack_offset.push_back(+1);
        // Move Right-Down
        if (Board::get_rank(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+1 + Board::TILE_COUNT);
    }
    if (Board::get_rank(index) > 0) {
        // Move Up
        attack_offset.push_back(-Board::TILE_COUNT);
        // Move Up-Right
        if (Board::get_file(index) < Board::TILE_COUNT - 1) attack_offset.push_back(+1 - Board::TILE_COUNT);
    }
    if (Board::get_rank(index) < Board::TILE_COUNT - 1) {
        // Move Down
        attack_offset.push_back(+Board::TILE_COUNT);
        // Move Down-Left
        if (Board::get_file(index) > 0) attack_offset.push_back(-1 + Board::TILE_COUNT);
    }

    return attack_offset;
}

std::vector<int8_t> ATTACKS(const Board& board, uint8_t piece_index) {
    const piece p = board.get_piece(piece_index);
    if (p == const_piece::null_piece) return {};
    const bool is_black = p & BLACK;
    std::vector<int8_t> attacks;
    switch (p & TYPE_MASKE) {
        case PAWN:
            attacks= pawn_attack(is_black, piece_index);
            break;
        case FERZ:
            attacks= ferz_attack(piece_index);
            break;
        case HORS:
            attacks= hors_attack(board, piece_index);
            break;
        case WAZIR:
            attacks= wazir_attack(piece_index);
            break;
        case KING:
            attacks= king_attack(piece_index);
            break;
        default:
            return {};
    }
    clear_own_captures(board,piece_index,attacks);
    return attacks;
}

static std::vector<int8_t> pawn_move(bool is_black) {
    std::vector<int8_t> attack_offset{};

    if (!is_black)
        attack_offset.push_back(-Board::TILE_COUNT);
    else
        attack_offset.push_back(Board::TILE_COUNT);

    return attack_offset;
}
static std::vector<int8_t> ferz_move(uint8_t index) { return ferz_attack(index); }
static std::vector<int8_t> hors_move(const Board& board, uint8_t index) { return hors_attack(board, index); }
static std::vector<int8_t> wazir_move(uint8_t index) { return wazir_attack(index); }
static std::vector<int8_t> king_move(uint8_t index) { return king_attack(index); }

std::vector<int8_t> MOVES(const Board& board, uint8_t piece_index) {
    const piece p = board.get_piece(piece_index);
    if (p == const_piece::null_piece) return {};
    const bool is_black = p & BLACK;
    switch (p & TYPE_MASKE) {
        case PAWN:
            return pawn_move(is_black);
        case FERZ:
            return ferz_move(piece_index);
        case HORS:
            return hors_move(board, piece_index);
        case WAZIR:
            return wazir_move(piece_index);
        case KING:
            return king_move(piece_index);
        default:
            return {};
    }
}
