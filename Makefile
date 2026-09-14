CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

NAME = codexion

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=src/%.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c src/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

f: clean
	$(RM) $(NAME)

re: fclean all