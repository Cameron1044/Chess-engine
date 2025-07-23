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
#include "engine/move.h"

class Renderer {
    public:
        Renderer() {initSDL();}
        ~Renderer() {SDL_Quit();}

        void beginFrame();
        void endFrame();

        void drawBoard();
        void drawSelection(const BoardPtr& boardPtr, const chess::Tile& tile, const MoveList& legalMoves);
        void drawPieces(const BoardPtr& boardPtr, const std::optional<chess::Tile>& selectedTile = std::nullopt);
        void drawPieceAtCoord(const BoardPtr& boardPtr, const chess::Tile& tile, const chess::Coord& coord);
        void drawEvalBar(int wValue, int bValue);
    
    private:
        struct RGB {int R, G, B;};

        void initSDL();

        // Draw helpers
        void drawTile(chess::Tile tile, const RGB& C);
        void drawTextureAtCoord(int x, int y, const sdlw::TexturePtr& texture);
        void drawTextureOnTile(chess::Tile tile, const sdlw::TexturePtr& texture);

        sdlw::WindowPtr window_;
        sdlw::RendererPtr renderer_;

        // Sizes
        const int evalBarWidth_ = 15;

        // Colors
        const RGB lightTile_{235,236,208};
        const RGB darkTile_{115,149,82};
        const RGB lightTileSelected_{245,246,131};
        const RGB darkTileSelected_{185,202,67};

        // Textures
        std::unordered_map<char, sdlw::TexturePtr> pieceTextures_;
        sdlw::TexturePtr circleTexture_;
        sdlw::TexturePtr dotTexture_;
};