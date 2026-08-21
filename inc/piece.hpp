#ifndef PIECE_HPP
#define PIECE_HPP

typedef struct {
    const unsigned char id;
    const unsigned char value;
    const bool is_black;
    unsigned char pos_x, pos_y;
} piece;

const piece white_ferz{0, 1, false, 3, 3};
const piece white_hors{1, 2, false, 2, 3};
const piece white_king{2, 255, false, 0, 3};
const piece white_pawn{3, 1, false, 0, 2};
const piece white_wazir{4, 1, false, 1, 3};

const piece black_ferz{5, 1, true, 0, 0};
const piece black_hors{6, 2, true, 1, 0};
const piece black_king{7, 255, true, 3, 0};
const piece black_pawn{8, 1, true, 3, 1};
const piece black_wazir{9, 1, true, 2, 0};

#endif