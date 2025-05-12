#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

const int windowWidth = 800;
const int windowHeight = 600;
const int cardSize = 100;
const int rows = 4;
const int cols = 4;
const int totalCards = rows * cols / 2;

class Card {
public:
    sf::RectangleShape shape;
    bool revealed = false;
    bool matched = false;
    int id;

    Card(int x, int y, int id) : id(id) {
        shape.setSize(sf::Vector2f(cardSize, cardSize));
        shape.setPosition(sf::Vector2f(x, y));
        shape.setFillColor(sf::Color::Blue);
    }

    void reveal() {
        revealed = true;
        shape.setFillColor(sf::Color::Green);
    }

    void hide() {
        revealed = false;
        shape.setFillColor(sf::Color::Blue);
    }

    void match() {
        matched = true;
        shape.setFillColor(sf::Color::Yellow);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Memory Game");
    std::vector<Card> cards;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize cards with random pairs
    int ids[rows * cols];
    for (int i = 0; i < totalCards; ++i) {
        ids[2 * i] = i;
        ids[2 * i + 1] = i;
    }
    std::shuffle(ids, ids + rows * cols, std::default_random_engine(std::random_device{}()));

    // Create card objects
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x = j * (cardSize + 10);
            int y = i * (cardSize + 10);
            cards.emplace_back(x, y, ids[i * cols + j]);
        }
    }

    while (window.isOpen()) {
        if (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        // Draw cards
        for (auto& card : cards) {
            window.draw(card.shape);
        }

        window.display();
    }

    return 0;
}
