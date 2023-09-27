#pragma once
#include "Mapper.h"
class Mapper_000 : public Mapper
{

public:
	//------ Function of mapper is just to translate the address, it doesn't provide any address or data ------------
	bool cpuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool cpuMapWrite(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapWrite(uint16_t address, uint32_t& mapped_address) override;
};

