#include "Cartridge.h"

#include "raylib.h"
#include <iostream>

Cartridge::Cartridge(const std::string& sFileName)
{
    // iNES Format Header
    struct sHeader
    {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header{};

    bImageValid = false;

    std::ifstream ifs; // input file stream
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        SetWindowTitle(std::filesystem::path(sFileName).filename().string().c_str());
        // Read file header
        ifs.read((char*)&header, sizeof(sHeader));

        // If a "trainer" exists we just need to read past
        // it before we get to the good stuff
        if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

        // Determine Mapper ID
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        // "Discover" File Format
        uint8_t nFileType = 1;

        if (nFileType == 0)
        {

        }

        if (nFileType == 1)
        {
            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }

        if (nFileType == 2)
        {

        }

        // Load appropriate mapper
        switch (nMapperID)
        {
        case 0: pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks); break;
        }

        bImageValid = true;
        ifs.close();
    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::ImageValid()
{
    return bImageValid;
}

//--------- We will tell the system whether cartridge is reading or writing using boolean values-----------
bool Cartridge::cpuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped_address = 0;
    // address will only be transformed if corresponding mapper routine for that particular bus says that information has to come from cartridge , the mapped addr variable now stores an offset into the ROM data which I have accessed from the file.
    if (pMapper->cpuMapRead(address, mapped_address))
    {
        data = vPRGMemory[mapped_address];
        return true;
    }
    else
        return false;
}

bool Cartridge::cpuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped_address = 0;
    if (pMapper->cpuMapWrite(address, mapped_address))
    {
        vPRGMemory[mapped_address] = data;
        return true;
    }
    else
        return false;
}

bool Cartridge::ppuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped_address = 0;
    if (pMapper->ppuMapRead(address, mapped_address))
    {
        data = vCHRMemory[mapped_address];
        return true;
    }
    else
        return false;
}

bool Cartridge::ppuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped_address = 0;
    if (pMapper->ppuMapRead(address, mapped_address))
    {
        vCHRMemory[mapped_address] = data;
        return true;
    }
    else
        return false;
}

void Cartridge::reset(){

}


