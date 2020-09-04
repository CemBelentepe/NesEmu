#include "nes6502.h"
#include "Bus.h"

uint8_t nes6502::fetch()
{
	if (instructions[opcode].addrmode != &nes6502::IMP)
		fetched = read(addr_abs);
	return fetched;
}

void nes6502::reset()
{
	addr_abs = 0xFFFC;
	pc = read(addr_abs + 1) << 8 | read(addr_abs);

	reg_a = 0;
	reg_x = 0;
	reg_y = 0;
	sp = 0xFD;
	status_reg = 0 | U;

	addr_abs = 0;
	addr_rel = 0;
	fetched = 0;
}

void nes6502::clock()
{
	if (cycles == 0)
	{
		opcode = read(pc++);

		cycles = instructions[opcode].cycle;

		uint8_t cycle1 = (this->*instructions[opcode].addrmode)();
		uint8_t cycle2 = (this->*instructions[opcode].opcode)();
		cycles += cycle1 & cycle2;
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
	return (status_reg & flagName) > 0;
}

void nes6502::setFlag(Flags flagName, uint8_t data)
{
	if (data)
		status_reg |= flagName;
	else
		status_reg &= ~flagName;
}

uint8_t nes6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

uint8_t nes6502::IMP()
{
	return 0;
}

uint8_t nes6502::ZP0()
{
	addr_abs = read(pc++) % 0xFF;
	return 0;
}

uint8_t nes6502::ZPX()
{
	addr_abs = (read(pc++) + reg_x) % 0xFF;
	return 0;
}

uint8_t nes6502::ZPY()
{
	addr_abs = (read(pc++) + reg_y) % 0xFF;
	return 0;
}

uint8_t nes6502::ABS()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = hi << 8 + lo;
	return 0;
}

uint8_t nes6502::ABX()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) + lo + reg_x;
	return 0;
}

uint8_t nes6502::ABY()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) + lo + reg_y;
	return 0;
}

uint8_t nes6502::ACC()
{
	fetched = reg_a;
	return 0;
}

uint8_t nes6502::REL()
{
	addr_rel = read(pc++);
	return 0;
}

uint8_t nes6502::IND()
{
	uint8_t lo = read(pc++);
	uint8_t hi = read(pc++);
	uint16_t ptr = (hi << 8) + lo;
	addr_abs = read(ptr + 1) << 8 + read(ptr);
	return 0;
}
