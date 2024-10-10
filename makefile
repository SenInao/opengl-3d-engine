# Compiler and flags
CC = gcc
CFLAGS = -I$(IDIR)
LDFLAGS = -lSDL2 -lSDL2main -lopengl32 -lgdi32 -luser32 -lglew32

# Directories
SRCDIR = ./src/
IDIR = ./include/

# Source files
SOURCES = $(wildcard $(SRCDIR)*.c)

# Executable name
EXEC = 3d-engine

# Build rules
all: $(EXEC)

# Link the executable
$(EXEC):
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Run the executable
run:
	./$(EXEC)

# Clean up
clean:
	rm -f $(EXEC)
