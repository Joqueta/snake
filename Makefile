# Le compilateur
CXX = g++

# Les options de compilation pour SFML 3.0.0 sur macOS via Homebrew
CXXFLAGS = -Wall -Wextra -std=c++17 -I/opt/homebrew/Cellar/sfml/3.0.0_1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/3.0.0_1/lib -lsfml-graphics -lsfml-window -lsfml-system

# Répertoires
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Fichiers sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Fichiers objets
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Nom du programme
TARGET = Snake

# Règles
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
