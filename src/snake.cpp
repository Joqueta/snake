#include "Snake.hpp"

Snake::Snake(int squareSize) : squareSize(squareSize), dx(1), dy(0) {
    sf::RectangleShape segment(sf::Vector2f(squareSize, squareSize));
    segment.setFillColor(sf::Color::Green);
    segment.setPosition(100, 100);
    body.push_back(segment);
}

void Snake::move() {
    for (int i = body.size() - 1; i > 0; --i) {
        body[i].setPosition(body[i - 1].getPosition());
    }
    body[0].move(dx * squareSize, dy * squareSize);
}

void Snake::grow() {
    sf::RectangleShape newSegment(sf::Vector2f(squareSize, squareSize));
    newSegment.setFillColor(sf::Color::Green);
    newSegment.setPosition(body.back().getPosition());
    body.push_back(newSegment);
}

void Snake::setDirection(int dx, int dy) {
    this->dx = dx;
    this->dy = dy;
}

void Snake::draw(sf::RenderWindow& window) {
    for (const auto& segment : body) {
        window.draw(segment);
    }
}

sf::Vector2f Snake::getHeadPosition() const {
    return body[0].getPosition();
}
