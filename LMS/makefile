# I am Using variables here since they are scalable and better to maintain later
# Compiler
CC = gcc

# Executable name
TARGET = library

# Source files
SRCS = main.c lib.c issue.c

# Default rule
all: $(TARGET)

# How to build the executable
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)