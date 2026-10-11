#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "game.hpp"

class GameManager {
    Game m_game;

    uint8_t m_selected_index;

    void handle_board_click(uint8_t rank, uint8_t file);
    void handle_house_click(uint8_t house_index);

   public:
    GameManager();
    GameManager(std::string tiny_fen);

    inline const Game& game() const { return m_game; }

    void init();
    void loop();
    void close();
};

#endif
