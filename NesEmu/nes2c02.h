#pragma once

#include <memory>

class Cartridge;

class nes2c02
{
private:
	std::shared_ptr<Cartridge> cartridge;
	uint8_t nameTable[2][1024];
	uint8_t patternTable[2][4096];
	uint8_t paletteTable[32];

public:
	void insertCartridge(std::shared_ptr<Cartridge> cartridge);

	//ppu to main bus 
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr);

	//ppu to ppu bus
	void ppuWrite(uint16_t addr, uint8_t data);
	uint8_t ppuRead(uint16_t addr);

	void clock();
};

