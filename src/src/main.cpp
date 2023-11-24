/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
            This project performs static semantics checks on an Abstract Syntax Tree (AST), 
            ensuring proper variable definition and usage. The AST is generated from our 
            recursive decent parser which looks at the input source program.
*/

# include "../headers/scanner.h"
# include "../headers/parser.h"
# include "../headers/semantics.h"

/**
 * ------------------------------------------
 *              Entry function
 * ------------------------------------------
*/
int main(int argc, char** argv) {
    // get scanner object
    Scanner scanner;

    // handle input arguments
    scanner.arguments(argc, argv);

    // get tree object
    Tree tree;

    // get parser object
    Parser parser(scanner, tree);

    // begin program top-down parsing
    parser.begin();

    // get semantic object
    Semantics semantics(tree.root_node());

    // check program semantics
    semantics.static_semantics();

    return 0;
}

/*
    TODO (P4):

    Part 1. (Code generation)

        produce a target file:
            a) a.asm (if keyboard input)
            b) file.asm (if file was given as argument)


    Part 2. (Storage allocation)


*/