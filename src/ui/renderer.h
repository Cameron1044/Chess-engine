#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string_view>
#include "engine/board.h"

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

struct select_t {
    int x, y;
    bool selected = false;
};

class Renderer {
    public:
        Renderer();
        ~Renderer();
        void draw(const Board& board);
        select_t select;
        
    private:
        void initSDL();
        void drawBoard();
        void drawSelection();
        void drawPieces(const Board& board);

        const int tile_size_m = 100;
        App app_m;
        std::unordered_map<char, SDL_Texture*> piece_textures_m;
};