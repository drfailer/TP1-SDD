CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

prog: main.c util.c
	$(CC) $(CFLAGS) -o $@ $^ -g

clean:
	rm prog
