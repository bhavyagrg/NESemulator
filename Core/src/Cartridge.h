#pragma once
#include <cstdint>
#include<vector>
#include <fstream>
#include<string>

class Cartridge
{
public:
	Cartridge(const std::string& sFileName);
	~Cartridge();

private:
	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	uint8_t nMapperID = 0;
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;


	// --------------Connects PPU to CPU Bus------------------
	// -->> Communications  with Main Bus
	uint8_t cpuRead(uint16_t address, bool rdonly = false);
	void cpuWrite(uint16_t address, uint8_t data);

	// --------------Connects PPU to it's own Bus------------------
	// -->> Communications  with PPU Bus
	uint8_t ppuRead(uint16_t address, bool rdonly = false);
	void ppuWrite(uint16_t address, uint8_t data);
};

