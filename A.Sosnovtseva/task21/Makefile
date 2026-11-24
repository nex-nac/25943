CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean

all: main

main: t21.c
	$(CC) $(CPPFLAGS) $(CFLAGS) t21.c $(LDFLAGS) $(LDLIBS) -o main

clean:
	rm -f main