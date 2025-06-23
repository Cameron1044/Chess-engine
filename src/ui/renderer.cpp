#include "ui/renderer.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>

namespace {
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& relPath) {
        std::filesystem::path full = std::filesystem::path(SDL_GetBasePath()) / "assets" / relPath;

        SDL_Surface* surface = IMG_Load(full.string().c_str());
        if (!surface) {
            std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {            
            std::cerr << "CreateTexture failed:" << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return texture;
    }

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
    if (app_m.renderer) SDL_DestroyRenderer(app_m.renderer);
    if (app_m.window) SDL_DestroyWindow(app_m.window);
    for (auto& [_, texture] : pieceTextures_m) SDL_DestroyTexture(texture);
    SDL_Quit();
}

void Renderer::draw(const Board& board, const MouseState& mouse) {
    SDL_SetRenderDrawColor(app_m.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app_m.renderer);

    drawBoard();
    drawSelection(board, mouse);
    drawPieces(board, mouse);

    SDL_RenderPresent(app_m.renderer);
}

void Renderer::initSDL() {
    const int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    const int windowFlags = 0;

    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Create window
    app_m.window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tile_size_m*8, tile_size_m*8, windowFlags);
    if (!app_m.window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Create renderer
    app_m.renderer = SDL_CreateRenderer(app_m.window, -1, rendererFlags);
    if (!app_m.renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // TODO make a helper to do this
    pieceTextures_m['p'] = loadTexture(app_m.renderer, "wP.png");
    pieceTextures_m['b'] = loadTexture(app_m.renderer, "wB.png");
    pieceTextures_m['k'] = loadTexture(app_m.renderer, "wK.png");
    pieceTextures_m['n'] = loadTexture(app_m.renderer, "wN.png");
    pieceTextures_m['q'] = loadTexture(app_m.renderer, "wQ.png");
    pieceTextures_m['r'] = loadTexture(app_m.renderer, "wr.png");
    pieceTextures_m['P'] = loadTexture(app_m.renderer, "bP.png");
    pieceTextures_m['B'] = loadTexture(app_m.renderer, "bB.png");
    pieceTextures_m['K'] = loadTexture(app_m.renderer, "bK.png");
    pieceTextures_m['N'] = loadTexture(app_m.renderer, "bN.png");
    pieceTextures_m['Q'] = loadTexture(app_m.renderer, "bQ.png");
    pieceTextures_m['R'] = loadTexture(app_m.renderer, "bR.png");
    circleTexture_m = loadTexture(app_m.renderer, "bigCircle.png");
    dotTexture_m = loadTexture(app_m.renderer, "blackCircle.png");
}

void Renderer::drawBoard() {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            bool dark = (x + y) % 2 != 0;
            SDL_SetRenderDrawColor(app_m.renderer, 
                dark ? 118 : 238,
                dark ? 150 : 238,
                dark ? 86  : 210,
                255);
            SDL_Rect Rect{x*tile_size_m, y*tile_size_m, tile_size_m, tile_size_m};
            SDL_RenderFillRect(app_m.renderer, &Rect);
        }
    }
}

void Renderer::drawSelection(const Board& board, const MouseState& mouse) {
    if (mouse.isPressed()) {
        int file = mouse.getxPressed(true);
        int rank = mouse.getyPressed(true);
        if (board.isEmpty(file, rank)) {return;}
        SDL_Rect dst{file * tile_size_m, abs(rank-7) * tile_size_m, tile_size_m, tile_size_m};
        SDL_RenderCopy(app_m.renderer,
                    circleTexture_m, 
                    nullptr, &dst);
    }
}

void Renderer::drawPieces(const Board& board, const MouseState& mouse) {
    bool renderSelected;
    char pieceSelected;
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            if (board.isEmpty(file, rank)) {
                continue;
            }

            char piece = pieceToChar(board.getPieceAt(file, rank));

            if (mouse.isPressed() && mouse.getxPressed(true) == file && mouse.getyPressed(true) == rank) {
                renderSelected = true;
                pieceSelected = piece;
                continue;
            }
            SDL_Rect dst{file * tile_size_m, abs(rank-7) * tile_size_m, tile_size_m, tile_size_m};
            SDL_RenderCopy(app_m.renderer,
                        pieceTextures_m[piece], 
                        nullptr, &dst);
        }
    }
    if (renderSelected) {
        int x = mouse.getxWhilePressed() - tile_size_m/2;
        int y = mouse.getyWhilePressed() - tile_size_m/2;
        SDL_Rect dst{x, y, tile_size_m, tile_size_m};
        SDL_RenderCopy(app_m.renderer,
                    pieceTextures_m[pieceSelected], 
                    nullptr, &dst);
    }
}

