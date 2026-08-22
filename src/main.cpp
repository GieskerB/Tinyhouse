#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include "../inc/chessboard.hpp"

int main() {
    init_window();
    draw_board();

    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN and event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }
        SDL_Delay(100);
    }

    close_window();
    return 0;
}
