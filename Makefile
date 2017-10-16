CC=gcc
CFLAGS=-g -Wall -pedantic -std=c11 -O3

OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c))
HEADERS=$(wildcard *.h)

TARGET=robot

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET) 

run: clean all
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -lncurses -o $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
