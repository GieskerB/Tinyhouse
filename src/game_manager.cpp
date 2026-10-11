#include <SDL3/SDL.h>
#include <iostream>

#include "../inc/game_manager.hpp"
#include "../inc/renderer.hpp"
#include "../inc/move.hpp"

GameManager::GameManager() : m_game(), m_selected_index(-1) {};
GameManager::GameManager(std::string tiny_fen) : m_game(tiny_fen), m_selected_index(-1) {};

void GameManager::init() { init_renderer(); }

void GameManager::handle_board_click(uint8_t rank, uint8_t file) {
    const uint8_t current_index = Board::get_index(rank, file);
    const uint8_t current_piece_color = m_game.board().get_piece(current_index) & piece::mask::COLOR;
    const bool prev_selected = m_selected_index != static_cast<uint8_t>(-1);

    if (!prev_selected) {
        // currently nothing selected:
        // cant select piece of opposing color first
        if (m_game.is_whites_turn() and current_piece_color == piece::color::BLACK) return;
        if (!m_game.is_whites_turn() and current_piece_color == piece::color::WHITE) return;
    }

    // Selected a target tile for the moves of the selected piece: Make that move
    auto possible_moves = prev_selected ? legal_moves(m_game.board(), m_selected_index) : std::vector<move_t>{};
    for (const auto& move : possible_moves) {
        if ((move & 0b11110000) >> 4 == current_index) {
            m_game.make_move(move);
            clear_highlighted();
            m_selected_index = -1;
            return;
        }
    }
    // if not selecting another of you own pieces, unselect the last one
    clear_highlighted();
    m_selected_index = -1;
    if (m_game.is_whites_turn() and current_piece_color != piece::color::WHITE) return;
    if (!m_game.is_whites_turn() and current_piece_color != piece::color::BLACK) return;

    // at this point the player selected another of their own pieces: update selection
    possible_moves = legal_moves(m_game.board(), current_index);
    for (const auto& move : possible_moves) {
        highlight_move(move);
    }
    m_selected_index = current_index;
}

void GameManager::handle_house_click(uint8_t house_index) {
    const piece_t house_piece = m_game.house().index_to_piece(house_index);
    const uint8_t count = m_game.house().count(house_piece);
    if (count == 0) return;
    m_selected_index = -1;
    clear_highlighted();
    const auto possible_moves = legal_moves(m_game.board(), m_game.house(), house_index);
    for (const auto& move : possible_moves) {
        highlight_move(move);
    }


}

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
                if (mouse_x < House::SIZE) {
                    const int8_t house_index = static_cast<int>(mouse_y / HEIGHT * House::SIZE);
                    handle_house_click (house_index);

                } else {
                    const int8_t rank = static_cast<int8_t>(mouse_y / HEIGHT * Board::SIZE);
                    const int8_t file = static_cast<int8_t>((mouse_x - HOUSE_SIZE) / BOARD_SIZE * Board::SIZE);
                    handle_board_click(rank, file);
                }
            }
        }
        render_board(m_game.board());
        render_house(m_game.house());
        SDL_Delay(100);
    }
}

void GameManager::close() { close_renderer(); }
