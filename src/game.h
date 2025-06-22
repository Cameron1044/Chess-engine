#pragma once

#include <SDL.h>
#include <iostream>
#include "ui/renderer.h"
#include "engine/board.h"

class Game {
    public:
        void run();

    private:
        void handleEvent(const SDL_Event&);
        void onSquareClicked(int file, int rank);
        
        Board board_m;//{"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"};
        Renderer view_m;
        bool running_m = true;
            
        bool pieceSelected_m = false;
        int selFile_m, selRank_m;
};