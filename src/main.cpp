/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        This program utilizes a recursive descent top-down 
        parser to analyze a given programming
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

    // get parser object
    Parser parser(scanner);

    // being top-down parsing
    parser.begin();

    return 0;
}