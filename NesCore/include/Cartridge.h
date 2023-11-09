#pragma once
#include <cstdint>
#include<vector>
#include <fstream>
#include <filesystem>
#include<string>

#include "Mapper_000.h"

class Cartridge
{
public:
    Cartridge(const std::string& sFileName);
    ~Cartridge();

    bool ImageValid();

    enum MIRROR
    {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;

private:
    bool bImageValid = false;
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

    std::shared_ptr<Mapper> pMapper; // ----- pointer to the mapper class

public:
    // --------------Connects PPU to CPU Bus------------------
    // -->> Communications  with Main Bus
    bool cpuRead(uint16_t address, uint8_t& data);
    bool cpuWrite(uint16_t address, uint8_t data);

    // --------------Connects PPU to it's own Bus------------------
    // -->> Communications  with PPU Bus
    bool ppuRead(uint16_t address, uint8_t& data);
    bool ppuWrite(uint16_t address, uint8_t data);
    void reset();
};

