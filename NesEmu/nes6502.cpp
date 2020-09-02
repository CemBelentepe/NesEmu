#include "nes6502.h"
#include "Bus.h"

void nes6502::reset()
{
	addr_abs = 0xFFFC;
	pc = read(addr_abs + 1) << 8 | read(addr_abs);

	reg_a = 0;
	reg_x = 0;
	reg_y = 0;
	sp = 0xFD;
	status_reg = 0 | U;
}

void nes6502::clock()
{
	if (cycles == 0)
	{

	}
	cycles--;
}

uint8_t nes6502::read(uint16_t addr)
{
	return bus->read(addr);
}

void nes6502::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

uint8_t nes6502::getFlag(Flags flagName)
{
	return status_reg & flagName;
}

void nes6502::setFlag(Flags flagName, uint8_t data)
{
	if (data)
		status_reg |= flagName;
	else
		status_reg &= ~flagName;
}
