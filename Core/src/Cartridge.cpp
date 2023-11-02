#include "Cartridge.h"

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
	} header;

	bImageValid = false;

	std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary); // opening the nes file in binary mode and immediately reading the header
	if (ifs.is_open())
	{
		// Read file header
		ifs.read((char*)&header, sizeof(sHeader));

		// If a "trainer" exists we just need to read past
		// it before we get to the good stuff
		if (header.mapper1 & 0x04) 
			ifs.seekg(512, std::ios_base::cur); //521 bytes are junk
		 
		// Determine Mapper ID--> extracting which mapper rom is using
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;// extracting how the cartridge is mirrored

		// "Discover" File Format-->> there are 3 types of file formats and we are interested in type 1 file format
		uint8_t nFileType = 1;

		if (nFileType == 0)
		{

		}

		if (nFileType == 1)
		{
			nPRGBanks = header.prg_rom_chunks; // how many banks of memory are in the ROM for the program memory
			vPRGMemory.resize(nPRGBanks * 16384); // resizing our vector to that size
			ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());// reading data from the file directly into the vector

			// doing above three things for the character memory
			nCHRBanks = header.chr_rom_chunks;
			vCHRMemory.resize(nCHRBanks * 8192);
			ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
		}

		if (nFileType == 2)
		{

		}

		//one I've loaded vectors to represent my program and character memories,I then need to Load appropriate mapper based on the mapper id
		// using polymorphism to selectively choose which mapper class i want to use
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
bool Cartridge::cpuRead(uint16_t address, uint8_t & data)
{
	uint32_t mapped_address = 0;// transformed address var
	// address will only be transformed if corresponding mapper routine for that particular bus says that information has to come from cartridge , the mapped addr variable now stores an offset into the ROM data which I have accessed from the file.
	if (pMapper->cpuMapRead(address, mapped_address))
	{
		data = vPRGMemory[mapped_address];// accessing the data directly
		return true;
	}
	// true if cartrdige is interested else return false
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
	// ppu write uses char memory others are using program memory
	uint32_t mapped_address = 0;
	if (pMapper->ppuMapRead(address, mapped_address))
	{
		vCHRMemory[mapped_address] = data;
		return true;
	}
	else
		return false;
}
