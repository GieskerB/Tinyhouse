#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <iostream>

#include "../inc/game_manager.hpp"

int main() {
    GameManager manager;

    manager.init();
    manager.loop();
    manager.close();

    return 0;
}
