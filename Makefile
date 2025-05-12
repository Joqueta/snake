# Le compilateur
CXX = g++

# Les options de compilation pour SFML 3.0.0 sur macOS via Homebrew
CXXFLAGS = -Wall -Wextra -std=c++17 -I/opt/homebrew/Cellar/sfml/3.0.0_1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/3.0.0_1/lib -lsfml-graphics -lsfml-window -lsfml-system

# Répertoires
.PHONY = all, clean, fclean, re

ifeq ($(OS),Windows_NT)
    NAME = Snake.exe
	CC	 = g++
    LIB_PATH = -L./lib/windows \
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
        -lopengl32 -lwinmm -lgdi32
else
    NAME = Snake
	CC	 = clang++
    LIB_PATH = -L./lib/macos -lFLAC 						\
		-lsfml-graphics -lvorbis 					\
		-lfreetype -lsfml-network 					\
		-lvorbisenc -logg -lsfml-system 			\
		-lvorbisfile -lsfml-audio -lsfml-window 	\
		-framework OpenGL -framework AppKit 		\
		-framework IOKit -framework CoreServices 	\
		-framework Carbon -framework OpenAL

endif

CFLAGS = -Wall -Wextra -Werror -std=c++17

INC = -I./include

SRC	=	src/main.cpp			

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIB_PATH) $(CFLAGS) $(INC)

%.o: %.cpp
	$(CC) -o $@ -c $< $(INC) $(CFLAGS)

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean re fclean