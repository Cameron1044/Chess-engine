#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string_view>
#include "engine/board.h"
#include "ui/mouseState.h"

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

class Renderer {
    public:
        Renderer();
        ~Renderer();
        void draw(const Board& board, const MouseState& mouse);
        
    private:
        void initSDL();
        void drawBoard();
        void drawSelection(const Board& board, const MouseState& mouse);
        void drawPieces(const Board& board, const MouseState& mouse);

        const int tile_size_m = 100;
        App app_m;
        std::unordered_map<char, SDL_Texture*> pieceTextures_m;
        SDL_Texture* circleTexture_m;
        SDL_Texture* dotTexture_m;
};