#include "Bus.h"

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return ram[addr];
	
	return 0;
}
