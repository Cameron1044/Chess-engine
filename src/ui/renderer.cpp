#include "ui/renderer.h"

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>
#include <string>
#include <memory>

namespace {
    // Helpers
    char pieceToChar(uint8_t pieceInt) {
        const uint8_t type = piece::getType(pieceInt);
        const char piece = piece::toChar(type);
        if (piece::isWhite(pieceInt)) {
            return piece;
        }
        return std::toupper(piece);
    }

    bool isDark(chess::Tile tile) {
        return (tile.file + tile.rank) % 2 != 0;
    }
}

void Renderer::beginFrame() {
    SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
    SDL_RenderClear(renderer_.get());
}

void Renderer::endFrame() {
    SDL_RenderPresent(renderer_.get());
}

void Renderer::initSDL() {
    const int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    const int windowFlags = 0;

    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));

    // Create window
    const int w = chess::tileSize*8 + evalBarWidth_;
    const int h = chess::tileSize*8;
    window_ = sdlw::WindowPtr{
        SDL_CreateWindow("Chess++", // window title
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // window location
                         w, h, // window size
                         windowFlags)
    };
    if (!window_) 
        throw std::runtime_error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));

    // Create renderer
    renderer_ = sdlw::RendererPtr{
        SDL_CreateRenderer(window_.get(), // window raw pointer
                           -1, // automatically assign render driver that supports the render flags
                           rendererFlags)
    };
    if (!renderer_) 
        throw std::runtime_error("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));

    // TODO make a helper to do this. Char map key should be determined from file names. This is ugly
    pieceTextures_['p'] = sdlw::loadTexture(renderer_.get(), "wP.png");
    pieceTextures_['b'] = sdlw::loadTexture(renderer_.get(), "wB.png");
    pieceTextures_['k'] = sdlw::loadTexture(renderer_.get(), "wK.png");
    pieceTextures_['n'] = sdlw::loadTexture(renderer_.get(), "wN.png");
    pieceTextures_['q'] = sdlw::loadTexture(renderer_.get(), "wQ.png");
    pieceTextures_['r'] = sdlw::loadTexture(renderer_.get(), "wR.png");
    pieceTextures_['P'] = sdlw::loadTexture(renderer_.get(), "bP.png");
    pieceTextures_['B'] = sdlw::loadTexture(renderer_.get(), "bB.png");
    pieceTextures_['K'] = sdlw::loadTexture(renderer_.get(), "bK.png");
    pieceTextures_['N'] = sdlw::loadTexture(renderer_.get(), "bN.png");
    pieceTextures_['Q'] = sdlw::loadTexture(renderer_.get(), "bQ.png");
    pieceTextures_['R'] = sdlw::loadTexture(renderer_.get(), "bR.png");
    circleTexture_ = sdlw::loadTexture(renderer_.get(), "bigCircle.png");
    dotTexture_ = sdlw::loadTexture(renderer_.get(), "blackCircle.png");
}

void Renderer::drawBoard() {

    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {

            chess::Tile tile{file, rank};

            RGB tileColor = isDark(tile) ? darkTile_ : lightTile_;
            drawTile(tile, tileColor);
        }
    }
}

void Renderer::drawSelection(const BoardPtr& boardPtr,
                             const chess::Tile& tile, 
                             const MoveList& legalMoves) {

    // draw selection
    RGB color = isDark(tile) ? darkTileSelected_ : lightTileSelected_;
    drawTile(tile, color);

    // draw legal moves of selected piece
    for (auto move : legalMoves) {
        bool capture = move.isCapture();
        RGB color = capture ? RGB{37, 150, 190} : RGB{218,11,56};
        drawTile(move.getToTile(), color);
    }
}

void Renderer::drawPieces(const BoardPtr& boardPtr, 
                          const std::optional<chess::Tile>& selectedTile) {

    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            chess::Tile tile{file, rank};

            // if no piece then keep going
            if (boardPtr->isEmptyAt(tile)) 
                continue;

            // don't draw piece at selected tile
            if (selectedTile && tile == *selectedTile) 
                continue;

            // draw piece
            char piece = pieceToChar(boardPtr->getPieceAt(tile));
            drawTextureOnTile(tile, pieceTextures_[piece]);
        }
    }
}

void Renderer::drawEvalBar(int wVal, int bVal) {
    const int bH = chess::tileSize*8 * bVal / (wVal + bVal);

    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);

    SDL_Rect Rect{chess::tileSize*8, 0, evalBarWidth_, bH};
    SDL_RenderFillRect(renderer_.get(), 
                       &Rect);
}

void Renderer::drawPieceAtCoord(const BoardPtr& boardPtr, 
                                const chess::Tile& tile, 
                                const chess::Coord& coord) {

    const char piece = pieceToChar(boardPtr->getPieceAt(tile));

    const int x = coord.x - chess::tileSize/2;
    const int y = coord.y - chess::tileSize/2;

    drawTextureAtCoord(x, y, pieceTextures_[piece]);
}

void Renderer::drawTile(chess::Tile tile, 
                        const RGB& C) {

    const int x = tile.file*chess::tileSize;
    const int y = abs(tile.rank - 7)*chess::tileSize;

    const int w = chess::tileSize;
    const int h = w;

    SDL_SetRenderDrawColor(renderer_.get(),
                           C.R, C.G, C.B,
                           255);

    SDL_Rect Rect{x, y, w, h};
    SDL_RenderFillRect(renderer_.get(), 
                       &Rect);
}

void Renderer::drawTextureAtCoord(int x, 
                                  int y, 
                                  const sdlw::TexturePtr& texture) {

    if (!texture)
        throw std::runtime_error("drawTexture: nullptr texture");

    const int w = chess::tileSize;
    const int h = w;

    SDL_Rect rect{x, y, w, h};
    SDL_RenderCopy(renderer_.get(),
                   texture.get(),
                   nullptr,
                   &rect);
}

void Renderer::drawTextureOnTile(chess::Tile tile, 
                                 const sdlw::TexturePtr& texture) {

    if (!texture)
        throw std::runtime_error("drawTexture: nullptr texture");

    const int x = tile.file*chess::tileSize;
    const int y = abs(tile.rank - 7)*chess::tileSize;

    const int w = chess::tileSize;
    const int h = w;

    SDL_Rect rect{x, y, w, h};
    SDL_RenderCopy(renderer_.get(),
                    texture.get(),
                    nullptr,
                    &rect);
}
