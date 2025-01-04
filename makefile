CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O0 -g

OBJ = src/main.o

TARGET = main

.PHONY=all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY=clean
clean:
	rm -rf $(TARGET) $(OBJ)

.PHONY=run
run:
	./$(TARGET)