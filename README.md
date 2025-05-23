🐍 Snake en C++ Ce projet est une implémentation du jeu classique Snake en C++, utilisant la bibliothèque SFML 3.0.* pour la gestion des graphismes, de la fenêtre et des événements.

📦 Prérequis Avant de compiler ou exécuter le projet, assurez-vous d'avoir installé les éléments suivants :

SFML 3.0.0 (Simple and Fast Multimedia Library)
Un compilateur C++ compatible avec la norme C++17 (par exemple g++)
Un système compatible (Linux, macOS, ou Windows via WSL ou MinGW)
💡 Installation de SFML sur macOS (via Homebrew): bash:

brew install sfml

⚙️ Compilation: Compilez le jeu avec la commande suivante:

g++ main.cpp -o snake -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

▶️ Exécution: Une fois la compilation terminée, lancez le jeu avec:

./snake

🎮 Contrôles: ↑, ↓, ←, → : Déplacer le serpent Espace : Démarrer ou relancer une partie Échap : Quitter le jeu

✍️ Auteur: MOITA Enzo / FROIS Fritzi
