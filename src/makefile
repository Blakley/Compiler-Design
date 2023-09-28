CC = g++
CFLAGS = -Wall -g

P0_SRC = P0.cpp
P0_EXEC = P0

TREE_SRC = tree.cpp

# List of header files
HEADERS = node.h tree.h

# build executables
all: $(P0_EXEC)

# P0 executable
$(P0_EXEC): $(P0_SRC) $(TREE_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(P0_EXEC) $(P0_SRC) $(TREE_SRC)

# clean up
clean:
	/bin/rm -f *.o $(P0_EXEC)

.PHONY: all clean