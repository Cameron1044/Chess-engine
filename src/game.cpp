#include <game.h>

#include <SDL.h>

void Game::run() {
    while(running_m) {
        moveEventFlag_m = false;
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            handleEvent(e);
        }
        handleSelection();
        view_m.draw(board_m, mouse_m);
    }
}

void Game::handleSelection() {
    // Temp logic before moveGen and validMove are implemented
    if (moveEventFlag_m) {
        int startFile = mouse_m.getxPressed(true);
        int startRank = mouse_m.getyPressed(true);
        if (board_m.isEmpty(startFile, startRank)) {return;}

        int endFile = mouse_m.getxReleased(true);
        int endRank = mouse_m.getyReleased(true);
        if (board_m.isEmpty(endFile, endRank)) {
            board_m.makeMove(startFile, startRank, endFile, endRank);
            return;
        }

        if ((piece::COLOR_MASK & board_m.getPieceAt(startFile, startRank)) != (piece::COLOR_MASK & board_m.getPieceAt(endFile, endRank))) {
            board_m.makeMove(startFile, startRank, endFile, endRank);
            return;
        }
    }
}

void Game::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            running_m = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                mouse_m.setPressed(e.button.x, e.button.y);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                mouse_m.setNotPressed(e.button.x, e.button.y);
                moveEventFlag_m = true;
            }
            break;
        case SDL_MOUSEMOTION:
            if (mouse_m.isPressed()) {
                int xPos, yPos;
                SDL_GetMouseState(&xPos, &yPos);
                mouse_m.setWhilePressed(xPos, yPos);
            }
            break;
    }
}