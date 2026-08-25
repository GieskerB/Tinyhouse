#include "../inc/game_manager.hpp"

#include <SDL3/SDL.h>

#include <iostream>

#include "../inc/renderer.hpp"

void GameManager::init() { init_renderer(); }

void GameManager::loop() {
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
                    const int8_t house_index = static_cast<int>(mouse_y / HEIGHT * House::HOUSE_SIZE);
                    house.select(house_index);
                    board.unselect_piece();
                } else {
                    const int8_t file = static_cast<int8_t>((mouse_x - HOUSE_SIZE) / BOARD_SIZE * Board::TILE_COUNT);
                    const int8_t rank = static_cast<int8_t>(mouse_y / HEIGHT * Board::TILE_COUNT);
                    board.select_piece(Board::get_index(file, rank));
                    house.unselect();
                }
            }
        }
        render_board(board);
        render_house(house);
        SDL_Delay(100);
    }
}

void GameManager::close() { close_renderer(); }