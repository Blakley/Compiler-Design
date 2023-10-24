CC = g++ -std=c++11
CFLAGS = -Wall -g

MAIN_SRC = main.cpp
SCANNER_SRC = scanner.cpp
SCANNER_EXE = P1

# List of header files
HEADERS = token.h scanner.h

# build executables
all: $(SCANNER_EXE)

# P0 executable
$(SCANNER_EXE): $(SCANNER_SRC) $(MAIN_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(SCANNER_EXE) $(SCANNER_SRC) $(MAIN_SRC)

# clean up
clean:
	/bin/rm -f *.o $(SCANNER_EXE)

.PHONY: all clean
