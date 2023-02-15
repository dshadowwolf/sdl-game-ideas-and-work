#include <iostream>
#include <SDL2/SDL_image.h>
#include "renderer.hpp"
#include "generic_exception.hpp"

Renderer::Renderer(SDL_Window *p, uint32_t flags) {
    renderer = SDL_CreateRenderer(p, -1, flags);

    if (!renderer) {
        std::string error(SDL_GetError());
        std::cerr << "SDL2 Unable to create Renderer for window: " << error << std::endl;
        throw new GenericException(error);
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

void Renderer::fillRect(Rect &rect, SDL_Color &color) {
    SDL_Color saved;
    SDL_GetRenderDrawColor(renderer, &saved.r, &saved.g, &saved.b, &saved.a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fillRect(rect);
    SDL_SetRenderDrawColor(renderer, saved.r, saved.g, saved.b, saved.a);
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

Texture *Renderer::getBlankTexture() {
    return new Texture(renderer);
}

void Renderer::drawRectOutline(SDL_Rect rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawRect(renderer, &rect);
}

Texture *Renderer::textureFromSurface(SDL_Surface *surface) {
    SDL_Texture *baseTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return new Texture(renderer, baseTexture);
}
