#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Snake.hpp"
#include "Food.hpp"

// Vérifie si la tête du serpent est sur la nourriture
bool isCollisionWithFood(sf::Vector2f headPos, sf::Vector2f foodPos) {
    return (headPos == foodPos);  // Vérifie si les positions sont identiques
}



// Vérifie si la tête du serpent touche un mur
bool isCollisionWithWall(sf::Vector2f headPos, int windowSize) {
    return (headPos.x < 0 || headPos.x >= windowSize || headPos.y < 0 || headPos.y >= windowSize);
    std::cout << "Vous avez heurté le mur";
}

const int windowSize = 800;
const int gridSize = 20;

int main() {
    // Initialisation de la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowSize, windowSize)), "Snake Game");


    // Initialisation du serpent et de la nourriture
    Snake snake(gridSize);
    Food food(gridSize, windowSize);

    // Boucle principale du jeu
    while (window.isOpen()) {
        // Gestion des événements
        if (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
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


        // Déplacer le serpent
        snake.move();

       if (isCollisionWithFood(snake.getHeadPosition(), food.getPosition())) {
            snake.grow();
            food.spawn();
            std::cout << "Nourriture mangée!" << std::endl;

}

        // Vérifier les collisions avec les murs ou le corps
        if (isCollisionWithWall(snake.getHeadPosition(), windowSize) || snake.checkCollisionWithBody()) {
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

    return 0;
}
