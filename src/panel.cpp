#include "panel.hpp"

/*

class Panel {
    std::shared_ptr<Rect> mRect;
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<SDL_Surface*> mSurface;
    std::string mFontName;
    uint8_t mBorderColor[4];
    uint8_t mFontColor[4];
    uint32_t x, y, w, h;
    bool mBorder = false;
    std::string mDisplayText;
    std::string mFontName;
    std::string mFontPath;
    uint8_t mFontSize = 12;
    std::shared_ptr<TTF_Font *> mFont;
    public:
    Panel(uint32_t xLoc, uint32_t yLoc, uint32_t width, uint32_t height, bool hasBorder, uint8_t borderColor[4], uint8_t textColor[4], std::string fontName);
    ~Panel() = default;

    void draw();
    void drawWithText(std::string text);
    void drawWithTextAndSize(std::string text, uint8_t fontSize);
    std::shared_ptr<SDL_Surface*> getSurface();
};

*/

Panel::Panel(Renderer *renderer, uint32_t xLoc, uint32_t yLoc, uint32_t width, uint32_t height, bool hasBorder, uint8_t borderColor[4], uint8_t textColor[4], std::string fontName)
    : x(xLoc), y(yLoc), w(width), h(height), mBorder(hasBorder), mBorderColor(borderColor), mFontColor(textColor), mFontName(fontName)
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

void Panel::loadFont() {
    TTF_Font *newFont = TTF_OpenFont(mFontPath.c_str(), mFontSize);
    if (!newFont) {
        std::string exceptionText(TTF_GetError());
        std::cerr << "Error opening font " << mFontName << " at size " << mFontSize << " : " << exceptionText << std::endl;
        throw new GenericException(exceptionText);
    }

    mFont = std::make_shared<TTF_Font *>(newFont);
}

void Panel::draw() {
    if (mBorder) (*mRect)->drawBorder(*mRenderer, mBorderColor[0], mBorderColor[1], mBorderColor[2], mBorderColor[3]);
    if (mDisplayText.length() > 0) {
        // draw text here
    }
}

void Panel::drawWithText(std::string text) {
    mDisplayText = text;
    draw();
}

void Panel::drawWithTextAndSize(std::string text, uint8_t fontSize) {
    mFontSize = fontSize;
    loadFont();
    drawWithText(text);
}

std::shared_ptr<SDL_Surface *> Panel::getSurface() {
    return mSurface;
}