#include "Snake.hpp"
#include <SFML/Graphics.hpp>

Snake::Snake(int squareSize) : squareSize(squareSize), direction{1, 0} {
    // Initialisation du serpent au centre de la fenêtre
    segments.push_front({400, 400});
}

void Snake::move() {
    // Calcul de la nouvelle position de la tête
    SnakeSegment newHead = {segments.front().x + direction.x * squareSize, segments.front().y + direction.y * squareSize};
    // Ajouter la nouvelle tête au début de la liste
    segments.push_front(newHead);
    // Supprimer le dernier segment pour maintenir la longueur
    segments.pop_back();
}

void Snake::grow() {
    // Ajouter un nouveau segment à la fin (même position que l'actuel dernier segment)
    segments.push_back(segments.back());
}

void Snake::setDirection(int dx, int dy) {
    // Vérifier que le serpent ne peut pas faire demi-tour directement
    if (direction.x != -dx && direction.y != -dy) {
        direction = {dx, dy};
    }
}

bool Snake::checkCollisionWithBody() const {
    // Vérifie si la tête touche un autre segment du corps
    const SnakeSegment& head = segments.front();
    for (size_t i = 1; i < segments.size(); ++i) {
        if (segments[i].x == head.x && segments[i].y == head.y) {
            return true;
        }
    }
    return false;
}

bool Snake::checkCollisionWithWall(int windowSize) const {
    const SnakeSegment& head = segments.front();
    // Vérifie si la tête touche un mur (hors de la fenêtre)
    return (head.x < 0 || head.x >= windowSize || head.y < 0 || head.y >= windowSize);
}

void Snake::draw(sf::RenderWindow& window) {
    // Dessiner chaque segment du serpent
    for (const auto& segment : segments) {
        sf::RectangleShape rectangle(sf::Vector2f(squareSize - 1, squareSize - 1));
        rectangle.setPosition(sf::Vector2f(segment.x, segment.y));
        rectangle.setFillColor(sf::Color::Green);
        window.draw(rectangle);
    }
}

sf::Vector2f Snake::getHeadPosition() const {
    // Retourne la position de la tête en tant que vecteur 2D
    return sf::Vector2f(segments.front().x, segments.front().y);
}
