/*
    Name: Anthony Blakley
    Date: 12/03/2023
    Description: 
            
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

    // output generated code to a file
    generator.output(scanner.filename);

    return 0;
}