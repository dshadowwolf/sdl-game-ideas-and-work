#include "rect.hpp"


Rect::Rect(uint32_t s_width, uint32_t s_height) {
    screen_width = s_width;
    screen_height = s_height;
}

void Rect::setW(uint32_t w) {
    rect.w = w;
}

void Rect::setH(uint32_t h) {
    rect.h = h;
}

void Rect::setX(uint32_t x) {
    rect.x = x;
}

void Rect::setY(uint32_t y) {
    rect.y = y;
}

uint32_t Rect::getW() {
    return rect.w;
}

uint32_t Rect::getH() {
    return rect.h;
}

uint32_t Rect::getX() {
    return rect.x;
}

uint32_t Rect::getY() {
    return rect.y;
}

void Rect::balanceHW(uint32_t w, uint32_t h) {
    if(w > h) {
        rect.w = screen_width * 0.6;
        rect.h = h * rect.w / w;
    } else {
        rect.h = screen_height * 0.6;
        rect.w = w * rect.h / h;
    }

    rect.x = screen_width / 2 - rect.w / 2;
    rect.y = screen_height / 2 - rect.h / 2;
}

void Rect::scaleSize(uint32_t w, uint32_t h, uint32_t w_scale, uint32_t h_scale, uint32_t x_scale, uint32_t y_scale) {
    rect.w = w + w_scale;
    rect.h = h + h_scale;
    rect.x -= x_scale;
    rect.y -= y_scale;
}

SDL_Rect Rect::getRect() {
    return rect;
}