#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "house.hpp"

class Game {
    Board board;
    House house;

    bool is_whites_turn;
    uint16_t threefold_repetition_counter;
    uint16_t halfmove_counter;
    uint16_t fullmove_counter;

   public:
    Game();
    Game(std::string tiny_fen);

    inline bool is_blacks_turn() { return !is_whites_turn; }
    inline uint16_t get_threefold_repetition_counter() { return +threefold_repetition_counter; }
    inline uint16_t get_halfmove_counter() { return +halfmove_counter; }
    inline uint16_t get_fullmove_counter() { return +fullmove_counter; }
};

#endif
