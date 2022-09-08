#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>

class Display
    : public sf::Drawable, public sf::Transformable
{
    public:
        Display(int width, int height, int scaleFactor);
        //~Display() = default;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void setPixel(int x, int y, bool state);
        void flipPixel(int x, int y);
        void clearDisplay();
        sf::FloatRect getBounds() const;

    private:
        size_t findIndex(int x, int y) const;
        void applyColorToPixel(size_t index, sf::Color color);

        sf::VertexArray vertices_;

        const int width_, height_, scaleFactor_;
};

#endif // DISPLAY_H
