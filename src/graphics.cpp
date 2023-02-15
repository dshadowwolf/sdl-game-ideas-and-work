#include "graphics.hpp"
#include "generic_exception.hpp"
#include <iostream>

Graphics::Graphics() {
    std::string err;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        err = SDL_GetError();
        std::cerr << "SDL2 Initialization Failed: " << err << std::endl;
        throw new GenericException(err);
    }

    if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG|IMG_INIT_JPG))) {
        err = IMG_GetError();
        std::cerr << "SDL2_Image Initialization Failed: " << err << std::endl;
        SDL_Quit();
        throw new GenericException(err);
    }

    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
        err = SDL_GetError();
        std::cerr << "Compositor Bypass Failure: " << err << std::endl;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear")) {
        err = SDL_GetError();
        std::cerr << "Render Scale Quality Failure: " << err << std::endl;
    }

    isInit = true;
}

Graphics::~Graphics() {
    if ( isInit ) {
        IMG_Quit();
        SDL_Quit();
    }
}


