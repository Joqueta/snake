#include "Food.hpp"
#include <cstdlib>

Food::Food(int squareSize, int windowSize) : squareSize(squareSize), windowSize(windowSize) {
    foodShape.setSize(sf::Vector2f(squareSize, squareSize));
    foodShape.setFillColor(sf::Color::Red);
    spawn();  // Générer la première nourriture
}

void Food::spawn() {
    int x = (std::rand() % (windowSize / squareSize)) * squareSize;
    int y = (std::rand() % (windowSize / squareSize)) * squareSize;
    foodShape.setPosition(sf::Vector2f(x, y));
}

void Food::draw(sf::RenderWindow& window) {
    window.draw(foodShape);
}

sf::Vector2f Food::getPosition() const {
    return foodShape.getPosition();
}
