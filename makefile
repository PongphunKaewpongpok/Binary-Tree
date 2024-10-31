# Compiler and flags
CC = gcc
CFLAGS = -Wall -I"Raylib/src"
LDFLAGS = -L"Raylib/src" -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32

# Files
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = main.exe

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Rule to compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)