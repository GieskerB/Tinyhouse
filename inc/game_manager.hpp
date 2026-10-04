#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "game.hpp"
class GameManager {
    Game m_game;

   public:
    GameManager();
    GameManager(std::string tiny_fen);

    inline const Game& game() const {return m_game;}

    void init();
    void loop();
    void close();
};

#endif
