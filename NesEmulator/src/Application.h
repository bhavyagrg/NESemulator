#pragma once

#include <iostream>
#include <sstream>

#include "Bus.h"
#include "CPU.h"

#define		OLC_PGE_APPLICATION
#include "external\PixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "external\PGEX_Sound.h"


class Application : public olc::PixelGameEngine {
public:
	Application();
	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
	void DrawCpu(int x, int y);
	void DrawCode(int x, int y, int nLines);
	bool OnUserCreate();
	bool EmulatorUpdateWithoutAudio(float fElapsedTime);
	bool EmulatorUpdateWithAudio(float fElapsedTime);
	bool OnUserUpdate(float fElapsedTime);


public:
	std::shared_ptr<Cartridge> cart; // pointer to cartridge object
	Bus bus;// object of type bus which now represents our nes
	std::map<uint16_t, std::string> mapAsm;
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;

	uint8_t nSelectedPalette = 0x00;

private:
	std::string hex(uint32_t n, uint8_t d);
};


static Demo_olcNES* pInstance; // static variable that will hold a pointer to "this" of derived pixelgameengine derived class