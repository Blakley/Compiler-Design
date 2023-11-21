CC = g++ -std=c++11
CFLAGS = -Wall -g

MAIN_SRC = src/main.cpp
TREE_SRC = src/tree.cpp
PARSER_SRC = src/parser.cpp
SCANNER_SRC = src/scanner.cpp
EXE = P2

# List of header files
HEADERS = headers/token.h headers/tree.h headers/scanner.h headers/parser.h

# Build executable
all: $(EXE)

# Executable
$(EXE): $(MAIN_SRC) $(TREE_SRC) $(PARSER_SRC) $(SCANNER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(MAIN_SRC) $(TREE_SRC) $(PARSER_SRC) $(SCANNER_SRC)

# Clean up
clean:
	/bin/rm -f *.o $(EXE)

.PHONY: all clean