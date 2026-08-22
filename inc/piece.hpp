#ifndef PIECE_HPP
#define PIECE_HPP

static constexpr unsigned char PAWN_ID = 0, PAWN_VALUE = 1;
static constexpr unsigned char FERZ_ID = 1, FERZ_VALUE = 1;
static constexpr unsigned char HORS_ID = 2, HORS_VALUE = 2;
static constexpr unsigned char WAZIR_ID = 3, WAZIR_VALUE = 1;
static constexpr unsigned char KING_ID = 4, KING_VALUE = 255;

typedef struct {
    const unsigned char id, value;
    bool is_black, on_board;
    unsigned char pos_x, pos_y;

} piece;

const piece white_ferz{FERZ_ID, FERZ_VALUE, false, true, 3, 3};
const piece white_hors{HORS_ID, HORS_VALUE, false, true, 2, 3};
const piece white_wazir{WAZIR_ID, WAZIR_VALUE, false, true, 1, 3};
const piece white_pawn{PAWN_ID, PAWN_VALUE, false, true, 0, 2};
const piece white_king{KING_ID, KING_VALUE, false, true, 0, 3};

const piece black_ferz{FERZ_ID, FERZ_VALUE, true, true, 0, 0};
const piece black_hors{HORS_ID, HORS_VALUE, true, true, 1, 0};
const piece black_wazir{WAZIR_ID, WAZIR_VALUE, true, true, 2, 0};
const piece black_pawn{PAWN_ID, PAWN_VALUE, true, true, 3, 1};
const piece black_king{KING_ID, KING_VALUE, true, true, 3, 0};

#endif