#pragma once

#include <array>
#include <cinttypes>

class Bus
{
private:
	std::array<uint8_t, 0xFFFF> ram;

public:
	Bus()
	{
		ram.fill(0);
	}

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
};

