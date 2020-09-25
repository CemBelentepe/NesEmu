#pragma once

#include <memory>
#include "SFML/Graphics.hpp"

class Cartridge;

class nes2c02
{
private:
	std::shared_ptr<Cartridge> cartridge;

	//Image buffer
	sf::Image screenBuffer;

	//PPU Palette
	uint8_t ppuPalette[0x40][4] = {
		 {84, 84, 84, 0}
		,{0, 30, 116, 0}
		,{8, 16, 144, 0}
		,{48, 0, 136, 0}
		,{68, 0, 100, 0}
		,{92, 0, 48, 0}
		,{84, 4, 0, 0}
		,{60, 24, 0, 0}
		,{32, 42, 0, 0}
		,{8, 58, 0, 0}
		,{0, 64, 0, 0}
		,{0, 60, 0, 0}
		,{0, 50, 60, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}

		,{152, 150, 152, 0}
		,{8, 76, 196, 0}
		,{48, 50, 236, 0}
		,{92, 30, 228, 0}
		,{136, 20, 176, 0}
		,{160, 20, 100, 0}
		,{152, 34, 32, 0}
		,{120, 60, 0, 0}
		,{84, 90, 0, 0}
		,{40, 114, 0, 0}
		,{8, 124, 0, 0}
		,{0, 118, 40, 0}
		,{0, 102, 120, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}

		,{236, 238, 236, 0}
		,{76, 154, 236, 0}
		,{120, 124, 236, 0}
		,{176, 98, 236, 0}
		,{228, 84, 236, 0}
		,{236, 88, 180, 0}
		,{236, 106, 100, 0}
		,{212, 136, 32, 0}
		,{160, 170, 0, 0}
		,{116, 196, 0, 0}
		,{76, 208, 32, 0}
		,{56, 204, 108, 0}
		,{56, 180, 204, 0}
		,{60, 60, 60, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}

		,{236, 238, 236, 0}
		,{168, 204, 236, 0}
		,{188, 188, 236, 0}
		,{212, 178, 236, 0}
		,{236, 174, 236, 0}
		,{236, 174, 212, 0}
		,{236, 180, 176, 0}
		,{228, 196, 144, 0}
		,{204, 210, 120, 0}
		,{180, 222, 120, 0}
		,{168, 226, 144, 0}
		,{152, 226, 180, 0}
		,{160, 214, 228, 0}
		,{160, 162, 160, 0}
		,{0, 0, 0, 0}
		,{0, 0, 0, 0}
	};
	

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

	int16_t scanline = 0;
	int16_t cycle = 0;

	uint8_t bg_next_id = 0x00;
	uint8_t bg_next_attrib = 0x00;
	uint8_t bg_next_pattern_low = 0x00;
	uint8_t bg_next_pattern_high = 0x00;

	uint8_t bg_shifter_pattern_low = 0x00;
	uint8_t bg_shifter_pattern_high = 0x00;
	uint8_t bg_shifter_attrib_low = 0x00;
	uint8_t bg_shifter_attrib_high = 0x00;

	sf::Color& getColorFromPalette(uint8_t palette, uint8_t pixel);
	sf::Image& getScreenBuffer();

public:
	nes2c02();

	void insertCartridge(std::shared_ptr<Cartridge> cartridge);

	//ppu to main bus 
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr);

	//ppu to ppu bus
	void ppuWrite(uint16_t addr, uint8_t data);
	uint8_t ppuRead(uint16_t addr);

	void clock();
	void reset();

	bool nmi = false;
	bool frame_complete = false;
};

