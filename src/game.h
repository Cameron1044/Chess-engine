#pragma once

#include "chess.h"
#include "ui/renderer.h"
#include "engine/board.h"
#include "engine/moveGen.h"
#include <algorithm>
#include <SDL.h>
#include <optional>

struct Mouse {
    bool down = false;
    chess::Coord coord{0, 0};
    chess::Tile tile{0, 0};
};

class Game {
    public:
        void run();

    private:
        void handleEvent(const SDL_Event&);
        void handleClick(chess::Tile tile);
        void handleRelease(chess::Tile tile);
        bool attemptMakeMove(chess::Tile tile);
        
        Board board_; //{"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"};
        // Board board_{"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1"};
        Renderer view_;
        MoveGen moveGen_;
        Mouse mouse_;
        bool running_ = true;

        std::optional<chess::Tile> selection_;
        std::vector<chess::Tile> selectionLegalMoves_;
};