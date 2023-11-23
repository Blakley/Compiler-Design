# Use the `make` command to make the project

# Running the program with the provided test files:
    1. good input programs: (3 total from project notes)
        
        ./P2 tests/semantics/good/P3_gg<file number>

        example: 
            ./P2 tests/semantics/good/P3_gg1
        or 
            /P2 < tests/semantics/good/P3_gg1.f23

    2. bad input programs: (3 total from project notes)
        
        ./P2 tests/semantics/bad/P3_gg<file number>

        example: 
            ./P2 tests/semantics/bad/P3_gg3
        or 
            /P2 < tests/semantics/bad/P3_gg3.f23

# Program structure
    The source files are stored in the "src" directory
    The header files are all stored in the "headers" directory

# Global option for all variables
    * There is only one scope, the global scope, regardless of where a variable is defined.