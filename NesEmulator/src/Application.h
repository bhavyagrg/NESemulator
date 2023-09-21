#pragma once

#include <iostream>
#include <sstream>

#include "Bus.h"
#include "CPU.h"

#include "external\PixelGameEngine.h"


class Application : public olc::PixelGameEngine {
public:
	Application();
	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
	void DrawCpu(int x, int y);
	void DrawCode(int x, int y, int nLines);
	bool OnUserCreate();
	bool OnUserUpdate(float fElapsedTime);


public:
	Bus nes;
	std::map<uint16_t, std::string> mapAsm;

private:
	std::string hex(uint32_t n, uint8_t d);
};