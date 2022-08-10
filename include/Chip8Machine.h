#ifndef CHIP8MACHINE_H
#define CHIP8MACHINE_H
#include <cstdint>
#include <array>
#include <memory>
#include <stack>
#include "Display.h"

class Chip8Machine
{
    public:
        struct State
        {
            uint16_t PC;
            uint16_t I;
            std::array<uint8_t, 16> V;
        };

        Chip8Machine(const std::shared_ptr<Display>& display);

        void loadRom(std::string name);
        void advance();

    protected:

    private:
        void logicAndArithmeticOppcodes(uint16_t oppcode);
        void fPrefixOppcodes(uint16_t oppcode);
        uint16_t readMemory(uint16_t addr) const;
        void drawSprite(int x, int y, int height);

        State s_;
        std::shared_ptr<Display> display_;
        std::array<uint8_t, 4096> memory_;
        std::stack<uint16_t> stack_;
        const uint16_t programOffset_;
};

#endif // CHIP8MACHINE_H
