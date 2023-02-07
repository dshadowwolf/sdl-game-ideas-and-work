#include <iostream>
#include <SDL2/SDL_image.h>
#include "renderer.hpp"
#include "generic_exception.hpp"

Renderer::Renderer(SDL_Window *p, uint32_t flags) {
    renderer = SDL_CreateRenderer(p, -1, flags);

    if (!renderer) {
        std::cerr << "SDL2 Unable to create Renderer for window: " << SDL_GetError() << std::endl;
        throw new GenericException(SDL_GetError());
    }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer);
}

Texture* Renderer::loadImage(std::string path) {
    return new Texture(renderer, path);
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}
void Renderer::fillRect(Rect &rect) {
    SDL_Rect r = rect.getRect();
    SDL_RenderFillRect(renderer, &r);
}

void Renderer::copy(Texture *texture, Rect *src, Rect *dest) {
    SDL_Rect *s, *d;
    SDL_Rect sr, dr;
    if (src == NULL) s = NULL;
    else {
        sr = src->getRect();
        s = &sr;
    }
    dr = dest->getRect();
    d = &dr;
    SDL_RenderCopy(renderer, texture->getTexture(), s, d);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}
