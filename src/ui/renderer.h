#pragma once

#include <format>
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>

#include "engine/board.h"
#include "ui/mouseState.h"
#include "ui/SDL_RAII.h"

class Renderer {
    public:
        Renderer();
        ~Renderer();
        void draw(const Board& board, const MouseState& mouse);
        
    private:
        void initSDL();

        // Drawers
        void drawBoard();
        void drawSelection(const Board& board, const MouseState& mouse);
        void drawPieces(const Board& board, const MouseState& mouse);

        // Draw helpers
        void drawTile(int file, int rank, const std::array<int, 3>& RGB);
        void drawTexture(int x, int y, const sdlw::TexturePtr& texture);

        const int tile_size_m = 100;

        sdlw::WindowPtr window_m;
        sdlw::RendererPtr renderer_m;

        // Textures
        std::unordered_map<char, sdlw::TexturePtr> pieceTextures_m;
        sdlw::TexturePtr circleTexture_m;
        sdlw::TexturePtr dotTexture_m;
};