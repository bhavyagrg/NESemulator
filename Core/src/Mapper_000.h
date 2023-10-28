#pragma once
#include "Mapper.h"

// Mapper_000 derived from abstract Mapper class
class Mapper_000 : public Mapper
{

public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);

	//------ Function of mapper is just to translate the address, it doesn't provide any address or data ------------
	bool cpuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool cpuMapWrite(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapWrite(uint16_t address, uint32_t& mapped_address) override;
};

