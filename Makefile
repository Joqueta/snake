# Variables de compilation
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I./include
LDFLAGS = -L./lib/macos -lFLAC -lsfml-graphics -lvorbis -lfreetype -lsfml-network -lvorbisenc -logg -lsfml-system -lvorbisfile -lsfml-audio -lsfml-window -framework OpenGL -framework AppKit -framework IOKit -framework CoreServices -framework Carbon -framework OpenAL

# Répertoires
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Nom de l'exécutable
TARGET = Snake

# Règle par défaut
all: $(TARGET)

# Règle pour l'exécutable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Règle générique pour la compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des objets et de l'exécutable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Nettoyage complet
distclean: clean
	rm -f $(TARGET)
