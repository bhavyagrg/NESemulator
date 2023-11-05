#pragma once
#include "raylib.h"
#include "Pixel.h"
#include <vector>

namespace Gui
{

    class Sprite
    {
    public:
        Sprite(int width, int height);
        Color color = BLACK;

        void setColor(Color color);
        void setPixel(int x, int y, Color &color);
        void draw(int x, int y, int scale = 1);

    private:
        int mWidth = 0, mHeight = 0;
        std::vector<Pixel> mPixels;
    };

}
