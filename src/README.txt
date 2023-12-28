# Use the `make` command to make the project

# Running the program with the provided test files (7 files from Projects View: P4)
    Input program: 
        ./comp tests/p4_<file number>

        example: 
            ./comp tests/p4_1
        or 
            ./comp < tests/p4_1.f23
    
    Note: In the provided p4_4.23 and p4_6.f23 files, there is semantic error.
    Namely, the variable "x" doesn't exist. (wasn't sure if this was a typo)

# Verifying the assembly code
    The output assembly will be generated and stored in the parent directory

# Program structure
    The source files are stored in the "src" directory
    The header files are all stored in the "headers" directory

# Program clean up
	Note, when using `make clean` command, the previously
	generated code, .asm files, will also be removed.

# Stack
    This project has been extended to use stack storage for temporary variables
    thus, only one temporary variable is ever used while several stack variables
    are used depending on the program. 
    For example, running the program with an input: 
       "xout << 2 * ( 5 + ( 2 * 5 ) ) / ( 1 + 1 ) + 15 ;"
    will have multiple stack variables.
