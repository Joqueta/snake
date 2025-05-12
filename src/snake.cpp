#include "Snake.hpp"

Snake::Snake(int squareSize) : squareSize(squareSize), direction{1, 0} {
    // Initialisation du serpent au centre de la fenêtre
    segments.push_front({400, 400});
}

void Snake::move() {
    SnakeSegment newHead = {segments.front().x + direction.x * squareSize, segments.front().y + direction.y * squareSize};
    segments.push_front(newHead);
    segments.pop_back();
}

void Snake::grow() {
    segments.push_back(segments.back());
}

void Snake::setDirection(int dx, int dy) {
    direction = {dx, dy};
}

bool Snake::checkCollision(int windowSize) {
    const SnakeSegment& head = segments.front();
    if (head.x < 0 || head.x >= windowSize || head.y < 0 || head.y >= windowSize)
        return true;
    for (size_t i = 1; i < segments.size(); ++i) {
        if (segments[i].x == head.x && segments[i].y == head.y)
            return true;
    }
    return false;
}

void Snake::draw(sf::RenderWindow& window) {
    for (const auto& segment : segments) {
        sf::RectangleShape rectangle(sf::Vector2f(squareSize - 1, squareSize - 1));
        rectangle.setPosition(sf::Vector2f(segment.x, segment.y));
        rectangle.setFillColor(sf::Color::Green);
        window.draw(rectangle);
    }
}

sf::Vector2f Snake::getHeadPosition() const {
    return sf::Vector2f(segments.front().x, segments.front().y);
}
