#include "../inc/chessboard.hpp"

#include <SDL3/SDL.h>

#include <array>
#include <iostream>

#include "../inc/piece.hpp"

#define TEX_ID_MACRO_DIRECT(id, is_black) id + (is_black ? 0 : 5)
#define TEX_ID_MACRO(piece) TEX_ID_MACRO_DIRECT(piece.id, piece.is_black)
#define SET_COLOR_MACRO(color) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static constexpr unsigned char TILE_COUNT = 4;
static constexpr float TILE_SIZE = BOARD_SIZE / TILE_COUNT;

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

static inline void create_texture(const char* path, unsigned char id, bool is_black) {
    SDL_Surface* surf = SDL_LoadPNG(path);
    textures[TEX_ID_MACRO_DIRECT(id, is_black)] = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

static void load_pieces() {
    create_texture("assets/White_Ferz.png", FERZ_ID, false);
    create_texture("assets/White_Hors.png", HORS_ID, false);
    create_texture("assets/White_King.png", KING_ID, false);
    create_texture("assets/White_Pawn.png", PAWN_ID, false);
    create_texture("assets/White_Wazir.png", WAZIR_ID, false);

    create_texture("assets/Black_Ferz.png", FERZ_ID, true);
    create_texture("assets/Black_Hors.png", HORS_ID, true);
    create_texture("assets/Black_King.png", KING_ID, true);
    create_texture("assets/Black_Pawn.png", PAWN_ID, true);
    create_texture("assets/Black_Wazir.png", WAZIR_ID, true);
}

void init_window() {
    init_SDL();
    load_pieces();
}

void draw_pieces() {
    for (const auto& p : pieces) {
        const SDL_FRect place{p.pos_x * TILE_SIZE + POCKET_SIZE, p.pos_y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        if (p.on_board) SDL_RenderTexture(renderer, textures[TEX_ID_MACRO(p)], NULL, &place);
    }
}

void draw_pocket() {
    // First half is white, second half is black
    static constexpr float y_lookup[]{3 * TILE_SIZE / 2,
                                      TILE_SIZE,
                                      TILE_SIZE / 2,
                                      0,
                                      0,  // White KING_ID spacer
                                      BOARD_SIZE / 2,
                                      BOARD_SIZE / 2 + TILE_SIZE / 2,
                                      BOARD_SIZE / 2 + TILE_SIZE,
                                      BOARD_SIZE / 2 + 3 * TILE_SIZE / 2};
    bool pocket_empty[10]{false};
    SET_COLOR_MACRO(POCKET_COLOR);
    const SDL_FRect pocket_rect {0,0, POCKET_SIZE, HEIGHT};
    SDL_RenderFillRect(renderer, &pocket_rect);
    SET_COLOR_MACRO(LINE_COLOR);
    for(char i = -1; i <= 1; ++i) {
        SDL_RenderLine(renderer, POCKET_SIZE+i, 0, POCKET_SIZE+i, HEIGHT);
    }
    for (const auto& p : pieces) {
        if (p.id == KING_ID) continue;
        const float x = pocket_empty[TEX_ID_MACRO(p)] ? POCKET_SIZE / 4 : 0;
        const float y = y_lookup[TEX_ID_MACRO(p)];
        const SDL_FRect place{x, y, TILE_SIZE / 2, TILE_SIZE / 2};
        if (!p.on_board) {
            SDL_RenderTexture(renderer, textures[TEX_ID_MACRO(p)], NULL, &place);
            pocket_empty[TEX_ID_MACRO(p)] = true;
        }
    }
}

void draw_board() {
    for (unsigned char x = 0; x < TILE_COUNT; ++x) {
        for (unsigned char y = 0; y < TILE_COUNT; ++y) {
            const auto& color = (x + y) % 2 == 0 ? LIGHT_TILE_COLOR : DARK_TILE_COLOR;
            SET_COLOR_MACRO(color);
            const SDL_FRect tile{x * TILE_SIZE + POCKET_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    draw_pieces();
    draw_pocket();
    SDL_RenderPresent(renderer);
}

void close_window() { SDL_Quit(); }