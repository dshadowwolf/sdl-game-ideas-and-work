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
#include "panel.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(int argc, char* args[]) {
    bool quit = false;
    Renderer *renderer;
    Graphics *g;
    SDLWindow *wind;
    SDL_Surface *screenSurface = NULL;
    Panel *bottomContent;
    Rect topPane(SCREEN_WIDTH - 2, (SCREEN_HEIGHT*0.75) - 2), bottomPane(SCREEN_WIDTH - 2, (SCREEN_HEIGHT*0.25) - 2);
    Rect topDivLeft((SCREEN_WIDTH*0.25)-4, (SCREEN_HEIGHT*0.75) - 6), topDivRight((SCREEN_WIDTH*0.75)-4, (SCREEN_HEIGHT*0.75) - 6);
    std::cout << "width: " << SCREEN_WIDTH << " -- height: " << SCREEN_HEIGHT * 0.75 << std::endl;
    Rect screenRect(SCREEN_WIDTH, SCREEN_HEIGHT);
    int topX = 1, topY = 1, bottomY = (SCREEN_HEIGHT*0.75)+1, bottomX = 1;
    int topLeftX = 3, topLeftY = 3, topRightX = (SCREEN_WIDTH*0.25)+2, topRightY = 3;
    const int borderSize = 5;

    EventMarshaller* events = *(EventMarshaller::getInstance());

    events->registerEventHandler(SDL_QUIT, [&quit](SDL_Event *event) mutable -> bool { quit = true; return true; });
    events->registerEventHandler(SDL_WINDOWEVENT, [&quit](SDL_Event *event) mutable -> bool { if (event->window.event == SDL_WINDOWEVENT_CLOSE) quit = true; return true; });
    events->registerEventHandler(SDL_KEYDOWN, [&quit](SDL_Event *e) mutable -> bool { auto ks = e->key.keysym.sym; if (ks == SDLK_q || ks == SDLK_ESCAPE) quit = true; return true; });

    TTF_Init();
    try {
        g = new Graphics();
        wind = new SDLWindow("SDL Testing Project", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, true);
        renderer = wind->getRenderer(SDL_RENDERER_ACCELERATED);
        uint8_t color[4] = { 0xFF,0xFF,0xFF,0xFF};
        bottomContent = new Panel(renderer, bottomPane.getX()+5, bottomPane.getY()+5, bottomPane.getW() - 5, bottomPane.getH() - 5, false, color, color, "cp437");
    } catch(GenericException e) {
        std::cerr << "Exception during startup: " << e.what() << std::endl;
        return -1;
    }

    std::string panelText[4] = { "This is a test\nThis is only a test", "HELP! I'M TRAPPED IN THE INTERNET!", "Go Away, you bother me!", "Welcome to MS-DOS 2.0" };
    uint8_t which = 0;

    topPane.setX(topX);
    topPane.setY(topY);
    topDivLeft.setX(topLeftX);
    topDivRight.setX(topRightX);
    topDivLeft.setY(topLeftY);
    topDivRight.setY(topRightY);
    bottomPane.setX(bottomX);
    bottomPane.setY(bottomY);

    while(!quit) {
        SDL_Event e;

        while(SDL_PollEvent(&e) > 0) {
            events->runEvents(&e);
        }

        renderer->setDrawColor(0x00, 0x00, 0x00, 0xFF);
        renderer->clear();
        topPane.drawBorder(renderer, 0xFF, 0x7F, 0x7F, 0xFF);
        bottomPane.drawBorder(renderer, 0x7F, 0x7F, 0xFF, 0xFF);
        topDivLeft.drawBorder(renderer, 0x7F, 0xFF, 0x7F, 0xFF);
        topDivRight.drawBorder(renderer, 0xFF, 0x7F, 0xFF, 0xFF);
        bottomContent->drawWithText(&bottomPane, panelText[(which++)%4]);
        renderer->present();
        wind->updateSurface();
        SDL_Delay(1000);
    }

    delete renderer;
    delete wind;
    delete g;
    return 0;
}
