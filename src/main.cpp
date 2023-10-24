/*
    Name: Anthony Blakley
    Date: 10/24/2023
    Description: 
        Implementation of a lexical scanner. The scanner is responsible 
        for tokenizing input text, recognizing various types of tokens 
        and organizing them into a list of tokens for further processing.
        *[ Option #2: FSA Implementation ]
*/

# include "scanner.h"

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

    return 0;
}
