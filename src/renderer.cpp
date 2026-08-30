#include "../inc/renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>

#include <iostream>

#include "../inc/piece.hpp"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static inline bool SET_COLOR(const SDL_Color& color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

constexpr SDL_Color DARK_TILE_COLOR{100, 50, 0, 255};
constexpr SDL_Color LIGHT_TILE_COLOR{255, 225, 195, 255};
constexpr SDL_Color DARK_TILE_SELECTED_COLOR{0, 50, 100, 255};
constexpr SDL_Color LIGHT_TILE_SELECTED_COLOR{195, 225, 255, 255};
constexpr SDL_Color HOUSE_COLOR{220, 210, 200, 255};
constexpr SDL_Color HOUSE_SELECTED_COLOR{200, 210, 220, 255};
constexpr SDL_Color LINE_COLOR{15, 10, 5, 255};

static std::array<SDL_Texture*, 10> textures{nullptr};

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

static inline void create_texture(const char* path, const piece p) {
    SDL_Surface* surf = SDL_LoadPNG(path);
    textures[PIECE_INDEX(p)] = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

static void load_pieces() {
    create_texture("assets/White_Ferz.png", const_piece::white_ferz);
    create_texture("assets/White_Hors.png", const_piece::white_hors);
    create_texture("assets/White_King.png", const_piece::white_king);
    create_texture("assets/White_Pawn.png", const_piece::white_pawn);
    create_texture("assets/White_Wazir.png", const_piece::white_wazir);

    create_texture("assets/Black_Ferz.png", const_piece::black_ferz);
    create_texture("assets/Black_Hors.png", const_piece::black_hors);
    create_texture("assets/Black_King.png", const_piece::black_king);
    create_texture("assets/Black_Pawn.png", const_piece::black_pawn);
    create_texture("assets/Black_Wazir.png", const_piece::black_wazir);
}

void init_renderer() {
    init_SDL();
    load_pieces();
}

void render_board(const Board& board) {
    // Render Board background
    for (unsigned char rank = 0; rank < Board::SIZE; ++rank) {
        for (unsigned char file = 0; file < Board::SIZE; ++file) {
            const auto& selected_color = (file + rank) % 2 == 0 ? LIGHT_TILE_SELECTED_COLOR : DARK_TILE_SELECTED_COLOR;
            const auto& color = (file + rank) % 2 == 0 ? LIGHT_TILE_COLOR : DARK_TILE_COLOR;
            // SET_COLOR(color);
            // SET_COLOR(board.is_highlighted(Board::get_index(rank, file)) ? selected_color : color);
            const SDL_FRect tile{file * TILE_SIZE + HOUSE_SIZE, rank * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    // Render Pieces on top
    for (uint8_t rank = 0; rank < Board::SIZE; ++rank) {
        for (uint8_t file = 0; file < Board::SIZE; ++file) {
            const piece p = board.get_piece(Board::get_index(rank, file));
            if (p == const_piece::null_piece) continue;
            const SDL_FRect place{file * TILE_SIZE + HOUSE_SIZE, rank * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderTexture(renderer, textures[PIECE_INDEX(p)], NULL, &place);
        }
    }

    SDL_RenderPresent(renderer);
}

void render_house(const House& house) {
    // First half is white, second half is black stripped by unimportant indices
    static constexpr std::array<float, 8> y_lookup{
        BOARD_SIZE / 2,                      // white pawn
        BOARD_SIZE / 2 + 3 * TILE_SIZE / 2,  // white ferz
        BOARD_SIZE / 2 + TILE_SIZE,          // white hors
        BOARD_SIZE / 2 + TILE_SIZE / 2,      // white wazir
        3 * TILE_SIZE / 2,                   // black pawn
        0,                                   // black ferz
        TILE_SIZE / 2,                       // black Hors
        TILE_SIZE,                           // black wazir
    };
    // Background
    SET_COLOR(HOUSE_COLOR);
    const SDL_FRect house_rect{0, 0, HOUSE_SIZE, HEIGHT};
    SDL_RenderFillRect(renderer, &house_rect);

    // Helper lambda
    auto render = [](float x, float y, piece p) {
        const SDL_FRect place{x, y, TILE_SIZE / 2, TILE_SIZE / 2};
        SDL_RenderTexture(renderer, textures[PIECE_INDEX(p)], NULL, &place);
    };

    // Render either 0 / 1 / 2 pieces in house
    for (uint8_t index = 0; index < House::HOUSE_SIZE; ++index) {
        // if (house.is_highlighted(index)) {
        //     SET_COLOR(HOUSE_SELECTED_COLOR);
        //     const SDL_FRect select_rect{0, TILE_SIZE / 2 * index, HOUSE_SIZE, TILE_SIZE / 2};
        //     SDL_RenderFillRect(renderer, &select_rect);
        // }

        const uint8_t count = house.count(index);
        if (count == 0) continue;
        const float y = y_lookup[index];
        const piece p = House::index_to_piece(index);
        render(0, y, p);
        if (count == 2) render(HOUSE_SIZE / 4, y, p);
    }
    // Boundary line
    SET_COLOR(LINE_COLOR);
    for (char i = -1; i <= 1; ++i) {
        SDL_RenderLine(renderer, HOUSE_SIZE + i, 0, HOUSE_SIZE + i, HEIGHT);
    }

    SDL_RenderPresent(renderer);
}

void close_renderer() { SDL_Quit(); }
