CC=cc
CFLAGS=-I.
LIBS=-lpthread

snake: game.c queue.c
	$(CC) $(LIBS) -o $@ game.c queue.c
