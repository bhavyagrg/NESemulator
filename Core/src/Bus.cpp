#include "Bus.h"

Bus::Bus()
{
	// Clear RAM contents
	for (auto& i : cpuRam) i = 0x00;

	// Connect CPU for communication with bus
	cpu.ConnectBus(this);
}

Bus::~Bus()
{

}

void Bus::cpuWrite(uint16_t address, uint8_t data)
{
	// --- priority given to cartridge for read and write operations over cpu or ppu, if cartridge read or write funtion returns true then that will be taken over by the cartridge--------
	if (cart->cpuWrite(address, data))
	{

	}

	else if (address >= 0x0000 && address <= 0x1FFF)
		cpuRam[address & 0x07FF] = data;

	// PPU 
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		// mirroring to 8 entries that we need
		ppu.cpuWrite(address & 0x0007, data);
	}

}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly)
{
	uint8_t data = 0x00;

	if (cart->cpuRead(address, data))
	{
		// Cartridge Address Range
	}

	else if (address >= 0x0000 && address <= 0x1FFF)
		data = cpuRam[address & 0x07FF];

	// PPU 
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		// mirroring to 8 entries that we need
		data = ppu.cpuRead(address & 0x0007, bReadOnly);
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	// Connect bus to the cartridge
	this->cart = cartridge;
	// Connect PPU to the cartridge 
	ppu.ConnectCartrdige(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();
	// cpu clock runs three times slower than the ppu clock
	if (nSystemClockCounter % 3 == 0)
	{
		cpu.clock();
	}

	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nonMaskableInterruptRequestSiq();
	}
	nSystemClockCounter++;
}
