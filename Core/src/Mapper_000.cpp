#include "Mapper_000.h"


bool Mapper_000::cpuMapRead(uint16_t address, uint32_t& mapped_address)
{
    // Have to come from the cartrdge
    if (address >= 0x8000 && address <= 0xFFFF)
    {
        // ---- 32K ROM maps exactly half the range of 16 bit address hence 0x7FFF
        // ---- if we got only one bank of PRG ROM then it is a 16K ROM hence 0x3FFF
        mapped_address = address & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t address, uint32_t& mapped_address)
{
    // Have to come from the cartrdge
    if (address >= 0x8000 && address <= 0xFFFF)
    {
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapRead(uint16_t address, uint32_t& mapped_address)
{
    if (address >= 0x0000 && address <= 0x1FFF)
    {
        // No Bank Swicthing in case of PPU
        mapped_address = address;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t address, uint32_t& mapped_address)
{
    /*if (address >= 0x0000 && address <= 0x1FFF)
    {

        return true;
    }*/

    return false;
}
