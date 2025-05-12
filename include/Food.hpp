#pragma once
#include <SFML/Graphics.hpp>

class Food {
public:
    Food(int squareSize, int windowSize);
    void spawn();
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape foodShape;
    int squareSize;
    int windowSize;
};
