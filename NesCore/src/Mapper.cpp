#include "Mapper.h"

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
{
	nPRGBanks = prgBanks;
	nCHRBanks = chrBanks;
}

Mapper::~Mapper()
{
}

bool Mapper::cpuMapRead(uint16_t address, uint32_t& mapped_address)
{
	return false;
}

bool Mapper::cpuMapWrite(uint16_t address, uint32_t& mapped_address)
{
	return false;
}

bool Mapper::ppuMapRead(uint16_t address, uint32_t& mapped_address)
{
	return false;
}

bool Mapper::ppuMapWrite(uint16_t address, uint32_t& mapped_address)
{
	return false;
}
