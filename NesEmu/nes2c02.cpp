#include "nes2c02.h"
#include "Cartridge.h"

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
{
	addr &= 0x3FFF;

	if(cartridge->ppuWrite(addr, data)){}
	else if (addr >= 0x0000 && 0x1FFF)
	{
		patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (cartridge->mirror == Cartridge::Mirror::VERTICAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF) nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF) nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF) nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF) nameTable[1][addr & 0x03FF] = data;
		}
		else if (cartridge->mirror == Cartridge::Mirror::HORIZONTAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF) nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF) nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF) nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF) nameTable[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;

		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		paletteTable[addr] = data;
	}
}

uint8_t nes2c02::ppuRead(uint16_t addr)
{
	uint8_t temp = 0x00;
	addr &= 0x3FFF;
	
	if (cartridge->ppuRead(addr, temp)) {}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		//if cartridge can't map
		temp = patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		//check cartridge mirroring to access name table
		addr &= 0x0FFF;
		if (cartridge->mirror == Cartridge::Mirror::VERTICAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF) temp = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF) temp = nameTable[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF) temp = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF) temp = nameTable[1][addr & 0x03FF];
		}
		else if (cartridge->mirror == Cartridge::Mirror::HORIZONTAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF) temp = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF) temp = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF) temp = nameTable[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF) temp = nameTable[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		//use palette memory
		addr &= 0x001F;
		//addr refers to bg color
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		temp = paletteTable[addr] & (mask_reg.greyscale ? 0x30 : 0x3F);
	}

	return temp;
}

void nes2c02::clock()
{}
