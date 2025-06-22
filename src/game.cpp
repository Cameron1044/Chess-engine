#include <game.h>

void Game::run() {
    while(running_m) {
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            handleEvent(e);
        }

        view_m.draw(board_m);
    }
}

void Game::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            running_m = false;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                view_m.select.x = e.button.x/100;
                view_m.select.y = e.button.y/100;
                if (view_m.select.selected) {
                    view_m.select.selected = true;
                } else {
                    view_m.select.selected = true;
                }
            }
    }
}