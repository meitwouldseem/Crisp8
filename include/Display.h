#ifndef DISPLAY_H
#define DISPLAY_H
#include <SFML/Graphics.hpp>

class Display
    : public sf::RenderWindow
{
    public:
        Display(int width, int height, int scaleFactor);
        //~Display() = default;

        void process();
        void setPixel(int x, int y, bool state);
        void flipPixel(int x, int y);
        void clearDisplay();

    private:
        size_t findIndex(int x, int y) const;
        void applyColorToPixel(size_t index, sf::Color color);

        sf::VertexArray vertices_;

        const int width_, height_, scaleFactor_;
};

#endif // DISPLAY_H
