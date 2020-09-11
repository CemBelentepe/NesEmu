#pragma once

#include <memory>

class Cartridge;

class nes2c02
{
private:
	std::shared_ptr<Cartridge> cartridge;

public:
	void insertCartridge(std::shared_ptr<Cartridge> cartridge);

	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr);

	void ppuWrite(uint16_t addr, uint8_t data);
	uint8_t ppuRead(uint16_t addr);

	void clock();
};

