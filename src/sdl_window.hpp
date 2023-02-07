#ifndef __SDL_WINDOW_HPP__
#include <SDL2/SDL.h>
#include <string>
#include "renderer.hpp"

class SDLWindow {
    private:
    SDL_Window* window;

    public:
    SDLWindow(std::string title = "I Am B0rken", int width = 640, int height = 480,  uint32_t flags = SDL_WINDOW_SHOWN, bool centered = true);
    ~SDLWindow();
    SDL_Window* getWindow();
    void updateSurface();
    Renderer* getRenderer(uint32_t flags);
};

#define __SDL_WINDOW_HPP__
#endif
