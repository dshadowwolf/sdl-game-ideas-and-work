#ifndef __RECT_HPP__

#include <SDL2/SDL.h>
#include <string>
#include "generic_exception.hpp"

class Rect {
    SDL_Rect rect;
    uint32_t screen_width, screen_height;

    public:
    Rect(uint32_t s_width, uint32_t s_height);

    void setW(uint32_t w);
    void setH(uint32_t h);
    void setX(uint32_t x);
    void setY(uint32_t y);
    uint32_t getW();
    uint32_t getH();
    uint32_t getX();
    uint32_t getY();

    void balanceHW(uint32_t w, uint32_t h);
    void scaleSize(uint32_t w, uint32_t h, uint32_t w_scale, uint32_t h_scale, uint32_t x_scale, uint32_t y_scale);
    SDL_Rect getRect();
};

#define __RECT_HPP__
#endif
