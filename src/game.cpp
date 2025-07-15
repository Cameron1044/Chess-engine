#include <game.h>
#include <SDL.h>
#include <iostream>

void Game::run() {
    while(running_) {
        view_.beginFrame();
        view_.drawBoard();

        for (SDL_Event e; SDL_PollEvent(&e); ) {
            handleEvent(e);
        }

        if (selection_) {
            view_.drawSelection(*selection_, board_, selectionLegalMoves_);
        }

        if (selection_ && mouse_.down) {
            view_.drawPieces(board_, selection_);
            view_.drawPieceAtCoord(board_, *selection_, mouse_.coord);
        } else {
            view_.drawPieces(board_);
        }

        view_.endFrame();
    }
}

bool Game::attemptMakeMove(chess::Tile tile) {
    if (std::find(selectionLegalMoves_.begin(), selectionLegalMoves_.end(), tile) != selectionLegalMoves_.end()) {
        board_.makeMove(*selection_, tile);
        return true;
    }
    return false;
}

void Game::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            running_ = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                chess::Tile tile = chess::tileOf({e.button.x, e.button.y});
                handleClick(tile);
                mouse_.coord = {e.button.x, e.button.y};
                mouse_.down = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                chess::Tile tile = chess::tileOf({e.button.x, e.button.y});
                handleRelease(tile);
                mouse_.down = false;
            }
            break;

        case SDL_MOUSEMOTION:
            if (mouse_.down) {
                SDL_GetMouseState(&mouse_.coord.x, &mouse_.coord.y);
            }
            break;
    }
}

void Game::handleClick(chess::Tile tile) {
    // Clicked same square. Do nothing
    if (selection_ && *selection_ == tile)
        return;

    // Moved piece. Deselect
    if (selection_ && attemptMakeMove(tile)) {
        selection_.reset();
        return;
    }

    // Tile is empty. Deselect
    if (board_.isEmptyAt(tile)) {
        selection_.reset();
        return;
    }

    // Make selection. Generate moves
    selection_ = tile;
    moveGen_.genLegalMovesAtTile(*selection_, board_, selectionLegalMoves_);
}

void Game::handleRelease(chess::Tile tile) {
    bool draggingPiece = selection_ && mouse_.down;

    // Piece was released on same tile. Do nothing
    if (draggingPiece && *selection_ == tile) {
        return;
    }

    // Piece was released and moved. Deselect
    if (draggingPiece && attemptMakeMove(tile)) {
        selection_.reset();
        return;
    }
}
