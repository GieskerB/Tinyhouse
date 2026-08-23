#ifndef PIECE_HPP
#define PIECE_HPP

typedef uint8_t piece;

static constexpr uint8_t PAWN = 0b000, PAWN_VALUE = 1;
static constexpr uint8_t FERZ = 0b001, FERZ_VALUE = 1;
static constexpr uint8_t HORS = 0b010, HORS_VALUE = 2;
static constexpr uint8_t WAZIR = 0b011, WAZIR_VALUE = 1;
static constexpr uint8_t KING = 0b100, KING_VALUE = 255;

static constexpr uint8_t WHITE = 0b1000, BLACK = 0b10000;
static constexpr uint8_t ON_BOARD = 0b100000, IN_HOUSE = 0b1000000;

static constexpr uint8_t ID_MASK = 0b111;
static constexpr uint8_t PIECE_MASK = 0b1111;

static inline uint8_t PIECE_INDEX(piece p) {
    return (p & ID_MASK) | ((p & BLACK) >> 1);
}

piece null_piece = 0b0; 

piece white_ferz = FERZ | WHITE | ON_BOARD;    
piece white_hors = HORS | WHITE | ON_BOARD;    
piece white_wazir = WAZIR | WHITE | ON_BOARD;  
piece white_pawn = PAWN | WHITE | ON_BOARD;    
piece white_king = KING | WHITE | ON_BOARD;    

piece black_ferz = FERZ | BLACK | ON_BOARD;    
piece black_hors = HORS | BLACK | ON_BOARD;    
piece black_wazir = WAZIR | BLACK | ON_BOARD;  
piece black_pawn = PAWN | BLACK | ON_BOARD;    
piece black_king = KING | BLACK | ON_BOARD;    

#endif