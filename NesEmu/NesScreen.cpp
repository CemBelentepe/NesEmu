#include "NesScreen.h"
#include "Common.h"

#include <fstream>

void NesScreen::renderRegisters()
{
	uint8_t flag = bus.cpu.status_reg;
	std::string c = "N V U B D I Z C";

	for (int i = 0; i < 16; i += 2)
	{
		if (!(flag & 0b1000'0000))
			c[i] = tolower(c[i]);
		flag <<= 1;
	}

	std::string regInfo = "Status: " + c + "\nA: " + hex(bus.cpu.reg_a) +
		"\nX: " + hex(bus.cpu.reg_x) +
		"\nY: " + hex(bus.cpu.reg_y) +
		"\nSP: " + hex(bus.cpu.sp) +
		"\nPC: " + hex(bus.cpu.pc);
		// + "\nCyc:" + hex(bus.cpu.cycles);

	sf::Text text(regInfo, font, 16);
	text.setFillColor(sf::Color::White);
	text.setPosition(650, 0);
	window.draw(text);
}

void NesScreen::renderScreen()
{
	sf::Image& buffer = bus.ppu.getScreenBuffer();
	sf::Texture texture;
	texture.loadFromImage(buffer);
	sf::Sprite screen(texture);
	screen.setScale(2, 2);
	window.draw(screen);
}

void NesScreen::renderCode()
{
	sf::Text text("", font, 16);
	text.setPosition(650, 150);
	auto it = image.find(bus.cpu.pc);
	for (int i = 0; i < 7 && it != image.begin(); i++)
		it--;

	for (int i = 0; i < 16; i++)
	{
		if (it->first == bus.cpu.pc)
			text.setFillColor(sf::Color::Green);
		else
			text.setFillColor(sf::Color::White);
		text.setString(it->second);
		text.setPosition(650, text.getPosition().y + 16);
		window.draw(text);
		it++;
	}
}

void NesScreen::renderNametables()
{}

NesScreen::NesScreen(std::string cartridgePath)
	:window(sf::VideoMode(900, 600), "NES Emu"), cart(std::make_shared<Cartridge>(cartridgePath))
{}

void NesScreen::init()
{
	font.loadFromFile("..\\res\\consola.ttf");
	bus.insertCartridge(cart);
	image = bus.cpu.dissamble(0x0000, 0xFFFF);
	bus.reset();
}

bool NesScreen::update()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				uint16_t pc_prev = bus.cpu.pc;
				while (pc_prev == bus.cpu.pc)
					bus.clock();
			}
			else if (event.key.code == sf::Keyboard::R)
			{
				bus.reset();
			}
		}
	}
	window.clear();

	uint16_t pc_prev = bus.cpu.pc;
	while (pc_prev == bus.cpu.pc)
		bus.clock();

	// renderRegisters();
	renderScreen();
	// renderCode();
	// renderNametables();

	window.display();
	return window.isOpen();
}

void NesScreen::printImage(std::string filename)
{
	std::ofstream file(filename);
	auto it = image.find(0x0000);

	while (it != image.end())
	{
		uint8_t data = bus.cpuRead(it->first);
		if (data == 0)
		{
			file << "0x" << hex(it->first) << ": [0x00]\n";
			while (data == 0)
			{
				data = bus.cpuRead(it->first);
				it++;
			}
		}
		else if (data == 0xFF)
		{
			file << "0x" << hex(it->first) << ": [0xFF]\n";
			while (data == 0xFF)
			{
				data = bus.cpuRead(it->first);
				it++;
			}
		}
		else
			file << it->second << "\n";
		it++;
	}

	file.close();
}
