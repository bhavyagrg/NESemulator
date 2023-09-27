#pragma once
#include <cstdint>
#include<memory>
#include "Cartridge.h"
#include "external\PixelGameEngine.h"
 
class PPU
{
public:
	PPU();
	~PPU();

// ------------------Memories--------------

private:
	uint8_t vRam[2][1024]; // ---->> Used to hold name table information(2KB)
	uint8_t palette_mem[32];
	uint8_t pattern_mem[2][4096]; // ---> not necessary

public:
	// --------------Connects PPU to CPU Bus------------------
	// -->> Communications  with Main Bus
	uint8_t cpuRead(uint16_t address, bool rdonly = false);
	void cpuWrite(uint16_t address, uint8_t data);

	// --------------Connects PPU to it's own Bus------------------
	// -->> Communications  with PPU Bus
	uint8_t ppuRead(uint16_t address, bool rdonly = false);
	void ppuWrite(uint16_t address, uint8_t data);


private:
	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	void ConnectCartrdige(const std::shared_ptr<Cartridge>& cartridge);
	void clock();

	// olc::Sprite sprScreen = olc::Sprite(256, 240);
	// olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	// olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

	// Changed To for API breaking subsequent PGE Update
private:
	olc::Pixel palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256,240),olc::Sprite(128,128) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128,128),olc::Sprite(128,128) };

public:
	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	int16_t scanline = 0; // which row on the screen
	int16_t cycle = 0; // which col on the screen

};

