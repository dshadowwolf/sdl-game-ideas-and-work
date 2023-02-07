#ifndef __PANEL_HPP__

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>
#include <iostream>

#include "rect.hpp"
#include "renderer.hpp"
#include "generic_exception.hpp"

class Panel {
    std::shared_ptr<Rect *> mRect;
    std::shared_ptr<Renderer *> mRenderer;
    std::shared_ptr<SDL_Surface*> mSurface;
    std::string mFontName;
    uint8_t mBorderColor[4];
    uint8_t mFontColor[4];
    uint32_t x, y, w, h;
    bool mBorder = false;
    std::string mDisplayText;
    std::string mFontPath;
    uint8_t mFontSize = 12;
    std::shared_ptr<TTF_Font *> mFont;
    void loadFont();

    public:
    Panel(Renderer *renderer, uint32_t xLoc, uint32_t yLoc, uint32_t width, uint32_t height, bool hasBorder, uint8_t borderColor[4], uint8_t textColor[4], std::string fontName);
    ~Panel();

    void draw(Rect *target);
    void drawWithText(Rect * target, std::string text);
    void drawWithTextAndSize(Rect *target, std::string text, uint8_t fontSize);
    std::shared_ptr<SDL_Surface*> getSurface();
    void appendData(std::string data);
};

#define __PANEL_HPP__
#endif
