/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
        
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

    // being top-down parsing
    parser.begin();

    // print tree (Test)
    tree.traverse(tree.root_node(), 0);

    // get semantic object
    Semantics semantics(tree.root_node());

    // check semantics
    semantics.static_semantics();

    return 0;
}