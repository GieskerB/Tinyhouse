#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "board.hpp"
#include "house.hpp"

class GameManager {
    Board board;
    House house;

   public:
    void init();

    void loop();

    void close();
};

#endif