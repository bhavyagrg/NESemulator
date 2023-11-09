#pragma once
#include "raylib.h"


namespace Gui {
    class Pixel {
    public:
        Pixel() = default;
        Pixel(int x, int y, Color color);
        void setColor(Color color);
        void draw(int scale = 1);
        void draw(int x, int y, int scale = 1);
    private:
        int mX = 0, mY = 0;
        Color mColor = BLACK;
    };
}