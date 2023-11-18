/*
    Name: Anthony Blakley
    Date: 11/16/2023
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
        // reference to current token
        token _token;
        
        // scanner reference
        Scanner scanner;

    public:
        Parser();   // constructor
        ~Parser();  // destructor
        
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

        void parse(int a, char** v);      // parser entry point
        void match(std::string expected); // determine if the token is correct
        void error(std::string message);  // parser error handler 
};

# endif // PARSER_H