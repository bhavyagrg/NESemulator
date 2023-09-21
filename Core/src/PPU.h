#pragma once
#include <cstdint>
#include<memory>
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
};

