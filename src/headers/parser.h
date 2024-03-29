/*
    Name: Anthony Blakley
    Date: 12/11/2023
    Description: 
        Header file for recursive descent top-down parser
*/

# ifndef PARSER_H
# define PARSER_H

# include "tree.h"
# include "token.h"
# include "scanner.h"

// Recursive descent top-down parser class
class Parser {
    private:
        token _token;     // Reference to the current token
        Scanner& scanner; // Scanner reference
        Tree& tree;       // Tree reference

        std::vector<std::string> _relationals; // relational operators of statement
        std::vector<std::string> _operations;  // values & operators of expression

    public:
        ~Parser();                          // Destructor
        Parser(Scanner& scanner, Tree& t);  // Constructor
        
        void parse_program();       // Parse the main program
        void parse_varList();       // Parse variable list
        void parse_vars();          // Parse variable declarations
        
        void parse_exp();           // Parse expression
        void parse_M();             // Parse additive expression
        void parse_N();             // Parse multiplicative expression
        void parse_R();             // Parse primary expression
        
        void parse_stat();          // Parse statement
        void parse_stats();         // Parse statements
        void parse_mStat();         // Parse multiple statements
        void parse_block();         // Parse code block
        
        void parse_in();            // Parse input statement
        void parse_out();           // Parse output statement

        void parse_if();            // Parse if statement
        void parse_loop();          // Parse loop statement
        
        void parse_RO();            // Parse relational operator
        void parse_assign();        // Parse assignment statement

        void begin();               // Parser entry point
        void retrieve();            // Get the next token
        void indentation(Node* node, int i);      // Sets node's indentation amount
        void error(std::string e, std::string v); // Parser error handler 
};

# endif // PARSER_H
