#pragma once

#include <memory>

class Cartridge;

class nes2c02
{
private:
	std::shared_ptr<Cartridge> cartridge;

	//Tables
	uint8_t nameTable[2][1024];
	uint8_t patternTable[2][4096];
	uint8_t paletteTable[32];

	//Registers
	union PPUMASK
	{
		struct 
		{
			uint8_t greyscale : 1;
			uint8_t bg_left_show : 1;
			uint8_t sprite_left_show : 1;
			uint8_t bg_show : 1;
			uint8_t sprite_show : 1;
			uint8_t red_emphasize : 1;
			uint8_t green_emphasize: 1;
			uint8_t blue_emphasize : 1;
		};

		uint8_t reg;
	} mask_reg;

	union PPUSTATUS
	{
		struct
		{
			uint8_t unused : 5;
			//buggy flag which creates false positives and negatives
			uint8_t overflow_sprite : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vblank : 1;
		};
		
		uint8_t reg;
	} status_reg;

	union PPUCTRL
	{
		struct
		{
			uint8_t x_nametable : 1;
			uint8_t y_nametable : 1;
			uint8_t inc_mode : 1;
			uint8_t sprite_patterntable : 1;
			uint8_t bg_patterntable : 1;
			uint8_t size_sprite : 1;
			uint8_t slave_mode : 1;
			uint8_t generate_nmi : 1;
		};

		uint8_t reg;
	} control_reg;

	union LOOPYREG
	{
		struct
		{
			uint16_t x_coarse : 5;
			uint16_t y_coarse : 5;
			uint16_t x_nametable : 1;
			uint16_t y_nametable : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};

	LOOPYREG vram_addr;
	LOOPYREG tram_addr;

	uint8_t fine_x = 0x00;

	uint8_t addr_latch = 0x00;
	uint8_t ppu_data_buffer = 0x00;

public:
	void insertCartridge(std::shared_ptr<Cartridge> cartridge);

	//ppu to main bus 
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr);

	//ppu to ppu bus
	void ppuWrite(uint16_t addr, uint8_t data);
	uint8_t ppuRead(uint16_t addr);

	void clock();
	void reset();
};

