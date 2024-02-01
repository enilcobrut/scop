MAKEFLAGS = "-j 10"

NAME = scop

HEADER_DIR = includes
HEADER = $(HEADER_DIR)/scop.hpp

SRCS_DIR = srcs
SRCS = $(SRCS_DIR)/scop.cpp

OBJS_DIR = objs
OBJS = $(OBJS_DIR)/scop.o

INCLUDES = -I$(HEADER_DIR) -I/opt/homebrew/include -I/usr/local/include
LIBS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -lGLEW -framework OpenGL

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98# -g -fsanitize=address

all: $(NAME)

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -f $(NAME)

re:
	make fclean
	make all

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(HEADER)
	mkdir -p $(OBJS_DIR)
	$(CC) $< $(CPPFLAGS) $(INCLUDES) -c -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(LIBS)

.PHONY: all clean fclean re
