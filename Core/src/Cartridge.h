#pragma once
#include <cstdint>
#include<vector>
#include <fstream>
#include<string>

#include "Mapper_000.h"

class Cartridge
{
public:
	Cartridge(const std::string& sFileName);// sFileName stores path of the nes file
	~Cartridge();

	bool ImageValid();

	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

private:
	bool bImageValid = false;
	std::vector<uint8_t> vPRGMemory; // memory for program rom
	std::vector<uint8_t> vCHRMemory; // memory for character rom

	uint8_t nMapperID = 0; // tells which mapper am i using
	uint8_t nPRGBanks = 0;// how many banks respective memory that are there
	uint8_t nCHRBanks = 0;

	std::shared_ptr<Mapper> pMapper; // ----- pointer to the mapper class

public:
	// --------------Connects PPU to CPU Bus------------------
	// -->> Communications  with Main Bus
	// Cartridge read and write functions returns boolean unlike others, this boolean will tell whether the cartridge is handling that read or write request or not
	bool cpuRead(uint16_t address, uint8_t& data);
	bool cpuWrite(uint16_t address, uint8_t data);

	// --------------Connects PPU to it's own Bus------------------
	// -->> Communications  with PPU Bus
	bool ppuRead(uint16_t address, uint8_t& data);
	bool ppuWrite(uint16_t address, uint8_t data);
};

