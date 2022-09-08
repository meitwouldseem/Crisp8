#include <sstream>
#include <iomanip>
#include "StateMonitor.h"
#include "Chip8Machine.h"
#include "UbuntuMono.h"

StateMonitor::StateMonitor(const std::shared_ptr<Chip8Machine>& machine)
    : machine_(machine)
    , text_("State Monitor", font_)
{
    if (!font_.loadFromMemory(UbuntuMono_R_ttf, UbuntuMono_R_ttf_len))
        throw std::runtime_error("Failed to load font");
}

void StateMonitor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(text_, states);
}

void StateMonitor::update()
{
    auto s = machine_->getState();

    std::stringstream status;
    status << "PC: " << std::hex << s.PC << " I: " << std::hex << s.I;
    status << "\nNext Instr: " << std::hex << machine_->readMemory(s.PC);

    for (int i=0; i<16; i++)
    {
        if (i%2 == 0)
            status << "\n";
        else
            status << " ";
        status << "V" << i << ": " << std::setw(3) << std::hex << static_cast<int>(s.V[i]);
    }

    text_.setString(status.str());
}
