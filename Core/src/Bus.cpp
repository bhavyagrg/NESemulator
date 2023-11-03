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
		// System RAM Address Range. The range covers 8KB, though
		// there is only 2KB available. That 2KB is "mirrored"
		// through this address range. Using bitwise AND to mask
		// the bottom 11 bits is the same as addr % 2048.
		cpuRam[address & 0x07FF] = data;

	// PPU 
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		// PPU Address range. The PPU only has 8 primary registers
		// and these are repeated throughout this range. We can
		// use bitwise AND operation to mask the bottom 3 bits, 
		// which is the equivalent of addr % 8.
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
		// System RAM Address Range, mirrored every 2048
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
	cart->reset();
	cpu.reset();
	ppu.reset();
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

	// The PPU is capable of emitting an interrupt to indicate the
	// vertical blanking period has been entered. If it has, we need
	// to send that irq to the CPU.
	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nonMaskableInterruptRequestSiq();
	}
	nSystemClockCounter++;
}
