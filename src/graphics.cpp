#include "graphics.hpp"
#include "generic_exception.hpp"
#include <iostream>

Graphics::Graphics() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL2 Initialization Failed: " << SDL_GetError() << std::endl;
        throw new GenericException(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG|IMG_INIT_JPG))) {
        std::cerr << "SDL2_Image Initialization Failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        throw new GenericException(SDL_GetError());
    }

    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {\
        std::cerr << "Compositor Bypass Failure: " << SDL_GetError() << std::endl;
    }

    isInit = true;
}

Graphics::~Graphics() {
    if ( isInit ) {
        IMG_Quit();
        SDL_Quit();
    }
}


