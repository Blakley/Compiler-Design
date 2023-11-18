/*
    Name: Anthony Blakley
    Date: 11/17/2023
    Description: 
        Parser function declarations
*/

# include "headers/parser.h"
# include <iostream>
# include <fstream>

/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Parser::Parser() {
    // constructor function
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Parser::~Parser() {
    // destructor function
}


/**
 * ------------------------------------------
 * 
 * 
 * @param argc  : argument count
 * @param argv  : argument array
 * ------------------------------------------
*/
void Parser::parse(int argc, char** argv) {
    // handle invalid arguments detected by scanner    
    scanner.arguments(argc, argv);

    // test scanner class
    // scanner.tester();

    // get a reference to the first token
    _tokens results = scanner.scanner();
    _token = std::get<1>(results);

    // determine which nonterminal the token belongs to


    if (_token.id == keyword_tk) {

    }




    // scanner.display(_token);
    // // get tokens from file until EOF
    // while (true) {
    //     if (_token.id == eof_tk)
    //         break;
        
    //     results = scanner.scanner();
    //     _token = std::get<0>(results);

    //     scanner.display(std::get<1>(results));
    // }




    // todo: Implementation for the overall parsing process
    /*
        1) start by reading the first token
        and depending on what it is, call the appropriate 
        nonterminal function in the parser class.
    */
   
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_program() {
    // Implementation for parsing <program>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_varsList() {
    // Implementation for parsing <varsList>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_vars() {
    // Implementation for parsing <vars>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_exp() {

}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_M() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_N() {

}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_R() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_stat() {

}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_stats() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_mStat() {

}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_block() {
    
}

/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_in() {
    /*
    
        each of those functions will then do their logic
        and at the end, call scanner again to parse the next given.
        Similiar to this:

        nonterminal <in> sudo code:
        func in() {
            if (token == xin)
                token = scanner()
            else 
                error("xin expected but received token ..")

            if (token == ">>")
                token = scanner()
            ekse
                error("id expected but reeived token")

            if (token == ;tk)
                token = scanner()
            else
                error(";tk expected but received token")
            
            return
        }
    
    */
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_out() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_if() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_loop() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_RO() {
    
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_assign() {
    
}


/**
 * ------------------------------------------
 *        Handle token matching
 * 
 * @param expected : the expected token
 * ------------------------------------------
*/
void Parser::match(std::string expected) {
    // Implementation for matching a token
}


/**
 * ------------------------------------------
 *         Handle error parsing
 * 
 * @param message : provided error message
 * ------------------------------------------
*/
void Parser::error(std::string message) {
    // Implementation for error handling
}


/* 
    =========================================== 
           Node function declarations
    ===========================================
*/


/**
 * ------------------------------------------
 * 
 * 
 * @param label  :
 * ------------------------------------------
*/
node* create(const std::string& label) {
    // Function to create a new node

    node* newNode = new node;
    newNode->label = label;
    return newNode;
}


/**
 * ------------------------------------------
 * 
 * 
 * @param parent :
 * @param child  :
 * ------------------------------------------
*/
void new_child(node* parent, node* child) {
    // Function to add a new child to a node
    parent->children.push_back(child);
}


/**
 * ------------------------------------------
 * 
 * 
 * @param node  :
 * @param token :
 * ------------------------------------------
*/
void new_token(node* node, const std::string& token) {
    // Function to add a new token to a node
    node->tokens.push_back(token);
}
