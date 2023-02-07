#ifndef __RENDERER_HPP__

#include <SDL2/SDL.h>
#include <string>

#include "texture.hpp"
#include "rect.hpp"

class Renderer {
    SDL_Renderer *renderer;

    public:
    Renderer(SDL_Window *p, uint32_t flags);
    ~Renderer();

    Texture *loadImage(std::string filepath);
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void fillRect(Rect &rect);
    void copy(Texture *texture, Rect *src, Rect *dest);
    void present();
};

#define __RENDERER_HPP__
#endif
