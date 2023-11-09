#pragma once

#include <cstdint>
#include<array>
#include<memory>
#include "CPU.h"
#include"PPU.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

	//Devices connected to the bus
	CPU cpu; // --->> CPU 
	PPU ppu;// ---->> PPU
	std::array<uint8_t, 2048> cpuRam; // -->> RAM
	std::shared_ptr<Cartridge> cart; // --->> The Cartridge

	uint8_t controller[2];

	// Read and write for the Bus
	void cpuWrite(uint16_t address, uint8_t data);
	uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

public:
	// --------------------System Interface-----------------
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	// --> A count of how many clocks have passed, how many times clock function has been called
	uint32_t nSystemClockCounter = 0;

	uint8_t controller_state[2];

	uint8_t dma_page = 0x00;
	uint8_t dma_addr = 0x00;
	uint8_t dma_data = 0x00;

	bool dma_transfer = false;
	bool dma_dummy = true;
};

