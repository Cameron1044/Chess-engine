#include "ui/renderer.h"
#include <iostream>

Renderer::Renderer() {
    this->initSDL();
}

Renderer::~Renderer() {
    if (this->app_.renderer) SDL_DestroyRenderer(this->app_.renderer);
    if (this->app_.window) SDL_DestroyWindow(this->app_.window);
    SDL_Quit();
}

void Renderer::startApp() {
    bool running = true;
    while(running) {
        for (SDL_Event e; SDL_PollEvent(&e); )
            if (e.type == SDL_QUIT) running = false;

        SDL_RenderClear(this->app_.renderer);
        this->drawBoard();
        SDL_RenderPresent(this->app_.renderer);
    }
}

void Renderer::initSDL() {
    const int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    const int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    this->app_.window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100*8, 100*8, windowFlags);
    if (!this->app_.window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->app_.renderer = SDL_CreateRenderer(this->app_.window, -1, rendererFlags);
    if (!this->app_.renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Renderer::drawBoard() {
    const int tile_size = 100;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            bool dark = (x + y) % 2 != 0;
            SDL_SetRenderDrawColor(this->app_.renderer, 
                dark ? 118 : 238,
                dark ? 150 : 238,
                dark ? 86  : 210,
                255);
            SDL_Rect Rect{x*tile_size, y*tile_size, tile_size, tile_size};
            SDL_RenderFillRect(this->app_.renderer, &Rect);
        }
    }
}