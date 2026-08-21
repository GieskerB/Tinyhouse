#include "../inc/chessboard.hpp"

#include <SDL3/SDL.h>

#include <array>
#include <iostream>

#include "../inc/piece.hpp"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static const unsigned char TILE_COUNT = 4;
static const float TILE_SIZE = BOARD_SIZE / TILE_COUNT;

static std::array<SDL_Texture*, 10> textures{nullptr};

static std::array<piece, 10> pieces{white_ferz, white_hors, white_king, white_pawn, white_wazir,
                                    black_ferz, black_hors, black_king, black_pawn, black_wazir};

static inline void init_SDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }
    if (!(window = SDL_CreateWindow("Tiny House", WIDTH, HEIGHT, 0))) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }
    if (!(renderer = SDL_CreateRenderer(window, ""))) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }
}

static inline void create_texture(const char* path, unsigned char id) {
    SDL_Surface* surf = SDL_LoadPNG(path);
    textures[id] = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

static void load_pieces() {
    create_texture("assets/White_Ferz.png", 0);
    create_texture("assets/White_Hors.png", 1);
    create_texture("assets/White_King.png", 2);
    create_texture("assets/White_Pawn.png", 3);
    create_texture("assets/White_Wazir.png", 4);

    create_texture("assets/Black_Ferz.png", 5);
    create_texture("assets/Black_Hors.png", 6);
    create_texture("assets/Black_King.png", 7);
    create_texture("assets/Black_Pawn.png", 8);
    create_texture("assets/Black_Wazir.png", 9);
}

void init_window() {
    init_SDL();
    load_pieces();
}

void draw_pieces() {
    for (const auto& p : pieces) {
        SDL_FRect place{p.pos_x * TILE_SIZE, p.pos_y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderTexture(renderer, textures[p.id], NULL, &place);
    }
}

void draw_board() {
    for (unsigned char x = 0; x < TILE_COUNT; ++x) {
        for (unsigned char y = 0; y < TILE_COUNT; ++y) {
            const auto& c = (x + y) % 2 == 0 ? LIGHT_TILE : DARK_TILE;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            const SDL_FRect tile{x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    draw_pieces();
    SDL_RenderPresent(renderer);
}

void close_window() { SDL_Quit(); }