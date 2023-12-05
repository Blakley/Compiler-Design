# Use the `make` command to make the project

# Running the program with the provided test files:
    1. good input programs: 
        
        ./P4 tests/generation/good/P4_gg<file number>

        example: 
            ./P4 tests/generation/good/P4_gg1
        or 
            ./P4 < tests/generation/good/P4_gg1.f23

    2. bad input programs:
        
        ./P4 tests/generation/bad/P4_gg<file number>

        example: 
            ./P4 tests/generation/bad/P4_gg3
        or 
            ./P4 < tests/generation/bad/P4_gg3.f23

# Verifying the assembly code:
    The output assembly will be generated and stored in parent directory

# Program structure
    The source files are stored in the "src" directory
    The header files are all stored in the "headers" directory

# Using Stack for temporary storage option