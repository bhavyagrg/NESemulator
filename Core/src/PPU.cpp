#include "PPU.h"
#include "Cartridge.h"

PPU::PPU()
{
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);

    sprScreen = new olc::Sprite(256, 240);
    sprNameTable[0] = new olc::Sprite(256, 240);
    sprNameTable[1] = new olc::Sprite(256, 240);
    // ------Two sprites to visualise the pattern memory-----
    sprPatternTable[0] = new olc::Sprite(128, 128);
    sprPatternTable[1] = new olc::Sprite(128, 128);
}

PPU::~PPU()
{
    delete sprScreen;
    delete sprNameTable[0];
    delete sprNameTable[1];
    delete sprPatternTable[0];
    delete sprPatternTable[1];
}

olc::Sprite& PPU::GetScreen()
{
    return *sprScreen;
}

olc::Sprite& PPU::GetNameTable(uint8_t i)
{
    return *sprNameTable[i];
}

olc::Sprite& PPU::GetPatternTable(uint8_t i,uint8_t palette)
{
    // ----- For a given pattern table there are 16 by 16 tiles
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
    {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
        {
            // converting 2D coordinate into 1D coordinate
            // 256 because a single tile consists of 16 bytes of information
            uint16_t nOffset = nTileY * 256 + nTileX * 16;

            // for each tile we have 8 rows of 8 pixels
            for (uint16_t row = 0; row < 8; row++)
            {
                // To read from the pattern memory we need ppu read function
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0);//least significant bit plane
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 8);


                for (uint16_t col = 0; col < 8; col++)
                {
                    // adding least significant bit to get the colour of a pixel, It will be give us a value between 0 and 3
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
                    tile_lsb >>= 1; tile_msb >>= 1; // Shift by 1

                    // Now drawing that pixel value
                    sprPatternTable[i]->SetPixel
                    (
                        // We are drawing from the top left first , so we need to invert 
                        // This refers to the right most pixel, so inversion or 7 - col is necessary
                        nTileX * 8 + (7 - col),
                        nTileY * 8 + row,
                        // Finally choose the colour, 
                        GetColourFromPaletteRam(palette,pixel)
                    );

                }
            }
        }
    }

    return *sprPatternTable[i];
}

olc::Pixel& PPU::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel)
{
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
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
        status.vertical_blank = 1;
        // when we read from the status reg we are interesed in the top 3 bits. the unused bits tend to fill with noise or likely what was last on the data buffer
        data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
        // reading from status register also clear vertical blank flag and address latch
        status.vertical_blank = 0;
        address_latch = 0;
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
        // we can read from the data reg but it is dealyed by 1 read, so store it in a buffer
        data = ppu_data_buffer;
        ppu_data_buffer = ppuRead(ppu_address); // and read the data of ppu_add to the buffer

        /* above delay is true for whole emulation except where the palette resides
         so we need to put in a special case to handle pallete addresses.*/
        if (ppu_address >= 0x3f00)
            data = ppu_data_buffer;
        ppu_address++;
        break;
    }

    return data;
}

