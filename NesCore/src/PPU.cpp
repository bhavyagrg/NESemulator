#include "PPU.h"
#include "Cartridge.h"

PPU::PPU()
{
    palScreen[0x00] = CLITERAL(Color) { 84, 84, 84, 255 };
    palScreen[0x01] = CLITERAL(Color) { 0, 30, 116, 255 };
    palScreen[0x02] = CLITERAL(Color) { 8, 16, 144, 255 };
    palScreen[0x03] = CLITERAL(Color) { 48, 0, 136, 255 };
    palScreen[0x04] = CLITERAL(Color) { 68, 0, 100, 255 };
    palScreen[0x05] = CLITERAL(Color) { 92, 0, 48, 255 };
    palScreen[0x06] = CLITERAL(Color) { 84, 4, 0, 255 };
    palScreen[0x07] = CLITERAL(Color) { 60, 24, 0, 255 };
    palScreen[0x08] = CLITERAL(Color) { 32, 42, 0, 255 };
    palScreen[0x09] = CLITERAL(Color) { 8, 58, 0, 255 };
    palScreen[0x0A] = CLITERAL(Color) { 0, 64, 0, 255 };
    palScreen[0x0B] = CLITERAL(Color) { 0, 60, 0, 255 };
    palScreen[0x0C] = CLITERAL(Color) { 0, 50, 60, 255 };
    palScreen[0x0D] = CLITERAL(Color) { 0, 0, 0, 255 };
    palScreen[0x0E] = CLITERAL(Color) { 0, 0, 0, 255 };
    palScreen[0x0F] = CLITERAL(Color) { 0, 0, 0, 255 };

    palScreen[0x10] = CLITERAL(Color) { 152, 150, 152, 255 };
    palScreen[0x11] = CLITERAL(Color) { 8, 76, 196, 255 };
    palScreen[0x12] = CLITERAL(Color) { 48, 50, 236, 255 };
    palScreen[0x13] = CLITERAL(Color) { 92, 30, 228, 255 };
    palScreen[0x14] = CLITERAL(Color) { 136, 20, 176, 255 };
    palScreen[0x15] = CLITERAL(Color) { 160, 20, 100, 255 };
    palScreen[0x16] = CLITERAL(Color) { 152, 34, 32, 255 };
    palScreen[0x17] = CLITERAL(Color) { 120, 60, 0, 255 };
    palScreen[0x18] = CLITERAL(Color) { 84, 90, 0, 255 };
    palScreen[0x19] = CLITERAL(Color) { 40, 114, 0, 255 };
    palScreen[0x1A] = CLITERAL(Color) { 8, 124, 0, 255 };
    palScreen[0x1B] = CLITERAL(Color) { 0, 118, 40, 255 };
    palScreen[0x1C] = CLITERAL(Color) { 0, 102, 120, 255 };
    palScreen[0x1D] = CLITERAL(Color) { 0, 0, 0, 255 };
    palScreen[0x1E] = CLITERAL(Color) { 0, 0, 0, 255 };
    palScreen[0x1F] = CLITERAL(Color) { 0, 0, 0, 255 };

    palScreen[0x20] = CLITERAL(Color) { 236, 238, 236, 255 };
    palScreen[0x21] = CLITERAL(Color) { 76, 154, 236, 255 };
    palScreen[0x22] = CLITERAL(Color) { 120, 124, 236, 255 };
    palScreen[0x23] = CLITERAL(Color) { 176, 98, 236, 255 };
    palScreen[0x24] = CLITERAL(Color) { 228, 84, 236, 255 };
    palScreen[0x25] = CLITERAL(Color) { 236, 88, 180, 255 };
    palScreen[0x26] = CLITERAL(Color) { 236, 106, 100, 255 };
    palScreen[0x27] = CLITERAL(Color) { 212, 136, 32, 255 };
    palScreen[0x28] = CLITERAL(Color) { 160, 170, 0, 255 };
    palScreen[0x29] = CLITERAL(Color) { 116, 196, 0, 255 };
    palScreen[0x2A] = CLITERAL(Color) { 76, 208, 32, 255 };
    palScreen[0x2B] = CLITERAL(Color) { 56, 204, 108, 255 };
    palScreen[0x2C] = CLITERAL(Color) { 56, 180, 204, 255 };
    palScreen[0x2D] = CLITERAL(Color) { 60, 60, 60, 255 };
    palScreen[0x2E] = CLITERAL(Color) { 0, 0, 0, 255 };
    palScreen[0x2F] = CLITERAL(Color) { 0, 0, 0, 255 };

    palScreen[0x30] = CLITERAL(Color) { 236, 238, 236, 255 };
    palScreen[0x31] = CLITERAL(Color) { 168, 204, 236, 255 };
    palScreen[0x32] = CLITERAL(Color) { 188, 188, 236, 255 };
    palScreen[0x33] = CLITERAL(Color) { 212, 178, 236, 255 };
    palScreen[0x34] = CLITERAL(Color) { 236, 174, 236, 255 };
    palScreen[0x35] = CLITERAL(Color) { 236, 174, 212, 255 };
    palScreen[0x36] = CLITERAL(Color) { 236, 180, 176, 255 };
    palScreen[0x37] = CLITERAL(Color) { 228, 196, 144, 255 };
    palScreen[0x38] = CLITERAL(Color) { 204, 210, 120, 255 };
    palScreen[0x39] = CLITERAL(Color) { 180, 222, 120, 255 };
    palScreen[0x3A] = CLITERAL(Color) { 168, 226, 144, 255 };
    palScreen[0x3B] = CLITERAL(Color) { 152, 226, 180, 255 };
    palScreen[0x3C] = CLITERAL(Color) { 160, 214, 228, 255 };
    palScreen[0x3D] = CLITERAL(Color) { 160, 162, 160, 255 };
    palScreen[0x3E] = BLACK;
    palScreen[0x3F] = BLACK;

    sprScreen = new Gui::Sprite(256, 240);
    sprNameTable[0] = new Gui::Sprite(256, 240);
    sprNameTable[1] = new Gui::Sprite(256, 240);
    sprPatternTable[0] = new Gui::Sprite(128, 128);
    sprPatternTable[1] = new Gui::Sprite(128, 128);
}

PPU::~PPU()
{
    delete sprScreen;
    delete sprNameTable[0];
    delete sprNameTable[1];
    delete sprPatternTable[0];
    delete sprPatternTable[1];
}

Gui::Sprite& PPU::GetScreen()
{
    return *sprScreen;
}

Gui::Sprite& PPU::GetNameTable(uint8_t i)
{
    return *sprNameTable[i];
}

Gui::Sprite& PPU::GetPatternTable(uint8_t i)
{
    return *sprPatternTable[i];
}

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
    // Fake some noise for now
    int ind = (rand() % 2) ? 0x3F : 0x30;
    sprScreen->setPixel(cycle - 1, scanline, palScreen[ind]);

    // Advance renderer - it never stops, it's relentless
    cycle++;
    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;
        if (scanline >= 261)
        {
            scanline = -1;
            frame_complete = true;
        }
    }
}
