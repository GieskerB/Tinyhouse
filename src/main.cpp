#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <iostream>

#include "../inc/game_manager.hpp"
#include "../inc/game.hpp"

// File = column
// Rank = row

int main() {
    Game game{};

    for(uint8_t i= 0; i< 16; ++i){
        std::cout << "Piece: " << +i << "\n";
        auto map = valid_move_bitmap(game.board(), i);
        for (uint16_t mask = 0b1; mask != 0; mask <<= 1) {
            if ((mask & map) != 0) {
                std::cout << '#';
            } else if (mask == (0b1 << (i))) {
                std::cout << '+';
            } else {
                std::cout << '.';
            }
            if ((mask == 0b1000) or (mask == 0b10000000) or (mask == 0b100000000000)) std::cout << '\n';
        }
        std::cout << "\n\n";
    }

    return 0;
}
