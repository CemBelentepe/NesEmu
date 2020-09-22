#include "nes2c02.h"
#include "Cartridge.h"

void nes2c02::insertCartridge(std::shared_ptr<Cartridge> cartridge)
{
	this->cartridge = cartridge;
}

void nes2c02::cpuWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{

	case 0x0000:
		control_reg.reg = data;
		tram_addr.x_nametable = control_reg.x_nametable;
		tram_addr.y_nametable = control_reg.y_nametable;
		break;
	case 0x0001:
		mask_reg.reg = data;
		break;
	case 0x0002:
		//can't write into status reg
		break;
	case 0x0003:
		break;
	case 0x0004:
		break;
	case 0x0005:
		if (addr_latch == 0)
		{
			fine_x = data & 0x07;
			tram_addr.x_coarse = data >> 3;
			addr_latch = 1;
		}
		else
		{
			tram_addr.fine_y = data & 0x07;
			tram_addr.y_coarse = data >> 3;
			addr_latch = 0;
		}
		break;
	case 0x0006:
		//accumulate data in tram then write to vram
		if (addr_latch == 0)
		{
			//read high first, low second
			tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
			addr_latch = 1;
		}
		else
		{
			tram_addr.reg = (uint16_t) data | (tram_addr.reg & 0xFF00);
			vram_addr = tram_addr;
			addr_latch = 0;
		}
		break;
	case 0x0007:
		ppuWrite(vram_addr.reg, data);

		//check inc mode and inc vram
		vram_addr.reg += (control_reg.inc_mode ? 32 : 1);
		break;
	}

}

uint8_t nes2c02::cpuRead(uint16_t addr)
{
	uint8_t data = 0x00;

	switch (addr)
	{
	
	case 0x0000:
		//ctrl can't be read
		break;
	case 0x0001:
		//mask can't be read
		break;
	case 0x0002:
		data = (status_reg.reg & 0xE0) | (ppu_data_buffer & 0x1F);
		status_reg.vblank = 0;
		addr_latch = 0;
		break;

	case 0x0003:
		break;
	case 0x0004:
		break;
	case 0x0005:
		//scroll can't be read
		break;
	case 0x0006:
		//ppu addr can't be read
		break;
	case 0x0007:

		data = ppu_data_buffer;
		ppu_data_buffer = ppuRead(vram_addr.reg);

		//no need to delay if reading from palette range
		if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;

		//check inc mode and inc vram
		vram_addr.reg += (control_reg.inc_mode ? 32 : 1);
		break;
	}
	
	return data;
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
{
	if (scanline >= -1 && scanline < 240)
	{
		if (scanline == 0 && cycle == 0)  cycle = 1;
		if (scanline == -1 && cycle == 1) status_reg.vblank = 0;

		//http://wiki.nesdev.com/w/index.php/PPU_scrolling
		//http://wiki.nesdev.com/w/index.php/PPU_rendering
		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			switch ((cycle - 1) % 8)
			{
			case 0:
				bg_next_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
				break;
			case 2:
			case 4:
			case 6:
			case 7:
			}
		}
	}



	if (scanline == 241 && cycle == 1)
	{
		status_reg.vblank = 1;
		if (control_reg.generate_nmi) nmi = true;
	}

}

void nes2c02::reset()
{
	fine_x = 0x00;
	addr_latch = 0x00;
	ppu_data_buffer = 0x00;
	status_reg.reg = 0x00;
	mask_reg.reg = 0x00;
	control_reg.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}