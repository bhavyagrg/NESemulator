#include "Pixel.h"

namespace Gui {
    Pixel::Pixel(int x, int y, Color color) : mX(x), mY(y), mColor(color) {
    }

    void Pixel::setColor(Color color) {
        mColor = color;
    }

    void Pixel::draw(int scale) {
        DrawRectangle(mX * scale, mY * scale, scale, scale, mColor);
    }


    void Pixel::draw(int x, int y, int scale) {
        DrawRectangle(x + (mX * scale), y + (mY * scale), scale, scale, mColor);
    }
}
