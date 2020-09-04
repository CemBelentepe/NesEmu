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
	addr_abs = read(pc++) & 0xFF;
	return 0;
}

uint8_t nes6502::ZPX()
{
	addr_abs = (read(pc++) + reg_x) & 0xFF;
	return 0;
}

uint8_t nes6502::ZPY()
{
	addr_abs = (read(pc++) + reg_y) & 0xFF;
	return 0;
}

uint8_t nes6502::ABS()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = hi << 8 | lo;
	return 0;
}

uint8_t nes6502::ABX()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + reg_x;
	return (hi << 8) != (addr_abs & 0xFF00); // Page wrapping
}

uint8_t nes6502::ABY()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = ((hi << 8) | lo) + reg_y;
	return (hi << 8) != (addr_abs & 0xFF00); // Page wrapping
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
	uint16_t ptr = (hi << 8) | lo;
	if(lo == 0xFF)
		addr_abs = (read((ptr + 1) & 0xFF) << 8) | read(ptr);
	else
		addr_abs = read(ptr + 1) << 8 | read(ptr);
	return 0;
}

uint8_t nes6502::IZX()
{
	uint8_t arg = read(pc++);
	uint8_t ptr_lo = read((arg + reg_x) & 0xFF);
	uint8_t ptr_hi = read((arg + reg_x + 1)& 0xff);
	addr_abs = ptr_hi << 8 | ptr_lo;
	return 0;
}

uint8_t nes6502::IZY()
{
	uint8_t arg = read(pc++);
	uint8_t lo = read(arg);
	uint8_t hi = read((arg + 1) & 0xFF);
	addr_abs = (hi << 8 | lo) + reg_y;
	return (hi << 8) != (addr_abs & 0xFF00); // Page wrapping
}

uint8_t nes6502::ADC()
{
	return uint8_t();
}

uint8_t nes6502::AND()
{
	return uint8_t();
}

uint8_t nes6502::ASL()
{
	return uint8_t();
}

uint8_t nes6502::BCC()
{
	return uint8_t();
}

uint8_t nes6502::BCS()
{
	return uint8_t();
}

uint8_t nes6502::BEQ()
{
	return uint8_t();
}

uint8_t nes6502::BIT()
{
	return uint8_t();
}

uint8_t nes6502::BMI()
{
	return uint8_t();
}

uint8_t nes6502::BNE()
{
	return uint8_t();
}

uint8_t nes6502::BPL()
{
	return uint8_t();
}

uint8_t nes6502::BRK()
{
	return uint8_t();
}

uint8_t nes6502::BVC()
{
	return uint8_t();
}

uint8_t nes6502::BVS()
{
	return uint8_t();
}

uint8_t nes6502::CLC()
{
	return uint8_t();
}

uint8_t nes6502::CLD()
{
	return uint8_t();
}

uint8_t nes6502::CLI()
{
	return uint8_t();
}

uint8_t nes6502::CLV()
{
	return uint8_t();
}

uint8_t nes6502::CMP()
{
	return uint8_t();
}

uint8_t nes6502::CPX()
{
	return uint8_t();
}

uint8_t nes6502::CPY()
{
	return uint8_t();
}

uint8_t nes6502::DEC()
{
	return uint8_t();
}

uint8_t nes6502::DEX()
{
	return uint8_t();
}

uint8_t nes6502::DEY()
{
	return uint8_t();
}

uint8_t nes6502::EOR()
{
	return uint8_t();
}

uint8_t nes6502::INC()
{
	return uint8_t();
}

uint8_t nes6502::INX()
{
	return uint8_t();
}

uint8_t nes6502::INY()
{
	return uint8_t();
}

uint8_t nes6502::JMP()
{
	return uint8_t();
}

uint8_t nes6502::JSR()
{
	return uint8_t();
}

uint8_t nes6502::LDA()
{
	return uint8_t();
}

uint8_t nes6502::LDX()
{
	return uint8_t();
}

uint8_t nes6502::LDY()
{
	return uint8_t();
}

uint8_t nes6502::LSR()
{
	return uint8_t();
}

uint8_t nes6502::NOP()
{
	return uint8_t();
}

uint8_t nes6502::ORA()
{
	return uint8_t();
}

uint8_t nes6502::PHA()
{
	return uint8_t();
}

uint8_t nes6502::PHP()
{
	return uint8_t();
}

uint8_t nes6502::PLA()
{
	return uint8_t();
}

uint8_t nes6502::PLP()
{
	return uint8_t();
}

uint8_t nes6502::ROL()
{
	return uint8_t();
}

uint8_t nes6502::ROR()
{
	return uint8_t();
}

uint8_t nes6502::RTI()
{
	return uint8_t();
}

uint8_t nes6502::RTS()
{
	return uint8_t();
}

uint8_t nes6502::SBC()
{
	return uint8_t();
}

uint8_t nes6502::SEC()
{
	return uint8_t();
}

uint8_t nes6502::SED()
{
	return uint8_t();
}

uint8_t nes6502::SEI()
{
	return uint8_t();
}

uint8_t nes6502::STA()
{
	return uint8_t();
}

uint8_t nes6502::STX()
{
	return uint8_t();
}

uint8_t nes6502::STY()
{
	return uint8_t();
}

uint8_t nes6502::TAX()
{
	return uint8_t();
}

uint8_t nes6502::TAY()
{
	return uint8_t();
}

uint8_t nes6502::TSX()
{
	return uint8_t();
}

uint8_t nes6502::TXA()
{
	return uint8_t();
}

uint8_t nes6502::TXS()
{
	return uint8_t();
}

uint8_t nes6502::TYA()
{
	return uint8_t();
}

uint8_t nes6502::XXX()
{
	return uint8_t();
}
