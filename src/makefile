CC = g++ -std=c++11
CFLAGS = -Wall -g

MAIN_SRC = src/main.cpp
TREE_SRC = src/tree.cpp
PARSER_SRC = src/parser.cpp
SCANNER_SRC = src/scanner.cpp
SEMANTICS_SRC = src/semantics.cpp
GENERATOR_SRC = src/generator.cpp
EXE = P4

# List of header files
HEADERS = headers/token.h headers/tree.h headers/scanner.h headers/parser.h headers/semantics.h headers/generator.h

# Build executable
all: $(EXE)

# Executable
$(EXE): $(MAIN_SRC) $(TREE_SRC) $(PARSER_SRC) $(SCANNER_SRC) $(SEMANTICS_SRC) $(GENERATOR_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(MAIN_SRC) $(TREE_SRC) $(PARSER_SRC) $(SCANNER_SRC) $(SEMANTICS_SRC) $(GENERATOR_SRC)

# Clean up
clean:
	/bin/rm -f *.o $(EXE)

.PHONY: all clean
