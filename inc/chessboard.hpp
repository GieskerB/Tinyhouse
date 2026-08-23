#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SDL3/SDL_pixels.h>

constexpr float BOARD_SIZE = 800;
constexpr float HOUSE_SIZE = BOARD_SIZE / 6 /*4*/;
constexpr float WIDTH = BOARD_SIZE + HOUSE_SIZE;
constexpr float HEIGHT = BOARD_SIZE;

constexpr uint8_t TILE_COUNT = 4;
constexpr float TILE_SIZE = BOARD_SIZE / TILE_COUNT;

constexpr SDL_Color DARK_TILE_COLOR{100, 50, 0, 255};
constexpr SDL_Color LIGHT_TILE_COLOR{255, 225, 195, 255};
constexpr SDL_Color HOUSE_COLOR{220,210,200,255};
constexpr SDL_Color LINE_COLOR{15,10,5,255};

void init_window();

void draw_board();

void close_window();

#endif