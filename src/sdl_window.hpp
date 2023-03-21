#ifndef __SDL_WINDOW_HPP__
#include <SDL2/SDL.h>
#include <string>
#include "renderer.hpp"

class SDLWindow {
    private:
    SDL_Window* window;
    std::string title;
    int width;
    int height;
    uint32_t flags;
    bool centered;

    public:
    SDLWindow(std::string title_ = "I Am B0rken", int width_ = 640, int height_ = 480,  uint32_t flags_ = SDL_WINDOW_SHOWN, bool centered_ = true);
    inline SDLWindow(SDLWindow &other) {
        title = other.title;
        width = other.width;
        height = other.height;
        flags = other.flags;
        centered = other.centered;
        window = other.window;
    };
    
    ~SDLWindow();
    SDL_Window* getWindow();
    void updateSurface();
    Renderer* getRenderer(uint32_t flags);
};

#define __SDL_WINDOW_HPP__
#endif
