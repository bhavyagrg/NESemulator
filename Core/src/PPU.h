#pragma once
#include <cstdint>
#include<memory>
#include "Cartridge.h"
#include "external\PixelGameEngine.h"
 
class PPU
{
public:
	PPU();
	~PPU();


// ------------------Memories--------------
public:
	uint8_t vRam[2][1024]; // ---->> Used to hold name table information(2KB)
	uint8_t palette_mem[32]; // Ram connected to PPU bus to store palette info
	uint8_t pattern_mem[2][4096]; // ---> not necessary

public:
	// --------------Connects PPU to CPU Bus------------------
	// -->> Communications  with Main Bus
	uint8_t cpuRead(uint16_t address, bool rdonly = false);
	void cpuWrite(uint16_t address, uint8_t data);

	// --------------Connects PPU to it's own Bus------------------
	// -->> Communications  with PPU Bus
	uint8_t ppuRead(uint16_t address, bool rdonly = false);
	void ppuWrite(uint16_t address, uint8_t data);


private:
	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	void ConnectCartrdige(const std::shared_ptr<Cartridge>& cartridge);
	void clock();

	// olc::Sprite sprScreen = olc::Sprite(256, 240);
	// olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	// olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

	// Changed To for API breaking subsequent PGE Update
private:
	olc::Pixel palScreen[0x40];// array of colours of nes or palette of nes
	olc::Sprite* sprScreen; // sprite that represent full screen output
	olc::Sprite* sprNameTable[2];// sprite that represents graphical depiction of both name tables
	olc::Sprite* sprPatternTable[2];// sprite that represents depiction of both pattern tables

public:
	// Debugging Utilities --->> functions to access the sprites
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i,uint8_t palette);

	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);

	bool frame_complete = false; // boolean that representes when a frame is complete

private:
	int16_t scanline = 0; // represents which row on the screen
	int16_t cycle = 0; // which col on the screen

	// status register
	union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t reg;
	}status;

	//mask register---->> Mask register is just a series of switches that determine which parts of the PPU are turned on or off
	union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t reg;
	} mask;

	// control register
	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};

		uint8_t reg;
	} control;

	union loopy_register
	{
		// Credit to Loopy for working this out :D
		struct
		{

			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};


	loopy_register vram_addr; // Active "pointer" address into nametable to extract background tile info
	loopy_register tram_addr; // Temporary store of information to be "transferred" into "pointer" at various times

	// Pixel offset horizontally
	uint8_t fine_x = 0x00;

	// Internal communications
	uint8_t address_latch = 0x00;// to know whether i am writing to a low byte or a high byte
	uint8_t ppu_data_buffer = 0x00;// when we read from ppu it's delayed by one byte , so we need to buffer it
	uint16_t ppu_address = 0x0000; // to store the compiled address


	// Background rendering
	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attrib = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;// represent plane of the pattern memory for the next 8 pixels
	uint8_t bg_next_tile_msb = 0x00;
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;


private:
	struct sObjectAttributeEntry
	{
		uint8_t y; // Y position  of sprite
		uint8_t t_id; // tile id from pattern memory
		uint8_t attribute;// flags which define how sprite should be rendered
		uint8_t x; // X position of sprite
	}OAM[64];

	// OAM is an array of type sObjectAttributeEntry
public:
	void reset();
	bool nmi = false;

	uint8_t* pOAM = (uint8_t*)OAM; // pointer to the base if the array OAM

	uint8_t oam_addr = 0x00;

};

