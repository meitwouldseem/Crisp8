#include <fstream>
#include <bitset>
#include "Chip8Machine.h"

Chip8Machine::Chip8Machine(const std::shared_ptr<Display>& display)
    : display_(display)
    , programOffset_(0x200)
{
    s_.PC = programOffset_;
}

void Chip8Machine::loadRom(std::string name)
{
    std::fstream fs;
    fs.open(name, std::fstream::in | std::fstream::binary);
    if (!fs.is_open())
        throw std::runtime_error("Failed to load rom");

    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);

    fs.read((char*)memory_.data()+programOffset_, length);
}

void Chip8Machine::advance()
{
    uint16_t oppcode = readMemory(s_.PC);
    s_.PC += 2;

    switch ((oppcode & 0xF000) >> 12)
    {
    case 0x0:
        display_->clear();
        break;
    case 0x1:
        s_.PC = (oppcode & 0x0FFF);
        break;
    case 0x6:
        s_.V[(oppcode & 0x0F00) >> 8] = oppcode & 0x00FF;
        break;
    case 0x7:
        s_.V[(oppcode & 0x0F00) >> 8] += oppcode & 0x00FF;
        break;
    case 0xA:
        s_.I = oppcode & 0x0FFF;
        break;
    case 0xD:
        drawSprite(s_.V[(oppcode & 0x0F00) >> 8],
                   s_.V[(oppcode & 0x00F0) >> 4],
                   oppcode & 0x000F);
        break;
    default:
        break;
    }

}

uint16_t Chip8Machine::readMemory(uint16_t addr) const
{
    return (memory_[addr] << 8) | memory_[addr+1];
}

void Chip8Machine::drawSprite(int x, int y, int height)
{
    for (int line=0; line<height; line++)
    {
        std::bitset<8> spriteLine(memory_[s_.I+line]);
        for (int pixel=0; pixel<8; pixel++)
            if (spriteLine[7-pixel])
                display_->flipPixel(x+pixel, y+line);
    }
}
