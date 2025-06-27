#pragma once

/*
    Smart-pointer SDL wrapper for SDLs C-style resources
    (window, renderer, texture, surface)
*/

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <filesystem>
#include <stdexcept>

namespace sdlw { // SDL wrapper

// Window deleting functor
struct SDLWindowDeleter {
    void operator()(SDL_Window* w) const noexcept {
        if (w) {SDL_DestroyWindow(w);} // Frees memory
    }
};

// Renderer deleting functor
struct SDLRendererDeleter {
    void operator()(SDL_Renderer* r) const noexcept {
        if (r) {SDL_DestroyRenderer(r);} // Frees memory
    }
};

// Texture deleting functor
struct SDLTextureDeleter {
    void operator()(SDL_Texture* t) const noexcept {
        if (t) {SDL_DestroyTexture(t);}
    }
};

// Surface deleting functor
struct SDLSurfaceDeleter {
    void operator()(SDL_Surface* s) const noexcept {
        SDL_FreeSurface(s); // Safe to pass nullptr
    }
};

// Aliases
using WindowPtr = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>;

[[nodiscard]] // Warns if pointer is not assigned on function call
inline TexturePtr loadTexture(SDL_Renderer* renderer, 
                               const std::filesystem::path& relPath, 
                               const std::filesystem::path& absPath = "assets") {
    std::filesystem::path fullPath = absPath / relPath;

    SurfacePtr surface{IMG_Load(fullPath.string().c_str())}; // Takes c style string
    if (!surface) {
        throw std::runtime_error("IMG_Load failure: " + std::string(IMG_GetError())); // IMG_GetError returns a c style string
    }

    TexturePtr texture{SDL_CreateTextureFromSurface(renderer, surface.get())}; // Takes raw pointers
    if (!surface) {
        throw std::runtime_error("SDL_CreateTextureFromSurface: " + std::string(SDL_GetError())); // SDL_GetError returns a c style string
    }

    return texture;
}

}