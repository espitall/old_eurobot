# Le compilateur
CC=gcc

# Les dossiers
SRCDIR=src
HEADDIR=src
LIBDIR=src
BINDIR=bin

# les options du compilateur
CFLAGS=-W -Wall -Wextra -Werror -std=gnu99
GLLIBS=-lSDL

# Nom de l'exécutable
BIN=simu2015

# Liste des fichiers
SRC= $(shell find $(SRCDIR) -type f -name '*.c')
OBJ= $(SRC:$(SRCDIR)/%.c=$(LIBDIR)/%.o)

# Règles
all: $(BIN)

# Création de l'exécutable
simu2015: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)

# Création des différents *.o à partir des *.c
$(LIBDIR)/%.o: $(SRCDIR)/%.c $(HEADDIR)/%.h
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: run clean mrproper

run: all
	$(BINDIR)/$(BIN)

clean:
	find $(SRCDIR) -type f -name '*~' -exec rm {} \;
	find $(HEADDIR) -type f -name '*~' -exec rm {} \;
	find $(LIBDIR) -type f -name '*.o' -exec rm {} \;

mrproper: clean
	rm -rf $(BINDIR)/$(BIN)
