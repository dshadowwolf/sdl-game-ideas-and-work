#ifndef __TEXTURE_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Texture {
    SDL_Texture *tex;
    public:
    Texture(SDL_Renderer *renderer, std::string path);
    ~Texture();

    SDL_Texture *getTexture();
    void query(uint32_t *format, int *access, int *w, int *h);
};

#define __TEXTURE_HPP__
#endif
