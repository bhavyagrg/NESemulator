#pragma once

#include <cwchar>
#include <iostream>
#include <sstream>

#include "Core.h"


class Application {
public:
    Application();
    void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
    void DrawCpu(int x, int y);
    void DrawCode(int x, int y, int nLines);
    bool OnUserCreate();
    bool OnUserUpdate(float fElapsedTime);
    void Start();
    void DrawGame();
    void HandleInput();

public:
    std::shared_ptr<Cartridge> cart;
    Bus bus;
    std::map<uint16_t, std::string> mapAsm;
    bool bEmulationRun = false;
    float fResidualTime = 0.0f;

private:
    std::string hex(uint32_t n, uint8_t d);
    bool bIsFullScreen = false;
};
