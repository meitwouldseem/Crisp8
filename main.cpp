#include <memory>
#include "Display.h"
#include "Chip8Machine.h"

int main(int argc, char** argv)
{
    auto display = std::make_shared<Display>(64, 32, 10);
    auto machine = std::make_shared<Chip8Machine>(display);
    display->setFramerateLimit(120);
    display->clearDisplay();

    if (argc > 1)
        machine->loadRom(argv[1]);
    else
        machine->loadRom("test.ch8");

    while (display->isOpen())
    {
        display->process();
        machine->advance();
    }

    return 0;
}
