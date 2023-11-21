/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        This project implements a recursive descent parser for a programming language
        The parser is designed to generate a syntax tree, representing the hierarchical structure of the input code
        The syntax tree is then traversed in a pre-order fashion, printing each node along with its associated tokens
*/

# include "headers/scanner.h"
# include "headers/parser.h"

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

    // being top-down parsing
    parser.begin();

    // print tree [preorder traversal]
    tree.traverse(tree.root_node(), 0);

    return 0;
}