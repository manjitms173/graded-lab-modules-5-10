# Makefile for Process Manager
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
TARGET = process_manager
SOURCE = process_manager.c

all: $(TARGET)

$(TARGET): $(SOURCE)
$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
@echo "Compilation successful!"

clean:
rm -f $(TARGET)
@echo "Cleaned build files"

run: $(TARGET)
./$(TARGET)

.PHONY: all clean run
