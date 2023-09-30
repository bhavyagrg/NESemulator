#pragma once
#include<cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

public:
	// -----takes input address from cpu or ppu bus and transform it into an address through which we can access ROM's 
	// Transform CPU bus address into PRG ROM offset
	virtual bool cpuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
	virtual bool cpuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;
	// Transform PPU bus address into CHR ROM offset
	virtual bool ppuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
	virtual bool ppuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;


protected:
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;

};

