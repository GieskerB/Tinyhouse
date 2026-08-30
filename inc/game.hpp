#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "house.hpp"

class Game {
    Board m_board;
    House m_house;

    bool m_is_whites_turn;
    uint16_t m_halfmove_counter;
    uint16_t m_fullmove_counter;

   public:
    Game();
    Game(std::string tiny_fen);

    inline const Board& board() const { return m_board; }
    inline const House& house() const { return m_house; }

    inline bool is_whites_turn() const { return m_is_whites_turn; }
    inline uint16_t halfmove_counter() const { return +m_halfmove_counter; }
    inline uint16_t fullmove_counter() const { return +m_fullmove_counter; }
};

#endif
