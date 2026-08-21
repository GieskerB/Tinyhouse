#include <iostream>

#include "../inc/chessboard.hpp"

int main() {
    init_window();
    draw_board();

    int input;
    std::cin >> input;

    close_window();
    return 0;
}
