#ifndef __TERMINAL_HPP__

#include "panel.hpp"
#include "eventmarshaller.hpp"

class Terminal : protected Panel {
    std::string currentInput = "";
    std::string thePrompt;
    bool needsPrompt = true;
    bool appendCurrent = false;

    public:
    Terminal(std::string prompt, Renderer *renderer, uint32_t xLoc, uint32_t yLoc, uint32_t width, uint32_t height, bool hasBorder, uint8_t borderColor[4], uint8_t textColor[4], std::string fontName)
        : Panel(renderer, xLoc, yLoc, width, height, hasBorder, (uint8_t *) borderColor, (uint8_t *) textColor, fontName ),
                thePrompt(prompt)
    {
        Panel::setFontSize(15);

        // register event stuff for doing echo and input stuff
        EventMarshaller *events = *(EventMarshaller::getInstance());
        events->registerEventHandler(SDL_KEYDOWN, [&, this](SDL_Event *event) -> bool {
            const auto modifiers = event->key.keysym.mod;
            const auto key = event->key.keysym.sym;
            if ((((key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_a && key <= SDLK_z)) || 
                   key == SDLK_SPACE) && (modifiers == KMOD_NONE || (modifiers & (KMOD_SHIFT | KMOD_CTRL | KMOD_ALT | KMOD_NUM | KMOD_CAPS)))) {
                if (modifiers & (KMOD_SHIFT | KMOD_CAPS)) {
                    if (!(modifiers & (KMOD_CTRL | KMOD_ALT))) {
                        char nc = (key != SDLK_SPACE)?key-32:' ';
                        currentInput += nc;
                        mDisplayText += nc;
                        needsPrompt = false;
                    } else {
                        appendData("CTRL and ALT modifiers not handled currently for non-exit input\n");
                        needsPrompt = true;
                        appendCurrent = true;
                    }
                } else {
                    if (!(modifiers & (KMOD_CTRL | KMOD_ALT))) {
                        char nc = (key != SDLK_SPACE)?key:' ';
                        currentInput += nc;
                        mDisplayText += nc;
                        needsPrompt = false;
                    } else {
                        appendData("CTRL and ALT modifiers not handled currently for non-exit input\n");
                        needsPrompt = true;
                        appendCurrent = true;
                    }
                }
            } else if (key == SDLK_BACKSPACE) {
                mDisplayText = mDisplayText.substr(0, mDisplayText.length() - 1);
            } else if (key == SDLK_RETURN) {
                mDisplayText += "\n";
                mDisplayText += "Final Command: \"";
                mDisplayText += currentInput;
                mDisplayText += "\"\n";
                currentInput.clear();
                needsPrompt = true;
            }
            return true;
        });
    }

    void draw(Rect *target) {
        if (mBorder) (*mRect)->drawBorder(*mRenderer, mBorderColor[0], mBorderColor[1], mBorderColor[2], mBorderColor[3]);
        if (currentInput.empty() && needsPrompt) {
            mDisplayText.append(thePrompt);
            if (appendCurrent)
                mDisplayText.append(currentInput);
            needsPrompt = false;
            appendCurrent = false;
        }

        if (SDL_GetTicks() % 10) 
            if (mDisplayText[mDisplayText.length()-1] == '_') mDisplayText = mDisplayText.substr(0, mDisplayText.length() - 1);
            else mDisplayText.append("_");

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
    // we do not need these two
    void drawWithText(Rect * target, std::string text) = delete;
    void drawWithTextAndSize(Rect *target, std::string text, uint8_t fontSize) = delete;
};

#define __TERMINAL_HPP__
#endif
