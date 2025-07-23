#include <game.h>
#include <SDL.h>
#include <iostream>

void Game::run() {
    while(running_) {
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            handleEvent(e);
        }
        view_.beginFrame();
        view_.drawBoard();

        if (selection_) {
            view_.drawSelection(boardPtr_, *selection_, selectionLegalMoves_);
        }

        if (selection_ && mouse_.down) {
            view_.drawPieces(boardPtr_, selection_);
            view_.drawPieceAtCoord(boardPtr_, *selection_, mouse_.coord);
        } else {
            view_.drawPieces(boardPtr_);
        }

        view_.drawEvalBar(boardPtr_->getWEval(), boardPtr_->getBEval());

        view_.endFrame();
    }
}

bool Game::attemptMakeMove(chess::Tile tile) {
    for (auto move : selectionLegalMoves_) {
        if (move.getToTile() == tile) {
            boardPtr_->makeMove(move);
            // std::cout << moveGen_.kingIsChecked() << "\n";
            return true;
        }
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
        if (boardPtr_->isEmptyAt(tile)) {
        selection_.reset();
        return;
    }

    // Make selection. Generate moves
    selection_ = tile;
    selectionLegalMoves_ = moveGen_.generateLegalMovesOf(*selection_);
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
