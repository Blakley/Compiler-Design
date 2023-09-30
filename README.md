# Project 0

## Overview

This project is a practice exercise in implementing binary search trees (BSTs) and working with various tree traversals, command line arguments, and file I/O in C++. The program allows you to create and manipulate binary search trees, perform traversals, and save traversal results to files.

## Table of Contents

- [Usage](#usage)
- [File Structure](#file-structure)

## Usage

### Prerequisites

Before using this program, make sure you have:

- A C++ compiler (e.g., g++)

### Compilation

Compile the program using the provided Makefile:

```bash
make
```

### Running the Program

You can run the program with different command line arguments:

__Case 1__: No Arguments
When no arguments are provided, the program reads data from the keyboard until you signal the end of input (e.g., press Ctrl + D on Unix-based systems). In this mode, the data will be temporarily stored in a temporary file

```bash
./P0
```

__Case 2__: Input File
If you provide a single argument (the name of an input file), the program will treat it as the input file name. The program will append the required extension (".f23") to the filename and open it for reading.

```bash
./P0 inputfile
```

__Case 3__: Input Redirection
You can also use input redirection to provide data from a file:
```bash
./P0 < inputfile
```

### Output
The program performs various tree traversals (inorder, preorder, and postorder) and saves the results to files. The output files will be named based on the input file or the default "out" prefix:

* file_name.preorder
* file_name.inorder
* file_name.postorder

__Or, if no input file is provided__:

* out.preorder
* out.inorder
* out.postorder


## File Structure

* P0.cpp: Main program file.
* tree.cpp: Binary search tree implementation.
* tree.h: Header file for the binary search tree.
* node.h: Header file defining the structure of tree nodes.
* Makefile: Makefile for compiling the program.