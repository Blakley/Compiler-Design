/*
    Name: Anthony Blakley
    Date: 12/11/2023
    Description: 
        This program generates assembly code for out input language's abstract syntax tree (AST).
        It takes input code, processes it, and outputs the corresponding assembly instructions
        The program includes a lexical analyzer, parser, and code generator components.
*/

# include "../headers/scanner.h"
# include "../headers/parser.h"
# include "../headers/semantics.h"
# include "../headers/generator.h"

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
    
    // get generator object
    Generator generator(tree.root_node());

    // output generated .asm code to a file
    generator.output(scanner.filename);

    return 0;
}