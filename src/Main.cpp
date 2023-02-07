// compile with: clang++ main.cpp -o image_exmple -lSDL2 -lSDL2_image
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "sdl_window.hpp"
#include "graphics.hpp"
#include "rect.hpp"
#include "eventmarshaller.hpp"
#include "generic_exception.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(int argc, char* args[]) {
    bool quit = false;
    Renderer *renderer;
    Graphics *g;
    SDLWindow *wind;
    SDL_Surface *screenSurface = NULL;
    Texture *img;
    Rect imgRect(SCREEN_WIDTH, SCREEN_HEIGHT);
    Rect fillSquare(SCREEN_WIDTH, SCREEN_HEIGHT);
    const int borderSize = 5;
    EventMarshaller* events = *(EventMarshaller::getInstance());
    events->registerEventHandler(SDL_QUIT, [&quit](SDL_Event *event) mutable -> bool { quit = true; return true; });
    events->registerEventHandler(SDL_WINDOWEVENT, [&quit](SDL_Event *event) mutable -> bool { if (event->window.event == SDL_WINDOWEVENT_CLOSE) quit = true; return true; });
    events->registerEventHandler(SDL_KEYDOWN, [&quit](SDL_Event *e) mutable -> bool { auto ks = e->key.keysym.sym; if (ks == SDLK_q || ks == SDLK_ESCAPE) quit = true; return true; });

    try {
        g = new Graphics();
        wind = new SDLWindow("SDL Testing Project", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, true);
        renderer = wind->getRenderer(SDL_RENDERER_ACCELERATED);
        img = renderer->loadImage("/home/madman/projects/sdl-game-thing/photo_2021-11-08_22-23-35.jpg");
    } catch(GenericException e) {
        std::cerr << "Exception during startup: " << e.what() << std::endl;
        return -1;
    }

    int w, h;
    img->query(NULL, NULL, &w, &h);

    imgRect.balanceHW(w, h);

    fillSquare.setX(imgRect.getX());
    fillSquare.setY(imgRect.getY());
    uint32_t adjustHW = borderSize * 2;
    uint32_t adjustXY = borderSize;
    fillSquare.scaleSize(imgRect.getW(), imgRect.getH(), (uint32_t) borderSize * 2, (uint32_t) borderSize * 2, (uint32_t) borderSize, (uint32_t) borderSize);

    while(!quit) {
        SDL_Event e;

        while(SDL_PollEvent(&e) > 0) {
            events->runEvents(&e);
        }

        renderer->setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
        renderer->clear();
        renderer->setDrawColor(0x00, 0x00, 0x00, 0xFF);
        renderer->fillRect(fillSquare);
        renderer->copy(img, NULL, &imgRect);
        renderer->present();
        wind->updateSurface();
    }

    delete img;
    delete renderer;
    delete wind;
    delete g;
    std::cout << "Pausing for 10 seconds before `return 0;`" << std::endl;
    std::cout.flush();
    SDL_Delay(10000);
    return 0;
}
