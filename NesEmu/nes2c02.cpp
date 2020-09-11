#include "nes2c02.h"

void nes2c02::insertCartridge(std::shared_ptr<Cartridge> cartridge)
{
	this->cartridge = cartridge;
}

void nes2c02::cpuWrite(uint16_t addr, uint8_t data)
{}

uint8_t nes2c02::cpuRead(uint16_t addr)
{
	return 0;
}

void nes2c02::ppuWrite(uint16_t addr, uint8_t data)
{}

uint8_t nes2c02::ppuRead(uint16_t addr)
{
	return 0;
}

void nes2c02::clock()
{}
