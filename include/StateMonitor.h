#ifndef CHIP8STATEMONITOR_H
#define CHIP8STATEMONITOR_H

#include <memory>
#include <SFML/Graphics.hpp>

class Chip8Machine;

class StateMonitor
    : public sf::Drawable, public sf::Transformable
{
    public:
        StateMonitor(const std::shared_ptr<Chip8Machine>& machine);

        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    protected:

    private:
        std::shared_ptr<Chip8Machine> machine_;
        sf::Font font_;
        sf::Text text_;
};

#endif // CHIP8STATEMONITOR_H
