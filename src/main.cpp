#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <optional>

const int windowSize = 800;
const int gridSize = 20;


struct SnakeSegment {
    int x, y;
};

class Snake {
public:
    Snake() {
        segments.push_front({windowSize / 2, windowSize / 2});
        direction = {0, -gridSize};
    }

    void move() {
        SnakeSegment newHead = {segments.front().x + direction.x, segments.front().y + direction.y};
        segments.push_front(newHead);
        segments.pop_back();
    }

    void grow() {
        segments.push_back(segments.back());
    }

    void setDirection(int dx, int dy) {
        direction = {dx * gridSize, dy * gridSize};
    }

    bool checkCollision() {
        const SnakeSegment& head = segments.front();
        if (head.x < 0 || head.x >= windowSize || head.y < 0 || head.y >= windowSize)
            return true;
        for (size_t i = 1; i < segments.size(); ++i) {
            if (segments[i].x == head.x && segments[i].y == head.y)
                return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& segment : segments) {
            sf::RectangleShape rectangle(sf::Vector2f(gridSize - 1, gridSize - 1));
            rectangle.setPosition(sf::Vector2f(segment.x, segment.y));
            rectangle.setFillColor(sf::Color::Green);
            window.draw(rectangle);
        }
    }

private:
    std::deque<SnakeSegment> segments;
    SnakeSegment direction;
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowSize, windowSize)), "Snake Game");
    Snake snake;

    while (window.isOpen()) {
        if (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (event-> is<sf::Event::KeyPressed>()) {
                const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    if (keyEvent->code == sf::Keyboard::Key::Up) snake.setDirection(0, -1);
                    if (keyEvent->code == sf::Keyboard::Key::Down) snake.setDirection(0, 1);
                    if (keyEvent->code == sf::Keyboard::Key::Left) snake.setDirection(-1, 0);
                    if (keyEvent->code == sf::Keyboard::Key::Right) snake.setDirection(1, 0);
                            
            }
        }
        }

        snake.move();
        if (snake.checkCollision()) {
            window.close();
        }

        window.clear();
        snake.draw(window);
        window.display();

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
