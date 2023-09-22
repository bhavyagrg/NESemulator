#include "PPU.h"
#include "Cartridge.h"

uint8_t PPU::cpuRead(uint16_t address, bool rdonly)
{
    uint8_t data = 0x00;

    switch (address)
    {
    case 0x0000: //------ Control
        break;
    case 0x0001: //------ Mask
        break;
    case 0x0002: //------ Status
        break;
    case 0x0003: //------ OAM Address
        break;
    case 0x0004: //------ OAM Data
        break;
    case 0x0005: //------ Scroll
        break;
    case 0x0006: //------ PPU Address
        break;
    case 0x0007: //------ PPU Data
        break;
    }

    return data;
}

void PPU::cpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x0000: //------ Control
        break;
    case 0x0001: //------ Mask
        break;
    case 0x0002: //------ Status
        break;
    case 0x0003: //------ OAM Address
        break;
    case 0x0004: //------ OAM Data
        break;
    case 0x0005: //------ Scroll
        break;
    case 0x0006: //------ PPU Address
        break;
    case 0x0007: //------ PPU Data
        break;
    }
}

uint8_t PPU::ppuRead(uint16_t address, bool rdonly)
{
    // Placeholders
    uint8_t data = 0x00;
    address &= 0x3FFF;

    if (cart->ppuRead(address, data))
    {

    }

    return data;
}

void PPU::ppuWrite(uint16_t address, uint8_t data)
{
    // Placeholders
    address &= 0x3FFF;

    if (cart->ppuWrite(address, data))
    {

    }
}

void PPU::ConnectCartrdige(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cart = cartridge;
}

void PPU::clock()
{
}
