#pragma once

#include <array>
#include <unordered_map>
#include <SDL.h>
#include <optional>
#include <iostream>
#include <vector>

#include "chess.h"
#include "engine/board.h"
#include "ui/SDL_RAII.h"

struct Color {
    int R = 0;
    int G = 0;
    int B = 0;
};

class Renderer {
    public:
        Renderer() {initSDL();}
        ~Renderer() {SDL_Quit();}

        void beginFrame();
        void endFrame();

        void drawBoard();
        void drawSelection(const chess::Tile& tile, const Board& board, const std::vector<chess::Tile>& legalMoves);
        void drawPieces(const Board& board, const std::optional<chess::Tile>& selectedTile = std::nullopt);
        void drawPieceAtCoord(const Board& board, const chess::Tile& tile, const chess::Coord& coord);
    
    private:
        void initSDL();

        // Draw helpers
        void drawTile(chess::Tile tile, const Color& C);
        void drawTextureAtCoord(int x, int y, const sdlw::TexturePtr& texture);
        void drawTextureOnTile(chess::Tile tile, const sdlw::TexturePtr& texture);

        sdlw::WindowPtr window_;
        sdlw::RendererPtr renderer_;

        // Colors
        Color lightTile_{235,236,208};
        Color darkTile_{115,149,82};
        Color lightTileSelected_{245,246,131};
        Color darkTileSelected_{185,202,67};

        // Textures
        std::unordered_map<char, sdlw::TexturePtr> pieceTextures_;
        sdlw::TexturePtr circleTexture_;
        sdlw::TexturePtr dotTexture_;
};