#include "Gui.h"

namespace Gui {
    Font font;
    void DrawText(std::string text, float x, float y) {
        DrawTextEx(font, text.c_str(), { x, y }, 20, 1, WHITE);
    }

    void DrawText(std::string text, float x, float y, Color color) {
        DrawTextEx(font, text.c_str(), { x, y }, 20, 1, color);
    }


    void DrawText(float x, float y, std::string text, Color color) {
        DrawTextEx(font, text.c_str(), { x, y }, 20, 1, color);
    }

    void DrawText(float x, float y, std::string text) {
        DrawTextEx(font, text.c_str(), { x, y }, 20, 1, WHITE);
    }


    void Window::Init(int width, int height) {

        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(width, height, "Nes Emulator");

        font = LoadFontEx("assets/fonts/JetBrainsMono/JetBrainsMono-Regular.ttf", 20, 0, NULL);

        SetTargetFPS(60);
    }
}