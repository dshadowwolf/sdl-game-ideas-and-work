#include <iostream>
#include "sdl_window.hpp"
#include "generic_exception.hpp"
SDLWindow::SDLWindow(std::string title, int width, int height, uint32_t flags, bool centered) {
    if (centered)
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    else
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

    if (window == NULL) {
        std::cerr << "SDL2 Failed to create a window: " << SDL_GetError() << std::endl;
        throw GenericException(SDL_GetError());
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