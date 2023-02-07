#ifndef __GRAPHICS_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "sdl_window.hpp"

class Graphics {
    bool isInit = false;
    SDL_Renderer *render;

    public:
    Graphics();
    ~Graphics();
};

#define __GRAPHICS_HPP__
#endif