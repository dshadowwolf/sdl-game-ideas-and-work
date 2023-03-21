#include <iostream>
#include "sdl_window.hpp"
#include "generic_exception.hpp"
SDLWindow::SDLWindow(std::string title_, int width_, int height_, uint32_t flags_, bool centered_)
    : title(title_), width(width_), height(height_), flags(flags_), centered(centered_) {
    if (centered)
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    else
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    
    if (window == NULL) {
        std::string err = SDL_GetError();
        std::cerr << "SDL2 Failed to create a window: " << err << std::endl;
        throw GenericException(err);
    }
}

SDLWindow::~SDLWindow() {
    if (window != NULL) SDL_DestroyWindow(window);
}

SDL_Window* SDLWindow::getWindow() {
    return window;
}

void SDLWindow::updateSurface() {
    SDL_UpdateWindowSurface(window);
}

Renderer* SDLWindow::getRenderer(uint32_t flags) {
    return new Renderer(window, flags);
}