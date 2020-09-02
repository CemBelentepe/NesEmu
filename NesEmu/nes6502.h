#pragma once

#include <memory>

class Bus;

class nes6502
{
private:
	enum class FlagName
	{
		C = 0,
		Z = 1,
		I = 2,
		D = 3,
		B = 4,
		V = 6,
		N = 7,
	};

	union Flag
	{
		struct
		{
			uint8_t N : 1;
			uint8_t V : 1;
			uint8_t s : 1;
			uint8_t B : 1;
			uint8_t D : 1;
			uint8_t I : 1;
			uint8_t Z : 1;
			uint8_t C : 1;
		} named_data;

		uint8_t value;
	};

	uint8_t reg_a;
	uint8_t reg_x;
	uint8_t reg_y;
	uint8_t sp;
	uint16_t pc;
	Flag status_reg;

	std::unique_ptr<Bus> bus;

	uint8_t IMM();	uint8_t IMP();
	uint8_t ZP0();	uint8_t ABS();
	uint8_t ZPX();	uint8_t ABX();
	uint8_t ZPY();	uint8_t ABY();
	uint8_t ACC();	uint8_t REL();
	uint8_t IND();	uint8_t IZX();
	uint8_t IZY();

public:
	nes6502(std::unique_ptr<Bus> bus)
		: reg_a(0), reg_x(0), reg_y(0), sp(0), pc(0), status_reg({0}), bus(std::move(bus))
	{}

	void reset();
	void clock();
	void nmi();
	void irq();

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	uint8_t getFlag(FlagName flagName);
	void setFlag(FlagName flagName, uint8_t data);
};

