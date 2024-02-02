# Paramètres de compilation
NAME = scop
CC = g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++20
INCLUDES = -Iincludes -I/usr/include
LIBS = -L/usr/lib -lglfw -lGL -lm -lpthread -ldl
MAKEFLAGS = "-j 10"

# Répertoires et fichiers
SRCS_DIR = srcs
OBJS_DIR = objs
SRCS = $(wildcard $(SRCS_DIR)/*.cpp)
OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

# Règles
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(LIBS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(OBJS_DIR)
	$(CC) $< $(CPPFLAGS) $(INCLUDES) -c -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	@make all


.PHONY: all clean fclean re
