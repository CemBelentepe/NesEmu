#pragma once

#include <memory>
#include <string>
#include <vector>

class Bus;

class nes6502
{
private:
	enum Flags
	{
		C = 1,
		Z = 2,
		I = 4,
		D = 8,
		B = 16,
		U = 32,
		V = 64,
		N = 128,
	};

	struct Instruction
	{
		std::string name;
		uint8_t(nes6502::* op)(void) = nullptr;
		uint8_t(nes6502::* addrmode)(void) = nullptr;
		uint8_t cycle = 0;
	};

	uint8_t  reg_a = 0;
	uint8_t  reg_x = 0;
	uint8_t  reg_y = 0;
	uint8_t  sp = 0;
	uint16_t pc = 0;
	uint8_t	 status_reg = 0;
	uint16_t addr_abs = 0;
	uint8_t	 cycles = 0;
	uint8_t  fetched = 0;
	uint8_t	 opcode = 0;
	std::unique_ptr<Bus> bus;
	std::vector<Instruction> instructions = {/*Good luck*/ };

	uint8_t IMM();	uint8_t IMP();
	uint8_t ZP0();	uint8_t ABS();
	uint8_t ZPX();	uint8_t ABX();
	uint8_t ZPY();	uint8_t ABY();
	uint8_t ACC();	uint8_t REL();
	uint8_t IND();	uint8_t IZX();
	uint8_t IZY();

public:
	nes6502(std::unique_ptr<Bus> bus)
		: bus(std::move(bus))
	{}

	void fetch();

	void reset();
	void clock();
	void nmi();
	void irq();

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	uint8_t getFlag(Flags flagName);
	void setFlag(Flags flagName, uint8_t data);
};

