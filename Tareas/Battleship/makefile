CC = gcc
CFLAGS = -Wall
RM = rm -f

default: all

all: Battleship

Battleship: main.c
	$(CC) $(CFLAGS) $^ -o Battleship

run: main.c
	$(CC) $(CFLAGS) $^ -o Battleship
	./Battleship

clean:
	$(RM) Battleship
	$(RM) -rf Game
