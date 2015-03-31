CC=gcc
CFLAGS=-W -Wall -Wextra -Werror -std=gnu99
LDFLAGS=-lSDL
EXEC=simu
SRC= $(wildcard *.c)
SRC_H= $(wildcard *.h)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

simu: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: $(SRC_H)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper run

run: all
	@./$(EXEC)

clean:
	@rm -rf *.o *~

mrproper: clean
	@rm -rf $(EXEC)
