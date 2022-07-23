#include "Display.h"

Display::Display(int width, int height, int scaleFactor)
    : RenderWindow(sf::VideoMode(width*scaleFactor, height*scaleFactor), "Crisp8")
    , vertices_(sf::Quads, 0)
    , width_(width)
    , height_(height)
    , scaleFactor_(scaleFactor)
{
    for (int y=0; y<height_; y++)
        for (int x=0; x<width_; x++)
        {
            int xoffset = x * scaleFactor_;
            int yoffset = y * scaleFactor_;
            vertices_.append(sf::Vertex(sf::Vector2f(xoffset,yoffset), sf::Color::Red));
            vertices_.append(sf::Vertex(sf::Vector2f(xoffset,yoffset+scaleFactor_), sf::Color::Blue));
            vertices_.append(sf::Vertex(sf::Vector2f(xoffset+scaleFactor_,yoffset+scaleFactor_), sf::Color::Green));
            vertices_.append(sf::Vertex(sf::Vector2f(xoffset+scaleFactor_,yoffset), sf::Color::Yellow));
        }
}

void Display::process()
{
    sf::Event event;
    while (pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            close();
    }

    RenderWindow::clear();
    draw(vertices_);
    display();
}

void Display::setPixel(int x, int y, bool state)
{
    size_t index = findIndex(x, y);
    if (index > vertices_.getVertexCount())
        return;
    applyColorToPixel(index, state ? sf::Color::White : sf::Color::Black);
}

void Display::flipPixel(int x, int y)
{
    setPixel(x, y, vertices_[findIndex(x, y)].color == sf::Color::Black);
}

void Display::clear()
{
    for (size_t i=0; i<vertices_.getVertexCount(); i++)
        vertices_[i].color = sf::Color::Black;
}

size_t Display::findIndex(int x, int y) const
{
    return (x+(y*width_))*4;
}

void Display::applyColorToPixel(size_t index, sf::Color color)
{
    for (int i=0; i<4; i++)
        vertices_[index+i].color = color;
}
