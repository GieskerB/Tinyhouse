#include "../inc/chessboard.hpp"

#include <SDL3/SDL.h>

#include <array>
#include <iostream>

#include "../inc/piece.hpp"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static inline bool SET_COLOR(const SDL_Color& color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

static inline uint8_t ARRAY_INDEX(uint8_t file, uint8_t rank) { return rank * TILE_COUNT + file; }

static std::array<SDL_Texture*, 13> textures{nullptr};  // index 5,6,7 stall nullptr!
static std::array<piece, 10> house{null_piece};
static std::array<piece, 16> board{
    black_ferz, black_hors,  black_wazir, black_king,  // rank 4
    null_piece, null_piece,  null_piece,  black_pawn,  // rank 3
    white_pawn, null_piece,  null_piece,  null_piece,  // rank 2
    white_king, white_wazir, white_hors,  white_ferz   // rank 1
};

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
    create_texture("assets/White_Ferz.png", white_ferz);
    create_texture("assets/White_Hors.png", white_hors);
    create_texture("assets/White_King.png", white_king);
    create_texture("assets/White_Pawn.png", white_pawn);
    create_texture("assets/White_Wazir.png", white_wazir);

    create_texture("assets/Black_Ferz.png", black_ferz);
    create_texture("assets/Black_Hors.png", black_hors);
    create_texture("assets/Black_King.png", black_king);
    create_texture("assets/Black_Pawn.png", black_pawn);
    create_texture("assets/Black_Wazir.png", black_wazir);
}

void init_window() {
    init_SDL();
    load_pieces();
}

void draw_pieces() {
    for (uint8_t file = 0; file < TILE_COUNT; ++file) {
        for (uint8_t rank = 0; rank < TILE_COUNT; ++rank) {
            const piece p = board[ARRAY_INDEX(file, rank)];
            const SDL_FRect place{file * TILE_SIZE + HOUSE_SIZE, rank * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if ((p & ON_BOARD) != 0) SDL_RenderTexture(renderer, textures[PIECE_INDEX(p)], NULL, &place);
        }
    }
}

void push_house(uint8_t file, uint8_t rank) {
    piece p = board[ARRAY_INDEX(file, rank)];
    board[ARRAY_INDEX(file, rank)] = null_piece;  // clear peace form board
    if (p == null_piece) return;
    p ^= ON_BOARD | IN_HOUSE;  // Move from board to house
    p ^= WHITE | BLACK;        // Switch color
    const uint8_t p_id = p & ID_MASK;
    // Move piece to 2nd rank if not alone
    if (house[p_id] == null_piece)
        house[p_id] = p;
    else
        house[p_id + 5] = p;
}

void pop_house(uint8_t house_index, uint8_t file, uint8_t rank) {
    piece p = house[house_index];
    house[house_index] = null_piece;  // clear peace form house
    if (p == null_piece) return;
    p ^= ON_BOARD | IN_HOUSE;  // Move from house to board
    const uint8_t p_id = p & ID_MASK;
    if (house[p_id + 5] != null_piece) {
        // Shift piece in house if two existed
        house[p_id] = house[p_id + 5];
        house[p_id + 5] = null_piece;
    }
    board[ARRAY_INDEX(file, rank)] = p;  // place peace on board
}

void draw_house() {
    // First half is white, second half is black
    static constexpr std::array<float, 13> y_lookup{
        BOARD_SIZE / 2,                      // white pawn
        BOARD_SIZE / 2 + 3 * TILE_SIZE / 2,  // white ferz
        BOARD_SIZE / 2 + TILE_SIZE,          // white hors
        BOARD_SIZE / 2 + TILE_SIZE / 2,      // white wazir
        -1                                   // white king
        -2,                                  // spacer
        -2,                                  // spacer
        -2,                                  // spacer
        3 * TILE_SIZE / 2,                   // black pawn
        0,                                   // black ferz
        TILE_SIZE / 2,                       // black Hors
        TILE_SIZE,                           // black wazir
        -1,                                  // black king
    };
    SET_COLOR(HOUSE_COLOR);
    const SDL_FRect house_rect{0, 0, HOUSE_SIZE, HEIGHT};
    SDL_RenderFillRect(renderer, &house_rect);
    SET_COLOR(LINE_COLOR);
    for (char i = -1; i <= 1; ++i) {
        SDL_RenderLine(renderer, HOUSE_SIZE + i, 0, HOUSE_SIZE + i, HEIGHT);
    }
    for (uint8_t index = 0; index < house.size(); ++index) {
        const piece p = house[index];
        const float x = index < 5 ? 0 : HOUSE_SIZE / 4;
        const float y = y_lookup[PIECE_INDEX(p)];
        const SDL_FRect place{x, y, TILE_SIZE / 2, TILE_SIZE / 2};
        if ((p & IN_HOUSE) != 0) {
            SDL_RenderTexture(renderer, textures[PIECE_INDEX(p)], NULL, &place);
        }
    }
}

void draw_board() {
    for (unsigned char file = 0; file < TILE_COUNT; ++file) {
        for (unsigned char rank = 0; rank < TILE_COUNT; ++rank) {
            const auto& color = (file + rank) % 2 == 0 ? LIGHT_TILE_COLOR : DARK_TILE_COLOR;
            SET_COLOR(color);
            const SDL_FRect tile{file * TILE_SIZE + HOUSE_SIZE, rank * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    // push_house(0,0);
    // push_house(2,3);

    // pop_house(2, 0,0);
    
    draw_pieces();
    draw_house();
    SDL_RenderPresent(renderer);
}

void close_window() { SDL_Quit(); }