void PPU::cpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x0000: //------ Control
        control.reg = data;
        break;
    case 0x0001: //------ Mask
        mask.reg = data;
        break;
    case 0x0002: //------ Status -- can't write to it
        break;
    case 0x0003: //------ OAM Address
        break;
    case 0x0004: //------ OAM Data
        break;
    case 0x0005: //------ Scroll
        if (address_latch == 0)
        {
          /*   First write to scroll register contains X offset in pixel space
             which we split into coarse and fine x values*/
            fine_x = data & 0x07;
            tram_addr.coarse_x = data >> 3;
            address_latch = 1;
        }
        else
        {
             /*First write to scroll register contains Y offset in pixel space
             which we split into coarse and fine Y values*/
            tram_addr.fine_y = data & 0x07;
            tram_addr.coarse_y = data >> 3;
            address_latch = 0;
        }
        break;
    case 0x0006: //------ PPU Address
        if (address_latch == 0)
        {
            /* PPU address bus can be accessed by CPU via the ADDR and DATA
             registers. The fisrt write to this register latches the high byte
             of the address, the second is the low byte. Note the writes
             are stored in the tram register...*/

            /*tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
            address_latch = 1;*/

            ppu_address = (ppu_address & 0x00FF) | (data << 8);
            address_latch = 1;
        }
        else
        {
            /* ...when a whole address has been written, the internal vram address
             buffer is updated. Writing to the PPU is unwise during rendering
             as the PPU will maintam the vram address automatically whilst
             rendering the scanline position.*/

            /*tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
            vram_addr = tram_addr;
            address_latch = 0;*/

            ppu_address = (ppu_address & 0xFF00) | data;
            address_latch = 0;
        }
        break;
    case 0x0007: //------ PPU Data
        //ppuWrite(vram_addr.reg, data);
        ///* All writes from PPU data automatically increment the nametable
        // address depending upon the mode set in the control register.
        // If set to vertical mode, the increment is 32, so it skips
        // one whole nametable row; in horizontal mode it just increments
        // by 1, moving to the next column*/
        //vram_addr.reg += (control.increment_mode ? 32 : 1);
        ppu_address += (control.increment_mode ? 32 : 1); // if it's 1 , increment along x axis , if it's 32 skipping 32 tiles in x axis, which is same as going down a row in y axis
        ppuWrite(ppu_address, data);
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
    //pattern mem
    else if (address >= 0x0000 && address <= 0x1FFF)
    {
        //first dimension choses whether it's left side or right side of the array
        data = pattern_mem[(address & 0x1000) >> 12][address & 0x0FFF];
    }
    //name table mem
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        address &= 0x0FFF;

        if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            // Vertical
            if (address >= 0x0000 && address <= 0x03FF)
                data = vRam[0][address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                data = vRam[1][address & 0x03FF];
            if (address >= 0x0800 && address <= 0x0BFF)
                data = vRam[0][address & 0x03FF];
            if (address >= 0x0C00 && address <= 0x0FFF)
                data = vRam[1][address & 0x03FF];
        }

        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            // Horizontal
            if (address >= 0x0000 && address <= 0x03FF)
                data = vRam[0][address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                data = vRam[0][address & 0x03FF];
            if (address >= 0x0800 && address <= 0x0BFF)
                data = vRam[1][address & 0x03FF];
            if (address >= 0x0C00 && address <= 0x0FFF)
                data = vRam[1][address & 0x03FF];
        }


    }
    // Palette memory
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        address &= 0x001F;
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;
        data = palette_mem[address];
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
    else if (address >= 0x0000 && address <= 0x1FFF)
    {
        pattern_mem[(address & 0x1000) >> 12][address & 0x0FFF] = data;
    }
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        address &= 0x0FFF;
        if(cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            // Vertical
            if (address >= 0x0000 && address <= 0x03FF)
                vRam[0][address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                vRam[1][address & 0x03FF] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                vRam[0][address & 0x03FF] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                vRam[1][address & 0x03FF] = data;
        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            // Horizontal
            if (address >= 0x0000 && address <= 0x03FF)
                vRam[0][address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                vRam[0][address & 0x03FF] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                vRam[1][address & 0x03FF] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                vRam[1][address & 0x03FF] = data;
        }
    }
    // Palette memory
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        address &= 0x001F;
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;
        palette_mem[address] = data;
    }
}

void PPU::ConnectCartrdige(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cart = cartridge;
}

void PPU::clock()
{
    if (scanline == -1 && cycle == 1)
    {
        status.vertical_blank = 0;
    }

    if (scanline == 241 && cycle == 1)
    {
        status.vertical_blank = 1;
        if (control.enable_nmi)
            nmi = true;
    }

    // Fake some noise for now
    //sprScreen->SetPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);

    // Advance renderer - it never stops, it's relentless
    cycle++;
    if (cycle >= 341)
    {
        // if cycle is greater than 341 reset cycle and increase scanline
        cycle = 0;
        scanline++;
        if (scanline >= 261)
        {
            // we are scrolling across the screen in the x direction and y direction
            scanline = -1;
            frame_complete = true;
        }
    }
}
