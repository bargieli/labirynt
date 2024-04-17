CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99

# Lista plików źródłowych
SOURCES=main.c sciezka.c numeracja.c
# Lista plików nagłówkowych
HEADERS=sciezka.h numeracja.h

# Nazwa pliku wykonywalnego
EXECUTABLE=a.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

clean:
	rm -f $(EXECUTABLE)

