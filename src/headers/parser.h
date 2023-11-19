/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Parser header file recursive decent top-down parser
*/

# ifndef PARSER_H
# define PARSER_H

# include "node.h"
# include "token.h"
# include "scanner.h"

// Recursive decent: top-down parser parser class
class Parser {
    private:
        // Define function pointer type
        typedef void (Parser::*nonterminal_function)();

        token _token;     // reference to current token
        Scanner& scanner; // scanner reference

    public:
        ~Parser();                  // destructor
        Parser(Scanner& scanner);   // constructor
        
        void parse_program();  // 
        void parse_varsList(); //
        void parse_vars();     //
        
        void parse_exp();    // 
        void parse_M();      //
        void parse_N();      //
        void parse_R();      //
        
        void parse_stat();   //
        void parse_stats();  //
        void parse_mStat();  //
        void parse_block();  //
        
        void parse_in();     //
        void parse_out();    //

        void parse_if();     //
        void parse_loop();   //
        
        void parse_RO();     //  
        void parse_assign(); //

        nonterminal_function parse();               // parser handler           
        void begin();                               // parser entry point
        void retrieve();                            // get the next token
        bool identify(token& t);                    // returns if the token is a nonterminal
        void error(std::string e, std::string v);   // parser error handler 
};

# endif // PARSER_H