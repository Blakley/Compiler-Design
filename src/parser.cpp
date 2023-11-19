/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Parser function declarations
*/

# include "headers/parser.h"
# include <iostream>

// nonterminal function pointer table (potentially remove these)
using nonterminal_function = void (Parser::*)();

std::map<std::string, nonterminal_function> nonterminals = {
    {"xin"   , &Parser::parse_in},
    {"xout"  , &Parser::parse_out},
    {"xcond" , &Parser::parse_if},
    {"xloop" , &Parser::parse_loop},
    {"xlet"  , &Parser::parse_assign},
};


/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Parser::Parser(Scanner& scanner) : scanner(scanner) {
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
 *             Parser handler
 *   Determines which nonterminal is next
 *     in the program verification
 * ------------------------------------------
*/
Parser::nonterminal_function Parser::parse() {
    // todo: (potentially remove this function)
    // maybe return a function pointer to the next function we should call

    // example return
    return &Parser::parse_in;
} 


/**
 * ------------------------------------------
 *         Token parsing handler 
 *    Starts the initial top-down parsing
 * ------------------------------------------
*/
void Parser::begin() {
    // get a reference to the first token
    _tokens results = scanner.scanner();
    _token = std::get<1>(results);

    // begin parsing the input program
    parse_program();
}


/**
 * ------------------------------------------
 *    Implementation for parsing <program>
 *  
 *  BNF production rule: 
 *      <program> -> <vars> xopen <stats> xclose
 * ------------------------------------------
*/
void Parser::parse_program() {
    std::cout << "About to start parsing <program>.\n";

    // call function to parse <vars>
    parse_vars();

    // verify the opening parenthesis
    if (_token.instance == "xopen")
        retrieve(); // retrieve next token
    else
        error("xopen", _token.instance);

    // call function to parse <stats>
    parse_stats();

    // verify the closing parenthesis
    if (_token.instance == "xclose")
        retrieve(); // retrieve next token
    else
        error("xclose", _token.instance);

    // finished parsing
    // todo: print print
    std::cout << "Parsing <program> completed.\n";
}


/**
 * ------------------------------------------
 *    Implementation for parsing <vars>
 *  
 *  BNF production rule: 
 *      <vars> -> empty | xdata [or identifier] <varList>
 * ------------------------------------------
*/
void Parser::parse_vars() {

    if (_token.instance == "xdata") {
        // the examples show an identifer first
        // but handle the case where its xdata then the identifer
        retrieve(); // retrieve next token

        if (_token.id == identifier_tk) {
            retrieve(); // retrieve next token
            parse_varsList();
        }
    }
    else if (_token.id == identifier_tk) {
        retrieve(); // retrieve next token
        parse_varsList();
    }

    // else, <vars> should be empty
}


/**
 * ------------------------------------------
 *    Implementation for parsing <varList>
 *  
 *  BNF production rule: 
 *      <varList> -> identifier : integer ; | identifier : integer <varList>
 * ------------------------------------------
*/
void Parser::parse_varsList() {
    // check for the identifier
    if (_token.id == identifier_tk) {
        retrieve(); // retrieve next token

        // verify the colon
        if (_token.instance == ":")
            retrieve(); // retrieve next token
        else
            error(":", _token.instance);

        // verify the integer
        if (_token.id == integer_tk)
            retrieve(); // retrieve next token
        else
            error("integer", _token.instance);

        // check for the semicolon or the next <varList>
        if (_token.instance == ";") {
            retrieve(); // retrieve next token
        }
        // no semicolon; optional <varList> follows
        else if (_token.id == identifier_tk) {
            parse_varsList();
        }
        // no semicolon or <varList>; 
        else
            error("';' or identifier", _token.instance);
    } 
    else
        error("identifier", _token.instance);
}


/**
 * ------------------------------------------
 *    Implementation for parsing <stat>
 *  
 *  BNF production rule: 
 *      <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
 * ------------------------------------------
*/
void Parser::parse_stat() {
    if (_token.instance == "xin")
        parse_in();
    else if (_token.instance == "xout")
        parse_out();
    else if (_token.instance == "{") 
        parse_block();
    else if (_token.instance == "xcond") 
        parse_if();
    else if (_token.instance == "xloop") 
        parse_loop();
    else if (_token.instance == "xlet") 
        parse_assign();
    else
        // handle error: unexpected token for <stat>
        error("statement", _token.instance);
}


/**
 * ------------------------------------------
 *    Implementation for parsing <stats>
 *  
 *  BNF production rule: 
 *      <stats> -> <stat> <mStat>
 * ------------------------------------------
*/
void Parser::parse_stats() {
    // parse a single statement
    parse_stat();

    // parse additional statements (if any)
    parse_mStat();
}


/**
 * ------------------------------------------
 *    Implementation for parsing <mStat>
 *  
 *  BNF production rule: 
 *      empty | <stat> <mStat>
 * ------------------------------------------
*/
void Parser::parse_mStat() {
    // Check if there are more statements
    if (_token.instance == "xin"   || _token.instance == "xout" || _token.instance == "{" 
    ||  _token.instance == "xcond" || _token.instance == "xloop" 
    ||  _token.instance == "xlet") {
        // parse another statement
        parse_stat();

        // parse additional statements (if any)
        parse_mStat();
    } 

    // else, <mStat> should be empty
}




















/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_exp() {
    // Implementation for parsing <exp>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_M() {
    // Implementation for parsing <M>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_N() {
    // Implementation for parsing <N>
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_R() {
    // Implementation for parsing <R>
}


/**
 * ------------------------------------------
 *    Implementation for parsing <block>
 *  
 *  BNF production rule: 
 *      <block> -> { <vars> <stats> }
 * ------------------------------------------
*/
void Parser::parse_block() {
    std::cout << "Token is {, parsing <block> nonterminal.\n";

    // verify the opening curly brace
    if (_token.instance == "{")
        retrieve(); // retrieve next token
    else
        error("{", _token.instance);

    // call function to parse <vars>
    parse_vars();

    // call function to parse <stats>
    parse_stats();

    // verify the closing curly brace
    if (_token.instance == "}")
        retrieve(); // retrieve next token
    else
        error("}", _token.instance);

    // finished parsing nonterminal, call next function
    Parser::nonterminal_function next = this->parse();
    (this->*next)();
}


/**
 * ------------------------------------------
 *    Implementation for parsing <in>
 *  
 *  BNF production rule: 
 *      <in> -> xin >> identifier ; 
 * ------------------------------------------
*/
void Parser::parse_in() {

    std::cout << "Token is xin, parsing <in> nonterminal.\n";

    // verify first token
    if (_token.instance == "xin")
        retrieve(); // retrieve next token
    else 
        error("xin", _token.instance);

    // verify next expected token
    if (_token.instance == ">>") 
        retrieve(); // retrieve next token
    else
        error(">>", _token.instance);

    // verify next expected token
    if (_token.id == identifier_tk)
        retrieve(); // retrieve next token
    else
        error("identifier", _token.instance);
    
    // verify next expected token
    if (_token.instance == ";") {
        // finished parsing nonterminal, call next function
        Parser::nonterminal_function next = this->parse();
        (this->*next)(); 
    }
    else 
        error(";", _token.instance);
}


/**
 * ------------------------------------------
 *    Implementation for parsing <out>
 *  
 *  BNF production rule: 
 *      <out> -> xout << <exp> ;
 * ------------------------------------------
*/
void Parser::parse_out() {
    std::cout << "Token is xout, parsing <out> nonterminal.\n";

    // verify first token
    if (_token.instance == "xout")
        retrieve(); // retrieve next token
    else 
        error("xout", _token.instance);

    // verify next expected token
    if (_token.instance == "<<") 
        retrieve(); // retrieve next token
    else
        error("<<", _token.instance);

    // call function to parse <exp>
    parse_exp();

    // verify next expected token
    if (_token.instance == ";") {
        // finished parsing nonterminal, call next function
        Parser::nonterminal_function next = this->parse();
        (this->*next)(); 
    }
    else 
        error(";", _token.instance);
}


/**
 * ------------------------------------------
 *    Implementation for parsing <if>
 *  
 *  BNF production rule: 
 *      <if> -> xcond [ <exp> <RO> <exp> ] <stat>
 * ------------------------------------------
*/
void Parser::parse_if() {
    std::cout << "Token is xcond, parsing <if> nonterminal.\n";

    // verify first token
    if (_token.instance == "xcond")
        retrieve(); // retrieve next token
    else 
        error("xcond", _token.instance);

    // verify next expected token
    if (_token.instance == "[") 
        retrieve(); // retrieve next token
    else
        error("[", _token.instance);

    // call function to parse <exp>
    parse_exp();

    // call function to parse <RO>
    parse_RO();

    // call function to parse <exp>
    parse_exp();

    // verify next expected token
    if (_token.instance == "]") 
        retrieve(); // retrieve next token
    else
        error("]", _token.instance);

    // call function to parse <stat>
    parse_stat();

    // finished parsing nonterminal, call next function
    Parser::nonterminal_function next = this->parse();
    (this->*next)(); 
}


/**
 * ------------------------------------------
 *    Implementation for parsing <loop>
 *  
 *  BNF production rule: 
 *      <loop> -> xloop [ <exp> <RO> <exp> ] <stat>
 * ------------------------------------------
*/
void Parser::parse_loop() {
    std::cout << "Token is xloop, parsing <loop> nonterminal.\n";

    // verify first token
    if (_token.instance == "xloop")
        retrieve();
    else
        error("xloop", _token.instance);

    // verify next expected token
    if (_token.instance == "[")
        retrieve();
    else
        error("[", _token.instance);

    // call function to parse <exp>
    parse_exp();

    // call function to parse <RO>
    parse_RO();

    // call function to parse <exp>
    parse_exp();

    // verify next expected token
    if (_token.instance == "]")
        retrieve();
    else
        error("]", _token.instance);

    // call function to parse <stat>
    parse_stat();

    // finished parsing nonterminal, call next function
    Parser::nonterminal_function next = this->parse();
    (this->*next)();
}


/**
 * ------------------------------------------
 *    Implementation for parsing <assign>
 *  
 *  BNF production rule: 
 *      <assign> -> xlet identifier <exp> ;
 * ------------------------------------------
*/
void Parser::parse_assign() {
    std::cout << "Token is xlet, parsing <assign> nonterminal.\n";

    // verify first token
    if (_token.instance == "xlet")
        retrieve();
    else
        error("xlet", _token.instance);

    // verify next expected token
    if (_token.id == identifier_tk)
        retrieve();
    else
        error("identifier", _token.instance);

    // call function to parse <exp>
    parse_exp();

    // verify next expected token
    if (_token.instance == ";") {
        retrieve();
        
        // finished parsing nonterminal, call next function
        Parser::nonterminal_function next = this->parse();
        (this->*next)();
    }
    else
        error(";", _token.instance);
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_RO() {
    // Implementation for parsing <RO>
}








/**
 * ------------------------------------------
 *    Handle updating the current token
 * ------------------------------------------
*/
void Parser::retrieve() {
    // get and set the next token reference
    _tokens results = scanner.scanner();
    _token = std::get<1>(results);
}


/**
 * ------------------------------------------
 *         Handle error parsing
 * 
 * @param expected : expected token
 * @param value    : received token
 * ------------------------------------------
*/
void Parser::error(std::string expected, std::string value) {
    // Implementation for error handling
    std::cerr << "Error, expected [" << expected << "] but received token: " << value << std::endl;
    exit(EXIT_FAILURE);
}


/* 
    =========================================== 
           Node function declarations
    ===========================================
*/


/**
 * ------------------------------------------
 *       Function to create a new node
 * 
 * @param label  :
 * ------------------------------------------
*/
node* create(const std::string& label) {
    node* newNode = new node;
    newNode->label = label;
    return newNode;
}


/**
 * ------------------------------------------
 *   Function to add a new child to a node
 * 
 * @param parent :
 * @param child  :
 * ------------------------------------------
*/
void new_child(node* parent, node* child) {
    parent->children.push_back(child);
}


/**
 * ------------------------------------------
 *   Function to add a new token to a node
 * 
 * @param node  :
 * @param token :
 * ------------------------------------------
*/
void new_token(node* node, const std::string& token) {
    node->tokens.push_back(token);
}



