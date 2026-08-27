#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "house.hpp"

class Game {
    Board board;
    House house;

    bool is_whites_turn;
    uint8_t threefold_repetition_counter;
    uint8_t halfmove_counter;
    uint16_t fullmove_counter;

    public:
    Game();
    Game(std::string tiny_fen);

}

#endif

