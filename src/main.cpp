/*
    Name: Anthony Blakley
    Date: 11/16/2023
    Description: 
        
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
    
    // test scanner
    scanner.tester();

    // get parser object
    Parser parser;

    // start parser
    parser.parse();

    return 0;
}
