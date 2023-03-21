#ifndef __GRAPHICS_THREADER_HPP__

#include <iostream>

#include "graphics.hpp"
#include "rect.hpp"
#include "panel.hpp"
#include "terminal.hpp"
#include "program_status.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

class GraphicsThreadRunner {
    Rect topPane, bottomPane, topDivLeft, topDivRight, screenRect;
    int topX = 1, topY = 1, bottomY = (SCREEN_HEIGHT*0.75)+1, bottomX = 1;
    int topLeftX = 3, topLeftY = 3, topRightX = (SCREEN_WIDTH*0.25)+2, topRightY = 3;
    const int borderSize = 5;
    Terminal *bottomContent;
    Panel *topRightContent;
    uint8_t color[4] = { 0xFF,0xFF,0xFF,0xFF};
    int commfd;

public:
    GraphicsThreadRunner(int tfd) : commfd(tfd) {
            topPane = Rect(SCREEN_WIDTH - 2, (SCREEN_HEIGHT*0.75) - 2);
            bottomPane = Rect(SCREEN_WIDTH - 2, (SCREEN_HEIGHT*0.25) - 2);
            topDivLeft = Rect((SCREEN_WIDTH*0.25)-4, (SCREEN_HEIGHT*0.75) - 6);
            topDivRight = Rect((SCREEN_WIDTH*0.75)-4, (SCREEN_HEIGHT*0.75) - 6);
            screenRect = Rect(SCREEN_WIDTH, SCREEN_HEIGHT);

            topRightContent = new Panel(GameThingStatus::renderer, topDivRight.getX()+5, topDivRight.getY()+5, topDivRight.getW() - 5, topDivRight.getH() - 5, false, color, color, "terminus");
            bottomContent = new Terminal(tfd, 13, 160, TTF_OpenFont("/home/madman/projects/sdl-game-thing/resources/terminus.ttf", 12));
        }

    void init() {
        uint8_t which = 0, ow = 1;
        topRightContent->setFontSize(16);

        topPane.setX(topX);
        topPane.setY(topY);
        topDivLeft.setX(topLeftX);
        topDivRight.setX(topRightX);
        topDivLeft.setY(topLeftY);
        topDivRight.setY(topRightY);
        bottomPane.setX(bottomX);
        bottomPane.setY(bottomY);

    }

    void renderLoop() {
        while(!GameThingStatus::quit) {
            GameThingStatus::renderer->setDrawColor(0x00, 0x00, 0x00, 0xFF);
            GameThingStatus::renderer->clear();
            topPane.drawBorder(GameThingStatus::renderer, 0xFF, 0x7F, 0x7F, 0xFF);
            bottomPane.drawBorder(GameThingStatus::renderer, 0x7F, 0x7F, 0xFF, 0xFF);
            topDivLeft.drawBorder(GameThingStatus::renderer, 0x7F, 0xFF, 0x7F, 0xFF);
            topDivRight.drawBorder(GameThingStatus::renderer, 0xFF, 0x7F, 0xFF, 0xFF);
            bottomContent->processInput();
            bottomContent->render(GameThingStatus::renderer->getInternal(), bottomPane);
            topRightContent->draw(&topDivRight);
            GameThingStatus::renderer->present();
            GameThingStatus::window->updateSurface();
            SDL_Delay(1000);
        }
    }

    Terminal *getTerminal() {
        return bottomContent;
    }

    void postMessage(std::string msg) {
        topRightContent->appendData(msg);
    }
};

#define __GRAPHICS_THREADER_HPP__
#endif

