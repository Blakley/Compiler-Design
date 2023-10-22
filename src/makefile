CC = g++
CFLAGS = -Wall -g

P1_SRC = P1.cpp
P1_EXEC = P1

# List of header files
HEADERS = P1.h

# build executables
all: $(P1_EXEC)

# P0 executable
$(P1_EXEC): $(P1_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(P1_EXEC) $(P1_SRC)

# clean up
clean:
	/bin/rm -f *.o $(P1_EXEC)

.PHONY: all clean