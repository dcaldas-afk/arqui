CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
SRC = src/*.c

all: 
	$(CC) $(CFLAGS) main.c $(SRC) -o bin/poxim

test:
	$(CC) $(CFLAGS) test.c $(SRC) -o bin/test

clean:
	rm bin/poxim

re: clean all

debug: 
	$(CC) main.c $(SRC) -o bin/debug

.PHONY: all test clean re debug