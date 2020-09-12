#include <memory>
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "Cartridge.h"
#include "Bus.h"


int main()
{
	auto byteToHex = [](uint8_t num) -> std::string {
		std::string str = "00";
		for (int i = 0; i < 2; i++)
		{
			str[1 - i] = "0123456789ABCDEF"[num % 16];
			num /= 16;
		}
		return str;
	};

	auto wordToHex = [&](uint16_t num) -> std::string {
		return byteToHex((num & 0xFF00) >> 8) + byteToHex(num & 0x00FF);
	};


	auto cart = std::make_shared<Cartridge>("..\\tests\\nestest.nes");
	if (!cart->imageValid())
		return 1;
	Bus nes;
	nes.insertCartridge(cart);

	std::map<uint16_t, std::string> ram = nes.cpu.dissamble(0x0000, 0xFFFF);

	nes.reset();
#if 0
	std::ofstream file("out.txt");
	auto it = ram.find(0x0000);

	while(it != ram.end())
	{
		uint8_t data = nes.cpuRead(it->first);
		if (data == 0)
		{
			file << "0x" << wordToHex(it->first) << ": [0x00]\n";
			while (data == 0)
			{
				data = nes.cpuRead(it->first);
				it++;
			}
		}
		else if (data == 0xFF)
		{
			file << "0x" << wordToHex(it->first) << ": [0xFF]\n";
			while (data == 0xFF)
			{
				data = nes.cpuRead(it->first);
				it++;
			}
		}
		else
			file << it->second << "\n";
		it++;
	}

	file.close();
#endif

	while (true)
	{
		nes.cpu.clock();

		std::cout << std::hex << "A:" << int(nes.cpu.reg_a) << " X:" << int(nes.cpu.reg_x) << " Y:" << int(nes.cpu.reg_y) << "\n" <<
			"PC:" << int(nes.cpu.pc) << " SP: " << int(nes.cpu.sp) << "\n" <<
			"Cycle:" << int(nes.cpu.cycles) << "\n";
		uint8_t p = nes.cpu.status_reg;
		std::string c = "NVUBDIZC";

		for(int i = 0; i < 8; i++)
		{
			if (!(p & 0b1000'0000))
				c[i] = tolower(c[i]);
			p <<= 1;
		}
		std::cout << c << "\n\n";

		auto it = ram.find(nes.cpu.pc);

		for (int i = 0; i < 8; i++)
		{
			std::cout << it->second << "\n";
			it++;
		}

		std::cin.get();
		system("cls");
	}

	return 0;
}