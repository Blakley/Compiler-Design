CC = g++ -std=c++11
CFLAGS = -Wall -g

MAIN_SRC = main.cpp
PARSER_SRC = parser.cpp
SCANNER_SRC = scanner.cpp
EXE = P2

# List of header files
HEADERS = headers/token.h headers/scanner.h headers/parser.h headers/node.h

# Build executable
all: $(EXE)

# Executable
$(EXE): $(MAIN_SRC) $(PARSER_SRC) $(SCANNER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(MAIN_SRC) $(PARSER_SRC) $(SCANNER_SRC)

# Clean up
clean:
	/bin/rm -f *.o $(EXE)

.PHONY: all clean
