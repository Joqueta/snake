#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Snake.hpp"
#include "Food.hpp"

const int windowSize = 800;
const int gridSize = 20;

int main() {
    // Initialisation de la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowSize, windowSize)), "Snake Game");


    // Initialisation du serpent et de la nourriture
    Snake snake(gridSize);
    Food food(gridSize, windowSize);
    while (window.isOpen()) {

 //  sf::Event event;
while (auto optEvent = window.pollEvent()) {
const sf::Event& event = *optEvent;

    std::cout << "Event" << std::endl;
    if (event.is<sf::Event::Closed>()) {
        window.close();
    } else if (event.is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent) {
            std::cout << "Touche appuyée" << std::endl;
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                window.close();
            } else if (keyEvent->code == sf::Keyboard::Key::Up) {
                snake.setDirection(0, -1);
            } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                snake.setDirection(0, 1);
            } else if (keyEvent->code == sf::Keyboard::Key::Left) {
                snake.setDirection(-1, 0);
            } else if (keyEvent->code == sf::Keyboard::Key::Right) {
                snake.setDirection(1, 0);
            }
        }
}
        // Vérifier si le serpent a mangé la nourriture
        if (snake.getHeadPosition() == food.getPosition()) {
            snake.grow();
            food.spawn();
        }

        // Vérifier la collision avec les murs ou le corps du serpent
        if (snake.checkCollision(windowSize)) {
            std::cout << "Game Over!" << std::endl;
            window.close();
        }

        // Rendu graphique
        window.clear(sf::Color::Black);
        snake.draw(window);   // Dessiner le serpent
        food.draw(window);    // Dessiner la nourriture
        window.display();

        // Contrôle de la vitesse du serpent
        sf::sleep(sf::milliseconds(100));
    }
    }
    return 0;
}
