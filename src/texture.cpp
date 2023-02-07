#include <iostream>
#include "texture.hpp"
#include "generic_exception.hpp"

Texture::Texture(SDL_Renderer *renderer, std::string path) {
    mRenderer = renderer;
    tex = IMG_LoadTexture(renderer, path.c_str());

    if ( tex == NULL ) {
        std::cerr << "SDL2_image LoadTexture failed: " << IMG_GetError() << std::endl;
        throw new GenericException(IMG_GetError());
    }
}

Texture::Texture(SDL_Renderer *renderer) {
    mRenderer = renderer;
}

Texture::~Texture() {
    SDL_DestroyTexture(tex);
}

SDL_Texture *Texture::getTexture() {
    return tex;
}

void Texture::query(uint32_t *format, int *access, int *w, int *h) {
    if (!tex) {
        std::cerr << "Texture::query() called on unloaded texture!" << std::endl;
        throw new GenericException("Texture Not Loaded/Initialized");
    }

    SDL_QueryTexture(tex, format, access, w, h);
}