CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean

all: main

main: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $(LDFLAGS) $(LDLIBS) -o main

clean:
	rm -f main