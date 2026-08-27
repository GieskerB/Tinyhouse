#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../inc/board.hpp"
#include "../inc/house.hpp"

constexpr float BOARD_SIZE = 800;
constexpr float HOUSE_SIZE = BOARD_SIZE / 6 /*4*/;
constexpr float WIDTH = BOARD_SIZE + HOUSE_SIZE;
constexpr float HEIGHT = BOARD_SIZE;

constexpr float TILE_SIZE = BOARD_SIZE / Board::SIZE;

void init_renderer();

void render_board(const Board&);
void render_house(const House&);

void close_renderer();

#endif