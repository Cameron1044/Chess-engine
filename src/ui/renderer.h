#pragma once
#include <SDL.h>

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

class Renderer {
    public:
        Renderer();
        ~Renderer();
        void startApp();
        
    private:
        void initSDL();
        void drawBoard();
        App app_;
};