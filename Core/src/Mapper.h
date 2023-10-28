#pragma once
#include<cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

public:
	// -----takes input address from cpu or ppu bus and transform it into an address through which we can access ROM's 
	// Transform CPU bus address into PRG ROM offset, if address is sucessfully mapped, it returns true
	virtual bool cpuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
	virtual bool cpuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;
	// Transform PPU bus address into CHR ROM offset
	virtual bool ppuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
	virtual bool ppuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;

	// Virtual functions because mapper is an abstract base class.

protected:
	uint8_t nPRGBanks = 0;// number of program rom banks, which are passed into the constructor 
	uint8_t nCHRBanks = 0; // number of character rom banks, which are passed into the constructor

};

