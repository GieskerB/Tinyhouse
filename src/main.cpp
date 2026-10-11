#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <iostream>

#include "../inc/game.hpp"
#include "../inc/game_manager.hpp"

// File = column
// Rank = row

int main() {
    GameManager gm;

    gm.init();
    gm.loop();
    gm.close();

    return 0;
}
