// Snake.hpp
#ifndef SNAKE_HPP
#define SNAKE_HPP

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
    void draw(sf::RenderWindow& window);
    sf::Vector2f getHeadPosition() const;

    // Vérifications des collisions
    bool checkCollisionWithBody() const;
    bool checkCollisionWithWall(int windowSize) const;

private:
    int squareSize;
    sf::Vector2i direction;
    std::deque<SnakeSegment> segments;
};

#endif
