CC = gcc
OBJ = $(SRC:.c=.o)
RM = rm -f
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-format
BETTY = betty

SRC0 = shell.c get_path.c helper_functions.c main_helper_functions.c
OBJ0 = $(SRC0:.c=.o)
NAME0 = shell

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

0:
	$(BETTY) $(SRC0)
	$(CC) $(CFLAGS) $(SRC0) -o $(NAME0)

test:
	$(CC) $(SRC0) -o $(NAME0)
