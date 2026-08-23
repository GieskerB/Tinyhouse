#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <iostream>

#include "../inc/chessboard.hpp"

int main() {
    init_window();

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
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                const float mouse_x = event.button.x;
                const float mouse_y = event.button.y;
                if (mouse_x < HOUSE_SIZE) {
                    std::cout << "House index: " << static_cast<int>(mouse_y / HEIGHT * 8) << "\n";
                } else {
                    std::cout << "Tile: " << static_cast<int>((mouse_x - HOUSE_SIZE) / WIDTH * TILE_COUNT)
                              << " " << static_cast<int>(mouse_y / HEIGHT * TILE_COUNT) << "\n";
                }
            }
        }
        draw_board();
        SDL_Delay(100);
    }

    close_window();
    return 0;
}
