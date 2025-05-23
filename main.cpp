#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>

constexpr int CELL_SIZE = 20;
constexpr int GRID_WIDTH = 40;
constexpr int GRID_HEIGHT = 30;
constexpr int WINDOW_WIDTH = CELL_SIZE * GRID_WIDTH;
constexpr int WINDOW_HEIGHT = CELL_SIZE * GRID_HEIGHT;

enum class Direction { Up, Down, Left, Right };

int loadHighscore(const std::string& filename) {
    std::ifstream in(filename);
    int hs = 0;
    if (in >> hs) return hs;
    return 0;
}
void saveHighscore(const std::string& filename, int hs) {
    std::ofstream out(filename);
    if (out) out << hs;
}

sf::Vector2i randomPosition(std::mt19937 &rng, const std::deque<sf::Vector2i>& snake) {
    std::uniform_int_distribution<int> xDist(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<int> yDist(0, GRID_HEIGHT - 1);
    sf::Vector2i pos;
    do {
        pos = { xDist(rng), yDist(rng) };
    } while (std::find(snake.begin(), snake.end(), pos) != snake.end());
    return pos;
}

int main() {
restart:
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Snake SFML 3.0.0");
    window.setFramerateLimit(60);

    // --- AJOUT DES FONDS ---
    sf::Texture menuBgTexture, gameBgTexture, diffBgTexture;
    bool menuBgLoaded = menuBgTexture.loadFromFile("background_menu..png");
    bool gameBgLoaded = gameBgTexture.loadFromFile("background_game.png");
    bool diffBgLoaded = diffBgTexture.loadFromFile("background_menu..png");
    sf::Sprite menuBg(menuBgTexture);
    sf::Sprite gameBg(gameBgTexture);
    sf::Sprite diffBg(diffBgTexture);
    menuBg.setScale(sf::Vector2f(
        float(WINDOW_WIDTH) / menuBgTexture.getSize().x,
        float(WINDOW_HEIGHT) / menuBgTexture.getSize().y
    ));
    gameBg.setScale(sf::Vector2f(
        float(WINDOW_WIDTH) / gameBgTexture.getSize().x,
        float(WINDOW_HEIGHT) / gameBgTexture.getSize().y
    ));
    diffBg.setScale(sf::Vector2f(
        float(WINDOW_WIDTH) / diffBgTexture.getSize().x,
        float(WINDOW_HEIGHT) / diffBgTexture.getSize().y
    ));
    // ----------------------

    sf::Color bgColor = sf::Color(30, 30, 30);
    sf::Color snakeColor = sf::Color(60, 200, 70);
    sf::Color snakeHeadColor = sf::Color(30, 130, 60);
    sf::Color appleColor = sf::Color(230, 60, 60);

    std::random_device rd;
    std::mt19937 rng(rd());

    sf::Font font;
    if (!font.openFromFile("dogicapixel.ttf")) {
        std::cerr << "Erreur : police dogicapixel.ttf introuvable !" << std::endl;
    }

    const std::string HIGHSCORE_FILE = "highscore.txt";
    int highscore = loadHighscore(HIGHSCORE_FILE);

    // ----------- SONS & MUSIQUE -----------
    sf::Music backgroundMusic;
    sf::SoundBuffer eatBuffer, gameoverBuffer;

    if (!backgroundMusic.openFromFile("Game.wav")) {
        std::cerr << "Erreur musique d'ambiance !" << std::endl;
    }
    if (!eatBuffer.loadFromFile("food.wav")) {
        std::cerr << "Erreur food.wav !" << std::endl;
    }
    if (!gameoverBuffer.loadFromFile("GameOver.wav")) {
        std::cerr << "Erreur gameover.wav !" << std::endl;
    }

    sf::Sound eatSound(eatBuffer);
    sf::Sound gameoverSound(gameoverBuffer);

    // --------- SLIDER VOLUME MENU ---------
    float musicVolume = 100.f;
    sf::RectangleShape sliderBar(sf::Vector2f(200, 8));
    sliderBar.setFillColor(sf::Color(180, 180, 180));
    sliderBar.setPosition(sf::Vector2f((WINDOW_WIDTH-200)/2.f + 200, (WINDOW_HEIGHT-60)/2.f + 300));

    sf::CircleShape sliderKnob(12);
    sliderKnob.setFillColor(sf::Color(100,200,255));
    sliderKnob.setOrigin(sf::Vector2f(12.f, 12.f));
    sliderKnob.setPosition(sf::Vector2f(
        sliderBar.getPosition().x + (musicVolume/100.f)*200.f,
        sliderBar.getPosition().y + 4
    ));

    sf::Text volumeText(font, "Volume : 100%", 22);
    volumeText.setFillColor(sf::Color::White);
    volumeText.setPosition(sf::Vector2f(sliderBar.getPosition().x , sliderBar.getPosition().y - 40));

    bool draggingSlider = false;
    backgroundMusic.setVolume(musicVolume);
    backgroundMusic.play();

    // Menu principal
    bool startGame = false;
    bool quitGame = false;

    sf::RectangleShape playButton(sf::Vector2f(200, 60));
    playButton.setFillColor(sf::Color(100, 180, 100));
    playButton.setPosition(sf::Vector2f((WINDOW_WIDTH-200)/2.f, (WINDOW_HEIGHT-60)/2.f - 40));

    sf::Text playText(font, "Jouer", 32);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(sf::Vector2f(playButton.getPosition().x + 35.f, playButton.getPosition().y + 10.f));

    sf::RectangleShape quitButton(sf::Vector2f(200, 60));
    quitButton.setFillColor(sf::Color(180, 100, 100));
    quitButton.setPosition(sf::Vector2f((WINDOW_WIDTH-200)/2.f, (WINDOW_HEIGHT-60)/2.f + 50));

    sf::Text quitText(font, "Quitter", 32);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(sf::Vector2f(quitButton.getPosition().x + 15.f, quitButton.getPosition().y + 10.f));

    sf::Text title(font, "SNAKE", 64);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(sf::Vector2f(WINDOW_WIDTH/2.f - 140.f, 60.f));

    sf::Text highscoreTextMenu(font, "Record : " + std::to_string(highscore), 24);
    highscoreTextMenu.setFillColor(sf::Color::Cyan);
    highscoreTextMenu.setPosition(sf::Vector2f(WINDOW_WIDTH/2.f - 100.f, 145.f));

    // Menu principal loop
    while (window.isOpen() && !startGame && !quitGame) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        if (playButton.getGlobalBounds().contains(sf::Vector2f(mouse))) {
            playButton.setFillColor(sf::Color(130, 210, 130));
        } else {
            playButton.setFillColor(sf::Color(100, 180, 100));
        }
        if (quitButton.getGlobalBounds().contains(sf::Vector2f(mouse))) {
            quitButton.setFillColor(sf::Color(210, 130, 130));
        } else {
            quitButton.setFillColor(sf::Color(180, 100, 100));
        }

        //  SLIDER VOLUME 
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !draggingSlider) {
            if (sliderKnob.getGlobalBounds().contains(sf::Vector2f(mouse))) {
                draggingSlider = true;
            }
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            draggingSlider = false;
        }
        if (draggingSlider) {
            float x = std::clamp<float>(
                mouse.x,
                sliderBar.getPosition().x,
                sliderBar.getPosition().x + 200.f
            );
            sliderKnob.setPosition(sf::Vector2f(x, sliderBar.getPosition().y + 4));
            musicVolume = ((x - sliderBar.getPosition().x) / 200.f) * 100.f;
            backgroundMusic.setVolume(musicVolume);
        }
        volumeText.setString("Volume : " + std::to_string(int(musicVolume)) + "%");

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePos(float(mouseEvent->position.x), float(mouseEvent->position.y));
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (playButton.getGlobalBounds().contains(mousePos)) startGame = true;
                    if (quitButton.getGlobalBounds().contains(mousePos)) quitGame = true;
                    if (sliderKnob.getGlobalBounds().contains(mousePos)) draggingSlider = true;
                }
            }
        }

        window.clear();
        if (menuBgLoaded) window.draw(menuBg);

        window.draw(title);
        window.draw(highscoreTextMenu);
        window.draw(playButton); window.draw(playText);
        window.draw(quitButton); window.draw(quitText);
        window.draw(sliderBar); window.draw(sliderKnob); window.draw(volumeText);
        window.display();
    }
    if (quitGame) return 0;
    if (!startGame) goto restart;

    // === ECRAN CHOIX DIFFICULTE ===
    bool choixDiff = true;
    float snakeSpeed = 0.1f; // sera choisi
    float buttonWidth = 280;

    sf::RectangleShape facileButton(sf::Vector2f(buttonWidth, 60));
    sf::RectangleShape moyenButton(sf::Vector2f(buttonWidth, 60));
    sf::RectangleShape diffButton(sf::Vector2f(buttonWidth, 60));
    sf::RectangleShape retourButton(sf::Vector2f(buttonWidth, 50));
    facileButton.setFillColor(sf::Color(100, 180, 255));
    moyenButton.setFillColor(sf::Color(255, 200, 80));
    diffButton.setFillColor(sf::Color(255, 80, 80));
    retourButton.setFillColor(sf::Color(120, 120, 220));
    retourButton.setOutlineColor(sf::Color::White);
    retourButton.setOutlineThickness(2.f);

    facileButton.setPosition(sf::Vector2f((WINDOW_WIDTH-buttonWidth)/2.f, (WINDOW_HEIGHT-60)/2.f - 70));
    moyenButton.setPosition(sf::Vector2f((WINDOW_WIDTH-buttonWidth)/2.f, (WINDOW_HEIGHT-60)/2.f + 10));
    diffButton.setPosition(sf::Vector2f((WINDOW_WIDTH-buttonWidth)/2.f, (WINDOW_HEIGHT-60)/2.f + 90));
    retourButton.setPosition(sf::Vector2f((WINDOW_WIDTH-buttonWidth)/2.f, (WINDOW_HEIGHT-60)/2.f + 180));

    sf::Text facileText(font, "Facile", 32);
    sf::Text moyenText(font, "Moyen", 32);
    sf::Text diffText(font, "Difficile", 32);
    sf::Text retourText(font, "Retour", 28);

    facileText.setFillColor(sf::Color::White);
    moyenText.setFillColor(sf::Color::White);
    diffText.setFillColor(sf::Color::White);
    retourText.setFillColor(sf::Color::White);

    facileText.setPosition(sf::Vector2f(facileButton.getPosition().x + 70.f, facileButton.getPosition().y + 10.f));
    moyenText.setPosition(sf::Vector2f(moyenButton.getPosition().x + 75.f, moyenButton.getPosition().y + 10.f));
    diffText.setPosition(sf::Vector2f(diffButton.getPosition().x + 30.f, diffButton.getPosition().y + 10.f));
    retourText.setPosition(sf::Vector2f(retourButton.getPosition().x + 75.f, retourButton.getPosition().y + 10.f));

    sf::Text diffTitle(font, "Choisir le niveau", 38);
    diffTitle.setFillColor(sf::Color::Yellow);
    diffTitle.setPosition(sf::Vector2f(WINDOW_WIDTH/2.f - 260.f, 80.f));

    while (window.isOpen() && choixDiff) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        // Hover
        facileButton.setFillColor(facileButton.getGlobalBounds().contains(sf::Vector2f(mouse)) ? sf::Color(120,200,255) : sf::Color(100,180,255));
        moyenButton.setFillColor(moyenButton.getGlobalBounds().contains(sf::Vector2f(mouse)) ? sf::Color(255,220,120) : sf::Color(255,200,80));
        diffButton.setFillColor(diffButton.getGlobalBounds().contains(sf::Vector2f(mouse)) ? sf::Color(255,120,120) : sf::Color(255,80,80));
        retourButton.setFillColor(retourButton.getGlobalBounds().contains(sf::Vector2f(mouse)) ? sf::Color(140,140,255) : sf::Color(120,120,220));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePos(float(mouseEvent->position.x), float(mouseEvent->position.y));
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (facileButton.getGlobalBounds().contains(mousePos)) { snakeSpeed = 0.13f; choixDiff = false; }
                    if (moyenButton.getGlobalBounds().contains(mousePos)) { snakeSpeed = 0.08f; choixDiff = false; }
                    if (diffButton.getGlobalBounds().contains(mousePos)) { snakeSpeed = 0.045f; choixDiff = false; }
                    if (retourButton.getGlobalBounds().contains(mousePos)) goto restart;
                }
            }
        }

        window.clear();
        if (diffBgLoaded) window.draw(diffBg);

        window.draw(diffTitle);
        window.draw(facileButton); window.draw(facileText);
        window.draw(moyenButton); window.draw(moyenText);
        window.draw(diffButton); window.draw(diffText);
        window.draw(retourButton); window.draw(retourText);
        window.display();
    }
    if (choixDiff) goto restart;

    // DEBUT DU JEU le vrai du vrai

    std::deque<sf::Vector2i> snake = { {GRID_WIDTH/2, GRID_HEIGHT/2} };
    Direction dir = Direction::Right;
    Direction nextDir = dir;
    sf::Vector2i apple = randomPosition(rng, snake);

    sf::Clock moveClock;
    bool gameOver = false;
    int score = 0;
    static bool justLost = false;

    sf::Text scoreText(font, "", 16);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));

    sf::Text highscoreText(font, "", 16);
    highscoreText.setFillColor(sf::Color::Cyan);
    highscoreText.setPosition(sf::Vector2f(10.f, 40.f));

    sf::Text titleOver(font, "GAME OVER", 32);
    titleOver.setFillColor(sf::Color::White);
    titleOver.setOutlineColor(sf::Color::Black);
    titleOver.setOutlineThickness(2.f);

    sf::Text infoOver(font, "Espace pour rejouer", 22);
    infoOver.setFillColor(sf::Color::Yellow);

    sf::Text scoreOver(font, "", 22);
    scoreOver.setFillColor(sf::Color::Cyan);

    sf::Text recordOver(font, "", 22);
    recordOver.setFillColor(sf::Color::Green);

    sf::RectangleShape menuButton(sf::Vector2f(200, 50));
    menuButton.setFillColor(sf::Color(120, 120, 220));
    menuButton.setOutlineColor(sf::Color::White);
    menuButton.setOutlineThickness(2.f);

    sf::Text menuText(font, "Menu", 28);
    menuText.setFillColor(sf::Color::White);

    backgroundMusic.setVolume(musicVolume);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (!gameOver) {
                    if ((keyEvent->code == sf::Keyboard::Key::Up    || keyEvent->code == sf::Keyboard::Key::W) && dir != Direction::Down)
                        nextDir = Direction::Up;
                    else if ((keyEvent->code == sf::Keyboard::Key::Down  || keyEvent->code == sf::Keyboard::Key::S) && dir != Direction::Up)
                        nextDir = Direction::Down;
                    else if ((keyEvent->code == sf::Keyboard::Key::Left  || keyEvent->code == sf::Keyboard::Key::A) && dir != Direction::Right)
                        nextDir = Direction::Left;
                    else if ((keyEvent->code == sf::Keyboard::Key::Right || keyEvent->code == sf::Keyboard::Key::D) && dir != Direction::Left)
                        nextDir = Direction::Right;
                }
                if (gameOver && keyEvent->code == sf::Keyboard::Key::Space) {
                    snake = { {GRID_WIDTH/2, GRID_HEIGHT/2} };
                    dir = Direction::Right;
                    nextDir = dir;
                    apple = randomPosition(rng, snake);
                    gameOver = false;
                    score = 0;
                    moveClock.restart();
                    backgroundMusic.stop();
                    backgroundMusic.setVolume(musicVolume);
                    backgroundMusic.play();
                    justLost = false;
                }
            } else if (gameOver && event->is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePos(float(mouseEvent->position.x), float(mouseEvent->position.y));
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (menuButton.getGlobalBounds().contains(mousePos)) goto restart;
                }
            }
        }

        if (!gameOver && moveClock.getElapsedTime().asSeconds() >= snakeSpeed) {
            dir = nextDir;
            sf::Vector2i head = snake.front();
            switch (dir) {
                case Direction::Up:    head.y -= 1; break;
                case Direction::Down:  head.y += 1; break;
                case Direction::Left:  head.x -= 1; break;
                case Direction::Right: head.x += 1; break;
            }
            if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT)
                gameOver = true;
            for (const auto& segment : snake)
                if (segment == head)
                    gameOver = true;

            if (!gameOver) {
                snake.push_front(head);
                if (head == apple) {
                    score++;
                    apple = randomPosition(rng, snake);
                    eatSound.play();
                } else {
                    snake.pop_back();
                }
            }
            moveClock.restart();
            if (gameOver && !justLost) {
                backgroundMusic.stop();
                gameoverSound.play();
                justLost = true;
            }
            if (!gameOver) justLost = false;
        }

        // Boucle manuelle de la musique
        if (!gameOver && backgroundMusic.getStatus() == sf::SoundSource::Status::Stopped) {
            backgroundMusic.setVolume(musicVolume);
            backgroundMusic.play();
        }

        // ==== FOND JEU ====
        window.clear();
        if (gameBgLoaded) window.draw(gameBg);

        // DESSIN POMME RONDE 
        sf::CircleShape appleCircle(CELL_SIZE/2.f);
        appleCircle.setFillColor(appleColor);
        appleCircle.setOrigin(sf::Vector2f(CELL_SIZE/2.f, CELL_SIZE/2.f));
        appleCircle.setPosition(sf::Vector2f(apple.x * CELL_SIZE + CELL_SIZE/2.f, apple.y * CELL_SIZE + CELL_SIZE/2.f));
        window.draw(appleCircle);

        //  DESSIN SERPENT STYLE ROND + TETE DISTINCTE AVEC YEUX + LANGUE 
        for (size_t i = 0; i < snake.size(); ++i) {
            if (i == 0) {
                // Tête
                float headRadius = CELL_SIZE * 0.6f;
                sf::CircleShape head(headRadius);
                head.setFillColor(snakeHeadColor);
                head.setOrigin(sf::Vector2f(headRadius, headRadius));
                head.setPosition(sf::Vector2f(snake[i].x * CELL_SIZE + CELL_SIZE/2.f, snake[i].y * CELL_SIZE + CELL_SIZE/2.f));
                window.draw(head);

                // Dessin des yeux
                sf::CircleShape eye(CELL_SIZE * 0.13f);
                eye.setFillColor(sf::Color::Black);

                sf::Vector2f headPos = head.getPosition();
                // Placement des yeux selon la direction du serpent
                if (dir == Direction::Up) {
                    eye.setPosition(sf::Vector2f(headPos.x - CELL_SIZE*0.22f, headPos.y - CELL_SIZE*0.25f));
                    window.draw(eye);
                    eye.setPosition(sf::Vector2f(headPos.x + CELL_SIZE*0.15f, headPos.y - CELL_SIZE*0.25f));
                    window.draw(eye);
                } else if (dir == Direction::Down) {
                    eye.setPosition(sf::Vector2f(headPos.x - CELL_SIZE*0.22f, headPos.y + CELL_SIZE*0.10f));
                    window.draw(eye);
                    eye.setPosition(sf::Vector2f(headPos.x + CELL_SIZE*0.15f, headPos.y + CELL_SIZE*0.10f));
                    window.draw(eye);
                } else if (dir == Direction::Left) {
                    eye.setPosition(sf::Vector2f(headPos.x - CELL_SIZE*0.28f, headPos.y - CELL_SIZE*0.10f));
                    window.draw(eye);
                    eye.setPosition(sf::Vector2f(headPos.x - CELL_SIZE*0.28f, headPos.y + CELL_SIZE*0.13f));
                    window.draw(eye);
                } else if (dir == Direction::Right) {
                    eye.setPosition(sf::Vector2f(headPos.x + CELL_SIZE*0.13f, headPos.y - CELL_SIZE*0.10f));
                    window.draw(eye);
                    eye.setPosition(sf::Vector2f(headPos.x + CELL_SIZE*0.13f, headPos.y + CELL_SIZE*0.13f));
                    window.draw(eye);
                }
                // Langue
                sf::RectangleShape tongue(sf::Vector2f(CELL_SIZE * 0.18f, CELL_SIZE * 0.06f));
                tongue.setFillColor(sf::Color(230, 20, 60));
                tongue.setOrigin(sf::Vector2f(0, tongue.getSize().y / 2.f));
                if (dir == Direction::Up) {
                    tongue.setPosition(sf::Vector2f(headPos.x, headPos.y - headRadius));
                    tongue.setRotation(sf::degrees(0));
                } else if (dir == Direction::Down) {
                    tongue.setPosition(sf::Vector2f(headPos.x, headPos.y + headRadius));
                    tongue.setRotation(sf::degrees(180));
                } else if (dir == Direction::Left) {
                    tongue.setPosition(sf::Vector2f(headPos.x - headRadius, headPos.y));
                    tongue.setRotation(sf::degrees(270));
                } else if (dir == Direction::Right) {
                    tongue.setPosition(sf::Vector2f(headPos.x + headRadius, headPos.y));
                    tongue.setRotation(sf::degrees(90));
                }
                window.draw(tongue);
            } else {
                // Corps
                sf::CircleShape segment(CELL_SIZE/2.f);
                segment.setOrigin(sf::Vector2f(CELL_SIZE/2.f, CELL_SIZE/2.f));
                segment.setPosition(sf::Vector2f(snake[i].x * CELL_SIZE + CELL_SIZE/2.f, snake[i].y * CELL_SIZE + CELL_SIZE/2.f));
                segment.setFillColor(snakeColor);
                window.draw(segment);
            }
        }

        scoreText.setString("Score : " + std::to_string(score));
        highscoreText.setString("Record : " + std::to_string(highscore));
        window.draw(scoreText);
        window.draw(highscoreText);

        if (gameOver) {
            if (score > highscore) {
                highscore = score;
                saveHighscore(HIGHSCORE_FILE, highscore);
                highscoreTextMenu.setString("Record : " + std::to_string(highscore));
            }

            sf::RectangleShape overlay{sf::Vector2f(float(WINDOW_WIDTH), float(WINDOW_HEIGHT))};
            overlay.setFillColor(sf::Color(0, 0, 0, 160));
            window.draw(overlay);

            scoreOver.setString("Score : " + std::to_string(score));
            recordOver.setString("Record : " + std::to_string(highscore));
            float centerX = WINDOW_WIDTH / 2.f - 110.f;
            titleOver.setPosition(sf::Vector2f(centerX, WINDOW_HEIGHT / 2.f - 100.f));
            infoOver.setPosition(sf::Vector2f(centerX, WINDOW_HEIGHT / 2.f - 35.f));
            scoreOver.setPosition(sf::Vector2f(centerX, WINDOW_HEIGHT / 2.f + 25.f));
            recordOver.setPosition(sf::Vector2f(centerX, WINDOW_HEIGHT / 2.f + 60.f));
            window.draw(titleOver);
            window.draw(infoOver);
            window.draw(scoreOver);
            window.draw(recordOver);

            float menuX = WINDOW_WIDTH / 2.f - 100.f;
            float menuY = WINDOW_HEIGHT / 2.f + 110.f;
            menuButton.setPosition(sf::Vector2f(menuX, menuY));
            menuText.setPosition(sf::Vector2f(menuX + 45.f, menuY + 8.f));
            window.draw(menuButton);
            window.draw(menuText);
        }
        window.display();
    }
    return 0;
}
