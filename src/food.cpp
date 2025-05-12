#include "Food.hpp"
#include <cstdlib>
#include <ctime>

Food::Food(int windowWidth, int windowHeight, int squareSize) 
    : windowWidth(windowWidth), windowHeight(windowHeight), squareSize(squareSize) {
    shape.setSize(sf::Vector2f(squareSize, squareSize));
    shape.setFillColor(sf::Color::Red);
    generate();  // Génère la position initiale
}

void Food::generate() {
    int x = (std::rand() % (windowWidth / squareSize)) * squareSize;
    int y = (std::rand() % (windowHeight / squareSize)) * squareSize;
    shape.setPosition(sf::Vector2f(x, y));
}

void Food::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Food::getPosition() const {
    return shape.getPosition();
}
