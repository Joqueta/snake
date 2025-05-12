#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

struct SnakeSegment {
    int x, y;
};

class Snake {
public:
    Snake(int squareSize);
    void move();
    void grow();
    void setDirection(int dx, int dy);
    bool checkCollision(int windowSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getHeadPosition() const;

private:
    int squareSize;                    // Déclaré en premier
    SnakeSegment direction;            // Déclaré en second
    std::deque<SnakeSegment> segments; // Déclaré en troisième
};
