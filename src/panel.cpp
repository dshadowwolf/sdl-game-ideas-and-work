#include "panel.hpp"

Panel::Panel(Renderer *renderer, uint32_t xLoc, uint32_t yLoc, uint32_t width, uint32_t height, bool hasBorder, uint8_t borderColor[4], uint8_t textColor[4], std::string fontName)
    : x(xLoc), y(yLoc), w(width), h(height), mBorder(hasBorder), mBorderColor { borderColor[0], borderColor[1], borderColor[2], borderColor[3] }, mFontColor { textColor[0], textColor[1], textColor[2], textColor[3] }, mFontName(fontName)
{
    Rect *nRect = new Rect(w, h);
    nRect->setX(x);
    nRect->setY(y);
    mRenderer = std::make_shared<Renderer *>(renderer);
    mRect = std::make_shared<Rect *>(nRect);
    std::string mStr("/home/madman/projects/sdl-game-thing/resources/");
    mStr += mFontName;
    mStr += ".ttf";
    mFontSize = 12; // sane default ?
    mFontPath = mStr;
    loadFont();
    mSurface = std::make_shared<SDL_Surface *>(SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0));
}

Panel::~Panel() {
    if (mFont != nullptr) TTF_CloseFont(*mFont);
    SDL_FreeSurface(*mSurface);
}

void Panel::setFontSize(uint32_t fontSize) {
    mFontSize = fontSize;
    loadFont();
}

void Panel::loadFont() {
    TTF_Font *newFont = TTF_OpenFont(mFontPath.c_str(), mFontSize);
    if (!newFont) {
        std::string exceptionText(TTF_GetError());
        std::cerr << "Error opening font " << mFontName << " at size " << mFontSize << " : " << exceptionText << std::endl;
        throw new GenericException(exceptionText);
    }

    mFont = std::make_shared<TTF_Font *>(newFont);
}

void Panel::draw(Rect *target) {
    if (mBorder) (*mRect)->drawBorder(*mRenderer, mBorderColor[0], mBorderColor[1], mBorderColor[2], mBorderColor[3]);
    if (mDisplayText.length() > 0) {
        SDL_Color mColor;
        mColor.a = mFontColor[3];
        mColor.b = mFontColor[2];
        mColor.g = mFontColor[1];
        mColor.r = mFontColor[0];

        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped( *mFont, mDisplayText.c_str(), mColor, (*mRect)->getW() - 25);
        SDL_Rect srcRect = (*mRect)->getRect();
        Texture *texture = (*mRenderer)->textureFromSurface(textSurface);
        int w, h;
        uint32_t f = SDL_PIXELFORMAT_ABGR8888;
        texture->query(&f, NULL, &w, &h);
        if (h > srcRect.h) (*mRect)->setY(h - srcRect.h);
        else (*mRect)->setY(0);
        (*mRect)->setX(0);
        // preserve the original height and width of the target rectangle
        int orig_w = target->getW();
        int orig_h = target->getH();
        int orig_x = target->getX();
        int orig_y = target->getY();
        // change them to match what is needed for the text being displayed
        // this is so things don't get scaled
        target->setW(w);
        // lets have this capped, somewhat, so the scrolling works
        target->setH(h <= srcRect.h?h:srcRect.h);
        // make sure we render 5px inside the actual border of the box
        target->setX(target->getX()+5);
        target->setY(target->getY()+5);
        (*mRect)->setY((*mRect)->getY()+3);
        (*mRenderer)->copy(texture, (*mRect), target);
        (*mRect)->setY((*mRect)->getY()-3);
        // restore the target rect to its original state
        target->setW(orig_w);
        target->setH(orig_h);
        target->setX(orig_x);
        target->setY(orig_y);
    }
}

void Panel::drawWithText(Rect *target, std::string text) {
    mDisplayText = text;
    draw(target);
}

void Panel::drawWithTextAndSize(Rect *target, std::string text, uint8_t fontSize) {
    mFontSize = fontSize;
    loadFont();
    drawWithText(target, text);
}

std::shared_ptr<SDL_Surface *> Panel::getSurface() {
    return mSurface;
}

void Panel::appendData(std::string data) {
    if (!mDisplayText.empty() || mDisplayText[mDisplayText.length()-1] != '\n') {
        mDisplayText += "\n";
    }
    mDisplayText += data;
}