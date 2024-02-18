NAME = scop
CC = g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++2a
INCLUDES = -Iincludes -I/usr/include -I/home/cjunker/Downloads/glew-2.1.0/include -I/home/cjunker/glm
LIBS = -L/usr/lib -L/home/cjunker/Downloads/glew-2.1.0/lib -lglfw -lGL -lm -lpthread -ldl -lGLEW

# Répertoires et fichiers
SRCS_DIR = srcs
OBJS_DIR = objs
SRCS = $(wildcard $(SRCS_DIR)/*.cpp)
OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

# Créer le répertoire objs si nécessaire
$(shell mkdir -p $(OBJS_DIR))

# Règles
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(LIBS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(INCLUDES) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_DIR)/*.o $(NAME)

re: clean all

.PHONY: all clean re
