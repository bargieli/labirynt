# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SRCS = numeracja.c sciezka.c
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = numeracja.h

# Executable name
EXEC = program

# Build target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Compile .c files to .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(EXEC) $(OBJS)

