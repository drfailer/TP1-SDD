CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

prog: main.c util.c agenda.c tache.c agenda.h tache.h util.h
	$(CC) $(CFLAGS) -o $@ *.c -g

clean:
	rm prog
