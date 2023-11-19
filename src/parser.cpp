/*
    Name: Anthony Blakley
    Date: 11/17/2023
    Description: 
        Parser function declarations
*/

# include "headers/parser.h"
# include <iostream>

// nonterminal function pointer table
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
    // todo: 
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

    // verify first token
    if (_token.instance != "xopen")
        error("xopen", _token.instance);
    
    // call non terminal function
    parse_program();
}


/**
 * ------------------------------------------
 *         Identifies Nonterminals
 * 
 * @return : returns if the given token
 *           is a nonterminal
 * ------------------------------------------
*/
bool Parser::identify(token& t) {
    // nonterminals are identifier tokens
    if (t.id == identifier_tk || t.id == keyword_tk)
        return true;

    return false;
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
 * 
 * ------------------------------------------
*/
void Parser::parse_stat() {
    // Implementation for parsing <stat>
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
    // Implementation for parsing <stat>
    parse_stat();

    // Implementation for parsing <mStat>
    parse_mStat();
}


/**
 * ------------------------------------------
 * 
 * ------------------------------------------
*/
void Parser::parse_mStat() {
    // Implementation for parsing <mStat>
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



