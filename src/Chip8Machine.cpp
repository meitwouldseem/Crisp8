#include <fstream>
#include <bitset>
#include <stdlib.h>
#include "Chip8Machine.h"

Chip8Machine::Chip8Machine(const std::shared_ptr<Display>& display)
    : display_(display)
    , programOffset_(0x200)
{
    s_.PC = programOffset_;
    s_.I = 0;
    for (int i=0; i<16; i++)
        s_.V[i] = 0;
    memory_.fill(0);
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
    fs.close();
}

void Chip8Machine::advance()
{
    uint16_t oppcode = readMemory(s_.PC);
    s_.PC += 2;

    switch ((oppcode & 0xF000) >> 12)
    {
    case 0x0:
        if (oppcode == 0x00E0)//Clear screen
            display_->clearDisplay();
        else if (oppcode == 0x00EE)//Return subroutine
        {
            s_.PC = stack_.top();
            stack_.pop();
        }
        break;
    case 0x1://Jump
        s_.PC = oppcode & 0x0FFF;
        break;
    case 0x2://Call subroutine
        stack_.push(s_.PC);
        s_.PC = (oppcode & 0x0FFF);
        break;
    case 0x3://Skip if Vx = NN
        if (s_.V[(oppcode & 0x0F00) >> 8] == (oppcode & 0x00FF))
            s_.PC += 2;
        break;
    case 0x4://Skip if Vx != NN
        if (s_.V[(oppcode & 0x0F00) >> 8] != (oppcode & 0x00FF))
            s_.PC += 2;
        break;
    case 0x5://Skip if Vx = Vy
        if (s_.V[(oppcode & 0x0F00) >> 8] == s_.V[(oppcode & 0x00F0) >> 4])
            s_.PC += 2;
        break;
    case 0x6://Set Vx to NN
        s_.V[(oppcode & 0x0F00) >> 8] = oppcode & 0x00FF;
        break;
    case 0x7://Add Vx to NN
        s_.V[(oppcode & 0x0F00) >> 8] += oppcode & 0x00FF;
        break;
    case 0x8:
        logicAndArithmeticOppcodes(oppcode);
        break;
    case 0x9://Skip if Vx != Vy
        if (s_.V[(oppcode & 0x0F00) >> 8] != s_.V[(oppcode & 0x00F0) >> 4])
            s_.PC += 2;
        break;
    case 0xA://Set index
        s_.I = oppcode & 0x0FFF;
        break;
    case 0xB://Jump with offset
        s_.PC = (oppcode & 0x0FFF) + s_.V[0];
        break;
    case 0xC://Get random number
        s_.V[(oppcode & 0x0F00) >> 8] = (rand() % (oppcode & 0x00FF)) & (oppcode & 0x00FF);
        break;
    case 0xD://Display
        drawSprite(s_.V[(oppcode & 0x0F00) >> 8],
                   s_.V[(oppcode & 0x00F0) >> 4],
                   oppcode & 0x000F);
        break;
    case 0xE://Skip if key

        break;
    case 0xF:
        fPrefixOppcodes(oppcode);
        break;
    default:
        break;
    }
}

const Chip8Machine::State& Chip8Machine::getState() const
{
    return s_;
}

uint16_t Chip8Machine::readMemory(uint16_t addr) const
{
    return (memory_[addr] << 8) | memory_[addr+1];
}

void Chip8Machine::logicAndArithmeticOppcodes(uint16_t oppcode)
{
    //0x8XYN
    const int x = (oppcode & 0x0F00) >> 8;
    const int y = (oppcode & 0x00F0) >> 4;
    switch (oppcode & 0x000F)
    {
    case 0x0:
        s_.V[x] = s_.V[y];
        break;
    case 0x1:
        s_.V[x] = s_.V[x] | s_.V[y];
        break;
    case 0x2:
        s_.V[x] = s_.V[x] & s_.V[y];
        break;
    case 0x3:
        s_.V[x] = s_.V[x] ^ s_.V[y];
        break;
    case 0x4:
        s_.V[x] = s_.V[x] + s_.V[y];
        //set carry flag on overflow
        break;
    case 0x5:
        s_.V[x] = s_.V[x] - s_.V[y];
        //can set carry flag
        break;
    case 0x6:
        s_.V[x] = s_.V[x] >> 1;
        //ambiguous instruction
        break;
    case 0x7:
        s_.V[x] = s_.V[y] - s_.V[x];
        //can set carry flag
        break;
    case 0xE:
        s_.V[x] = s_.V[x] << 1;
        //ambiguous instruction
        break;
    default:
        break;
    }
}

void Chip8Machine::fPrefixOppcodes(uint16_t oppcode)
{
    const int x = (oppcode & 0x0F00) >> 8;

    switch (oppcode & 0x00FF)
    {
    case 0x07://Set Vx to timer value

        break;
    case 0x0A://Get Key

        break;
    case 0x15://Set delay timer to Vx

        break;
    case 0x18://Set sound timer to Vx

        break;
    case 0x1E://Add to index
        s_.I += s_.V[x];
        break;
    case 0x29://Get font sprite

        break;
    case 0x33://Binary-coded decimal conversion
        {
            const int vx = s_.V[x];
            memory_[s_.I] = vx/100;
            memory_[s_.I+1] = (vx/10)%10;
            memory_[s_.I+2] = vx%10;
        }
        break;
    case 0x55://Store memory
        for (int i=0; i<=x; i++)
            memory_[s_.I+i] = s_.V[i];
        break;
    case 0x65://Load memory
        for (int i=0; i<=x; i++)
            s_.V[i] = memory_[s_.I+i];
        break;
    default:
        break;
    }
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
