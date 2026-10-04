#include "../inc/game_manager.hpp"

#include <SDL3/SDL.h>

#include <iostream>

#include "../inc/renderer.hpp"

GameManager::GameManager(): m_game() {};
GameManager::GameManager(std::string tiny_fen): m_game(tiny_fen) {};

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
                    // const int8_t house_index = static_cast<int>(mouse_y / HEIGHT * House::HOUSE_SIZE);

                } else {
                    const int8_t rank = static_cast<int8_t>(mouse_y / HEIGHT * Board::SIZE);
                    const int8_t file = static_cast<int8_t>((mouse_x - HOUSE_SIZE) / BOARD_SIZE * Board::SIZE);
                    clear_highlighted();
                    for (const auto& move : legal_moves(m_game.board(), Board::get_index(rank, file))) {
                        const uint8_t source = move & 0b1111;
                        set_highlighted(source);
                        const uint8_t destination = (move & 0b11110000) >> 4;
                        set_highlighted(destination);
                    }
                }
            }
        }
        render_board(m_game.board());
        render_house(m_game.house());
        SDL_Delay(100);
    }
}

void GameManager::close() { close_renderer(); }
