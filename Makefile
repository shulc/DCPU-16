CC=clang++
CFLAGS=-c -Wall -std=c++17 -g
LDFLAGS=
SOURCES=main.cpp view.cpp emulator.cpp disasembler.cpp asembler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTEBLE=dcpu16

all: $(SOURCES) $(EXECUTEBLE)

$(EXECUTEBLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTEBLE)
