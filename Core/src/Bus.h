#pragma once
#include <cstdint>
#include<array>
#include "CPU.h"

class Bus
{
public:
	Bus();
	~Bus();

	//Devices connected to the bus
	CPU cpu;

	// Fake RAM
	std::array<uint8_t, 64 * 1024> ram; 

	// Read and write for the Bus
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool bReadOnly = false);
};

