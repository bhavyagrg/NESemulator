#pragma once

#include "raylib.h"

#include <string>

namespace Gui
{
    void DrawText(std::string text, float x, float y);
    void DrawText(float x, float y, std::string text);
    void DrawText(std::string text, float x, float y, Color color);
    void DrawText(float x, float y, std::string text, Color color);

    namespace Window {
        void Init(int width, int height);
    }
}

