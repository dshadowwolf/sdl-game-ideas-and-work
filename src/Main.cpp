// compile with: clang++ main.cpp -o image_exmple -lSDL2 -lSDL2_image
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <thread>

#include "sdl_window.hpp"
#include "graphics.hpp"
#include "rect.hpp"
#include "eventmarshaller.hpp"
#include "generic_exception.hpp"
#include "panel.hpp"
#include "terminal.hpp"
#include "interp.hpp"
#include "program_status.hpp"
#include "graphics_threader.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(int argc, char* args[]) {
    SDL_Surface *screenSurface = NULL;

    EventMarshaller* events = *(EventMarshaller::getInstance());

    events->registerEventHandler(SDL_QUIT, [](SDL_Event *event) mutable -> bool { GameThingStatus::quit = true; return true; });
    events->registerEventHandler(SDL_WINDOWEVENT, [](SDL_Event *event) mutable -> bool { if (event->window.event == SDL_WINDOWEVENT_CLOSE) GameThingStatus::quit = true; return true; });
    events->registerEventHandler(SDL_KEYDOWN, [](SDL_Event *e) mutable -> bool { 
        const auto ks = e->key.keysym.sym; 
        const auto modifiers = e->key.keysym.mod;
        if (ks == SDLK_ESCAPE &&
            (modifiers & (KMOD_RCTRL|KMOD_RSHIFT|KMOD_RALT))) 
                GameThingStatus::quit = true; 
        return true; 
    });

    int m, s;
    int zz = openpty(&m, &s, NULL, NULL, NULL);

    TTF_Init();
    try {
        GameThingStatus::graphics = new Graphics();
        GameThingStatus::window = new SDLWindow("SDL Testing Project", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, true);
        GameThingStatus::renderer = GameThingStatus::window->getRenderer(SDL_RENDERER_ACCELERATED);
        GameThingStatus::runner = new GraphicsThreadRunner(m);
        GameThingStatus::runner->init();
    } catch(GenericException e) {
        std::cerr << "Exception during startup: " << e.what() << std::endl;
        return -1;
    }

    GameThingStatus::dumpStatus();
    std::thread graphics([] { GameThingStatus::runner->renderLoop(); });
    GameThingStatus::runner->postMessage("READY\n");

    Interpreter interp(s);
    std::thread interpreter([&interp] { interp.run_loop(); });

    write(m, "\n", 1); // try to trigger the interpreter to actually do something!

    while(!GameThingStatus::quit) {
        SDL_Event e;

        while(SDL_PollEvent(&e) > 0) {
            events->runEvents(&e);
            GameThingStatus::runner->getTerminal()->processEvent(e);
        }

    }

    interpreter.join();
    SDL_Delay(2000); // temporary
    graphics.join();
    delete GameThingStatus::renderer;
    delete GameThingStatus::window;
    delete GameThingStatus::graphics;
    return 0;
}
