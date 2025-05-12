#ifndef FOOD_HPP
#define FOOD_HPP

#include <SFML/Graphics.hpp>

class Food {
public:
    Food(int windowWidth, int windowHeight, int squareSize);
    void generate();
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape shape;
    int windowWidth;
    int windowHeight;
    int squareSize;
};

#endif
