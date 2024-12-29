# Compiler and flags
CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4 libsodium)
LIBS = $(shell pkg-config --libs gtk4 libsodium) -lsqlite3 

# Target executable and source file
TARGET = main
SRC = ./src/main.c ./src/access.c ./src/globals.c ./database/database.c ./security/safe.c

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
