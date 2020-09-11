#pragma once
#include "Mapper.h"

class Mapper_000 :
	public Mapper
{
public:
	Mapper_000(uint8_t nPRGBank, uint8_t nCHRBank)
		: Mapper(nPRGBank, nCHRBank)
	{}

	bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr)override;
	bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;

	bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)override;
	bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
};

