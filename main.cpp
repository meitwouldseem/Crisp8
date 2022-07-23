#include <memory>
#include <SFML/Graphics.hpp>
#include "Display.h"
#include "Chip8Machine.h"

int main()
{
    auto display = std::make_shared<Display>(64, 32, 10);
    auto machine = std::make_shared<Chip8Machine>(display);
    machine->loadRom("IBM.ch8");

    while (display->isOpen())
    {
        display->process();
        machine->advance();
    }

    return 0;
}
