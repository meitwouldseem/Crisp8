#include <memory>
#include "Display.h"
#include "StateMonitor.h"
#include "Chip8Machine.h"

int main(int argc, char** argv)
{
    int width = 64;
    int height = 32;
    int scaleFactor = 10;
    auto display = std::make_shared<Display>(width, height, scaleFactor);
    auto machine = std::make_shared<Chip8Machine>(display);
    auto monitor = std::make_shared<StateMonitor>(machine);
    monitor->setPosition(width*scaleFactor, 0);

    display->clearDisplay();

    sf::RenderWindow window(sf::VideoMode((width*scaleFactor)+300, height*scaleFactor), "Crisp8");
    window.setFramerateLimit(120);

    if (argc > 1)
        machine->loadRom(argv[1]);
    else
        machine->loadRom("test.ch8");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
//            else if (event.type == sf::Event::KeyPressed)
//            {
//                if (event.key.code == sf::Keyboard::Space)
//                    machine->advance();
//            }
        }

        window.clear(sf::Color::Blue);
        window.draw(*display);
        window.draw(*monitor);
        window.display();

        machine->advance();
        monitor->update();
    }

    return 0;
}
