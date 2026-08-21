#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SDL3/SDL_pixels.h>

constexpr float WIDTH = 800, HEIGHT = 800, BOARD_SIZE = HEIGHT;

const SDL_Color DARK_TILE{100, 50, 0, 255}, LIGHT_TILE{255, 225, 195, 255};

void init_window();

void draw_board();

void close_window();

#endif