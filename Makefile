# define C compiler and flags
CC=gcc
CFLAGS=-std=gnu99 -Wall -c
LFLAGS=-std=gnu99 -Wall -o

SOURCES = main.c smallsh.c
OBJECTS = main.o smallsh.o
TARGET = smallsh

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean: 
	$(RM) $(TARGET)

.PHONY: valgrind

valgrind: 
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes ./$(TARGET)

.PHONY: run

run: 
	./$(TARGET)

# Source - https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html