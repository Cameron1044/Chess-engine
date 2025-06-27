#include "ui/renderer.h"
#include <iostream>
#include <stdexcept>

namespace {
    // Helper
    char pieceToChar(uint8_t pieceInt) {
        uint8_t type = piece::getTypeID(pieceInt);
        char piece = piece::intToChar(type);
        if (piece::isWhite(pieceInt)) {
            return piece;
        }
        return std::toupper(piece);
    }
}

Renderer::Renderer() {
    initSDL();
}

Renderer::~Renderer() {
    SDL_Quit();
}

void Renderer::draw(const Board& board, const MouseState& mouse) {
    SDL_SetRenderDrawColor(renderer_m.get(), 255, 255, 255, 255);
    SDL_RenderClear(renderer_m.get());

    drawBoard();
    drawSelection(board, mouse);
    drawPieces(board, mouse);

    SDL_RenderPresent(renderer_m.get());
}

void Renderer::initSDL() {
    const int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    const int windowFlags = 0;

    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));

    // Create window
    window_m = sdlw::WindowPtr{
        SDL_CreateWindow("Chess++", // window title
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // window location
                         tile_size_m*8, tile_size_m*8, // window size
                         windowFlags)
    };
    if (!window_m) 
        throw std::runtime_error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));

    // Create renderer
    renderer_m = sdlw::RendererPtr{
        SDL_CreateRenderer(window_m.get(), // window raw pointer
                           -1, // automatically assign render driver that supports the render flags
                           rendererFlags)
    };
    if (!renderer_m) 
        throw std::runtime_error("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));

    // TODO make a helper to do this. Char map key should be determined from file names. This is ugly
    pieceTextures_m['p'] = sdlw::loadTexture(renderer_m.get(), "wP.png");
    pieceTextures_m['b'] = sdlw::loadTexture(renderer_m.get(), "wB.png");
    pieceTextures_m['k'] = sdlw::loadTexture(renderer_m.get(), "wK.png");
    pieceTextures_m['n'] = sdlw::loadTexture(renderer_m.get(), "wN.png");
    pieceTextures_m['q'] = sdlw::loadTexture(renderer_m.get(), "wQ.png");
    pieceTextures_m['r'] = sdlw::loadTexture(renderer_m.get(), "wR.png");
    pieceTextures_m['P'] = sdlw::loadTexture(renderer_m.get(), "bP.png");
    pieceTextures_m['B'] = sdlw::loadTexture(renderer_m.get(), "bB.png");
    pieceTextures_m['K'] = sdlw::loadTexture(renderer_m.get(), "bK.png");
    pieceTextures_m['N'] = sdlw::loadTexture(renderer_m.get(), "bN.png");
    pieceTextures_m['Q'] = sdlw::loadTexture(renderer_m.get(), "bQ.png");
    pieceTextures_m['R'] = sdlw::loadTexture(renderer_m.get(), "bR.png");
    circleTexture_m = sdlw::loadTexture(renderer_m.get(), "bigCircle.png");
    dotTexture_m = sdlw::loadTexture(renderer_m.get(), "blackCircle.png");
}

void Renderer::drawBoard() {
    // RGB Colors
    std::array<int, 3> lightTile = {238, 238, 210};
    std::array<int, 3> darkTile = {118, 150, 86};

    // Iterates through all tiles and draws them
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            bool isDark = (file + rank) % 2 != 0;

            if (isDark) {
                drawTile(file, rank, darkTile);
            } else {
                drawTile(file, rank, lightTile);
            }
        }
    }
}

void Renderer::drawSelection(const Board& board, const MouseState& mouse) {
    // Draws circle on selectd tile while dragging piece
    if (mouse.isPressed()) {
        int file = mouse.getxPressed(true);
        int rank = mouse.getyPressed(true);

        if (board.isEmpty(file, rank)) return;

        drawTexture(file * tile_size_m, 
                    abs(rank-7) * tile_size_m,
                    circleTexture_m);
    }
}

void Renderer::drawPieces(const Board& board, const MouseState& mouse) {
    bool renderSelected = false;
    char pieceSelected;

    // iterates through all tiles and draws piece if it exists
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            // if no piece then keep going
            if (board.isEmpty(file, rank)) {
                continue;
            }

            char piece = pieceToChar(board.getPieceAt(file, rank));

            // if piece at this tile is being dragged then keep going, store the piece and draw later
            if (mouse.isPressed() && mouse.getxPressed(true) == file && mouse.getyPressed(true) == rank) {
                renderSelected = true;
                pieceSelected = piece;
                continue;
            }

            drawTexture(file*tile_size_m, 
                        abs(rank-7) * tile_size_m, 
                        pieceTextures_m[piece]);
        }
    }

    // if piece was stored then draw at location of mouse
    if (renderSelected) {
        int x = mouse.getxWhilePressed() - tile_size_m/2;
        int y = mouse.getyWhilePressed() - tile_size_m/2;
        drawTexture(x, y, pieceTextures_m[pieceSelected]);
    }
}

void Renderer::drawTile(int file, int rank, const std::array<int, 3>& RGB) {
    SDL_SetRenderDrawColor(renderer_m.get(),
                           RGB[0], RGB[1], RGB[2], // RGB integers 0-255 inclusive
                           255); // alpha value. always 255

    SDL_Rect Rect{file*tile_size_m, // x coord
                  abs(rank-7)*tile_size_m, // y coord (flip veritical since SDL uses top of window as y=0)
                  tile_size_m, tile_size_m}; // width and height

    SDL_RenderFillRect(renderer_m.get(), 
                       &Rect);
}

void Renderer::drawTexture(int x, int y, const sdlw::TexturePtr& texture) {
    if (!texture)
        throw std::runtime_error("drawTexture: nullptr texture");

    SDL_Rect rect{x, y, tile_size_m, tile_size_m};

    SDL_RenderCopy(renderer_m.get(),
                    texture.get(),
                    nullptr, // null for entire texture
                    &rect);
}
