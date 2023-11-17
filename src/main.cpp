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
    // get parser object
    Parser parser;

    // start parser
    parser.parse(argc, argv);

    return 0;
}

/*


    * the BNF gets hard-coded into functions. Each nonterminal, 'A', 'B', 'C', etc..
    gets implemented as a function (look at P2 Suggesions filesuggesions file). Name each function the same as the
    nonterminal name: A(), B(), C(), etc ..), each one will generate a node with a label string based on the nonterminal name

    * We'll need to compute First sets whenever there are predictions needed
    (Look at P2 BNF variables File)

    * Building and outputting the tree (look at P2 Tree File):
        To define the node: note that it needs a label, could be string, and should be the same as the function name generating it
        should have any tokens processed in the function (only ids, integers, and relational operators and those from expressions need to be stored),
        and finally will have children for each nonterminal in a production 
        - I believe you hav eat most 4 nonterminals per production

    * We'll have a global variable "current_token" that will be used in each Nonterminal function

    * remove print outputs in scanner other than error outputs

*/