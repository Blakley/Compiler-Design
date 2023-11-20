/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Parser function declarations
*/

# include "headers/parser.h"
# include <iostream>


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
 *         Token parsing handler 
 *    Starts the initial top-down parsing
 * ------------------------------------------
*/
void Parser::begin() {
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
    std::cout << "handling <program> BNF\n";

    // create a new node for <program>
    node* node_program = create("<program>");

    // call function to parse <vars> and store node
    retrieve();
    new_child(node_program, create("<vars>"));
    parse_vars();

    // verify the opening parenthesis
    if (_token.instance != "xopen")
        error("xopen", _token.instance);

    std::cout << "consumed <xopen>\n";

    // call function to parse <stats> and store node
    retrieve(); 
    new_child(node_program, create("<stats>"));
    parse_stats();

    // verify the closing parenthesis
    if (_token.instance != "xclose")
        error("xclose", _token.instance);

    std::cout << "consumed <xclose>\n";

    retrieve(); 
    if (_token.id != eof_tk)
        error("EOF", _token.instance);

    // finished parsing
    // todo: print tree using inorder traversal
    std::cout << "\nParsing <program> completed.\n";
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
    std::cout << "handling <vars> BNF\n";

    // create a new node for <vars>
    node* node_vars = create("<vars>");

    // verify token value
    if (_token.instance == "xdata") {
        // the examples show an identifer first
        // but handle the case where it's xdata then the identifier
        retrieve();

        if (_token.id == identifier_tk) {
            std::cout << "consumed: " << _token.instance << " token\n";
            // store new child <varList> node
            new_child(node_vars, create("<varList>"));
            parse_varList();
        } 
        else
            error("identifier", _token.instance);
    } 
    else if (_token.id == identifier_tk) {
        std::cout << "consumed: " << _token.instance << " token\n";
        // store new child <varList> node
        new_child(node_vars, create("<varList>"));
        parse_varList();
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
void Parser::parse_varList() {
    std::cout << "handling <varList> BNF\n";

    // create a new node for <varList>
    node* node_varList = create("<varList>");

    // check for the identifier
    if (_token.id == identifier_tk) {
        new_token(node_varList, _token.instance);
        retrieve(); // retrieve next token

        // verify the colon
        if (_token.instance == ":") {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_varList, _token.instance);
            retrieve(); // retrieve next token
        }
        else
            error(":", _token.instance);

        // verify the integer
        if (_token.id == integer_tk) {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_varList, _token.instance);
            retrieve(); // retrieve next token
        }
        else
            error("integer", _token.instance);

        // check for the semicolon or the next <varList>
        if (_token.instance == ";") {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_varList, _token.instance);
            retrieve(); // finished, retrieve next token
        }
        // no semicolon; optional <varList> follows
        else if (_token.id == identifier_tk) {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_varList, _token.instance);

            // create a new node for the recursive <varList> call
            new_child(node_varList, create("<varList>"));
            parse_varList();
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
    std::cout << "handling <stat> BNF\n";

    // create a new node for <stat>
    node* node_stat = create("<stat>");

    if (_token.instance == "xin") {
        // create a new node for <in>
        new_child(node_stat, create("<in>"));
        parse_in();
    } 
    else if (_token.instance == "xout") {
        // create a new node for <out>
        new_child(node_stat, create("<out>"));
        parse_out();
    } 
    else if (_token.instance == "{") {
        // create a new node for <block>
        new_child(node_stat, create("<block>"));
        parse_block();
    } 
    else if (_token.instance == "xcond") {
        // create a new node for <if>
        new_child(node_stat, create("<if>"));
        parse_if();
    } 
    else if (_token.instance == "xloop") {
        // create a new node for <loop>
        new_child(node_stat, create("<loop>"));
        parse_loop();
    } 
    else if (_token.instance == "xlet") {
        // create a new node for <assign>
        new_child(node_stat, create("<assign>"));
        parse_assign();
    } 
    else {
        // handle error: unexpected token for <stat>
        error("statement", _token.instance);
    }
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
    std::cout << "handling <stats> BNF\n";

    // create a new node for <stats>
    node* node_stats = create("<stats>");

    // parse a single statement and add it as a child
    new_child(node_stats, create("<stat>"));
    parse_stat();

    // parse additional statements (if any) and add them as children
    retrieve();
    parse_mStat();
    if (_token.instance == "xin"   || _token.instance == "xout" || _token.instance == "{" 
    ||  _token.instance == "xcond" || _token.instance == "xloop" 
    ||  _token.instance == "xlet") {
        new_child(node_stats, create("<mStat>"));
        parse_mStat();
    }
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
    std::cout << "handling <mStat> BNF\n";

    // create a new node for <mStat>
    node* node_mStat = create("<mStat>");

    // check if there are more statements
    if (_token.instance == "xin"   || _token.instance == "xout" || _token.instance == "{" 
    ||  _token.instance == "xcond" || _token.instance == "xloop" 
    ||  _token.instance == "xlet") {

        // parse another statement and add it as a child
        new_child(node_mStat, create("<stat>"));
        parse_stat();

        // parse additional statements (if any) and add them as children
        retrieve();
        parse_mStat();
        if (_token.instance == "xin"   || _token.instance == "xout" || _token.instance == "{" 
        ||  _token.instance == "xcond" || _token.instance == "xloop" 
        ||  _token.instance == "xlet") {
            new_child(node_mStat, create("<mStat>"));
            parse_mStat();
        }
    } 

    // else, <mStat> should be empty
}


/**
 * ------------------------------------------
 *    Implementation for parsing <exp>
 *  
 *  BNF production rule: 
 *      <exp> -> <M> / <exp> | <M> * <exp> | 
 *               <M> + <exp> | <M> - <exp> | 
 *               <M> ~ <exp> | <M>
 *      
 * ------------------------------------------
*/
void Parser::parse_exp() {
    std::cout << "handling <exp> BNF\n";

    // create a new node for <exp>
    node* node_exp = create("<exp>");

    // handle first part of the rule and add it as a child
    new_child(node_exp, create("<M>"));
    parse_M();

    // check for division, multiplication, addition, subtraction, and ~
    while (_token.instance == "/" || _token.instance == "*"
        || _token.instance == "+" || _token.instance == "-" || _token.instance == "~") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_exp, _token.instance);
        retrieve();  // retrieve next token

        // add the next <M> as a child
        new_child(node_exp, create("<M>"));
        parse_M();
    }
}


/**
 * ------------------------------------------
 *    Implementation for parsing <M>
 *  
 *  BNF production rule: 
 *      <M> -> <N> + <M> | <N>
 *      
 * ------------------------------------------
*/
void Parser::parse_M() {
    std::cout << "handling <M> BNF\n";

    // create a new node for <M>
    node* node_M = create("<M>");

    // handle first part of rule and add it as a child
    new_child(node_M, create("<N>"));
    parse_N();

    // check for addition
    while (_token.instance == "+") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_M, _token.instance);
        retrieve(); // retrieve next token

        // add the next <M> as a child
        new_child(node_M, create("<M>"));
        parse_M();
    }
}


/**
 * ------------------------------------------
 *    Implementation for parsing <N>
 *  
 *  BNF production rule: 
 *      <N> -> <R> - <N> | ~ <N> | <R> 
 * ------------------------------------------
*/
void Parser::parse_N() {
    std::cout << "handling <N> BNF\n";

    // create a new node for <N>
    node* node_N = create("<N>");

    // verify first token
    if (_token.instance == "~") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_N, _token.instance);
        retrieve(); // retrieve next token

        new_child(node_N, create("<N>"));
        parse_N();  // parse the next <N>
    } 
    else {
        // parse <R> and add it as a child
        new_child(node_N, create("<R>"));
        parse_R();

        // check for subtraction
        bool unary = false;
        while (_token.instance == "-") {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_N, _token.instance);
            unary = true;
            retrieve(); // retrieve next token
        }

        // handle: <R> - <N> case
        if (unary) {
            // add the right <N> as a child
            new_child(node_N, create("<N>"));
            parse_N();
        }
    }
}


/**
 * ------------------------------------------
 *    Implementation for parsing <R>
 *  
 *  BNF production rule: 
 *      <R> -> ( <exp> ) | identifier | integer
 * ------------------------------------------
*/
void Parser::parse_R() {
    std::cout << "handling <R> BNF\n";

    // create a new node for <R>
    node* node_R = create("<R>");

    // verify first token
    if (_token.instance == "(") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_R, _token.instance);
        retrieve();  // retrieve next token

        new_child(node_R, create("<exp>"));
        parse_exp(); // parse the enclosed <exp>
        
        // verify and consume the closing parenthesis
        if (_token.instance == ")") {
            std::cout << "consumed: " << _token.instance << " token\n";
            new_token(node_R, _token.instance);
            retrieve(); // retrieve next token
        }
        else
            error(")", _token.instance);
    } 
    else if (_token.id == identifier_tk || _token.id == integer_tk) {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_R, _token.instance);
        retrieve(); // retrieve next token
    }
    else 
        error("identifier, integer, or (", _token.instance);
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
    std::cout << "handling <block> BNF\n";

    // create a new node for <block>
    node* node_block = create("<block>");

    // verify the opening curly brace
    if (_token.instance == "{") {
        std::cout << "consumed: " << _token.instance << " token\n";
        // new_token(node_block, _token.instance);
        retrieve(); // retrieve next token
    }
    else
        error("{", _token.instance);

    // call function to parse <vars> and add it as a child
    new_child(node_block, create("<vars>"));
    parse_vars();

    // call function to parse <stats> and add it as a child
    new_child(node_block, create("<stats>"));
    parse_stats();

    // verify the closing curly brace
    if (_token.instance != "}")
        error("}", _token.instance);

    new_token(node_block, _token.instance);

    std::cout << "consumed: " << _token.instance << " token\n";
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
    std::cout << "handling <in> BNF\n";

    // create a new node for <in>
    node* node_in = create("<in>");

    // verify first token
    if (_token.instance == "xin") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_in, _token.instance);
        retrieve(); // retrieve next token
    }
    else 
        error("xin", _token.instance);

    // verify next expected token
    if (_token.instance == ">>") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_in, _token.instance);
        retrieve(); // retrieve next token
    }
    else
        error(">>", _token.instance);

    // verify next expected token
    if (_token.id == identifier_tk) {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_in, _token.instance);
        retrieve(); // retrieve next token
    }
    else
        error("identifier", _token.instance);
    
    // verify next expected token
    if (_token.instance != ";")
        error(";", _token.instance);

    new_token(node_in, _token.instance);

    std::cout << "consumed: " << _token.instance << " token\n";
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
    std::cout << "handling <out> BNF\n";

    // create a new node for <out>
    node* node_out = create("<out>");

    // verify first token
    if (_token.instance == "xout") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_out, _token.instance);
        retrieve(); // retrieve next token
    }
    else 
        error("xout", _token.instance);

    // verify next expected token
    if (_token.instance == "<<")  {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_out, _token.instance);
        retrieve(); // retrieve next token
    }
    else
        error("<<", _token.instance);

    // call function to parse <exp> and add it as a child
    new_child(node_out, create("<exp>"));
    parse_exp();

    // verify next expected token
    if (_token.instance != ";") 
        error(";", _token.instance);

    new_token(node_out, _token.instance);

    std::cout << "consumed: " << _token.instance << " token\n";
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
    std::cout << "handling <if> BNF\n";

    // create a new node for <if>
    node* node_if = create("<if>");

    // verify and consume "xcond"
    if (_token.instance == "xcond") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_if, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("xcond", _token.instance);

    // verify and consume "["
    if (_token.instance == "[") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_if, _token.instance);
        retrieve(); // retrieve next token
    } 
    else 
        error("[", _token.instance);

    // parse and add <exp> as a child node
    new_child(node_if, create("<exp>"));
    parse_exp();

    // parse and add <RO> as a child node
    new_child(node_if, create("<RO>"));
    parse_RO();

    // parse and add the second <exp> as a child node
    new_child(node_if, create("<exp>"));
    parse_exp();

    // verify and consume "]"
    if (_token.instance == "]") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_if, _token.instance);
        retrieve(); // retrieve next token
    } 
    else 
        error("]", _token.instance);
    
    // parse and add <stat> as a child node
    new_child(node_if, create("<stat>"));
    parse_stat();
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
    std::cout << "handling <loop> BNF\n";

    // create a new node for <loop>
    node* node_loop = create("<loop>");

    // verify and consume "xloop"
    if (_token.instance == "xloop") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_loop, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("xloop", _token.instance);

    // verify and consume "["
    if (_token.instance == "[") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_loop, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("[", _token.instance);

    // parse and add <exp> as a child node
    new_child(node_loop, create("<exp>"));
    parse_exp();

    // parse and add <RO> as a child node
    new_child(node_loop, create("<RO>"));
    parse_RO();

    // parse and add the second <exp> as a child node
    new_child(node_loop, create("<exp>"));
    parse_exp();

    // verify and consume "]"
    if (_token.instance == "]") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_loop, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("]", _token.instance);

    // parse and add <stat> as a child node
    new_child(node_loop, create("<stat>"));
    parse_stat();
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
    std::cout << "handling <assign> BNF\n";

    // create a new node for <assign>
    node* node_assign = create("<assign>");

    // verify and consume "xlet"
    if (_token.instance == "xlet") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_assign, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("xlet", _token.instance);

    // verify and consume identifier
    if (_token.id == identifier_tk) {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_assign, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error("identifier", _token.instance);

    // parse and add <exp> as a child node
    new_child(node_assign, create("<exp>"));
    parse_exp();

    // verify and consume ";"
    if (_token.instance == ";") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_assign, _token.instance);
        retrieve(); // retrieve next token
    } 
    else
        error(";", _token.instance);
}


/**
 * ------------------------------------------
 *    Implementation for parsing <RO>
 *  
 *  BNF production rule: 
 *      <RO> -> << (one token) | >> (one token) | < | > | = | %
 *      
 * ------------------------------------------
*/
void Parser::parse_RO() {
    std::cout << "handling <RO> BNF\n";

    // create a new node for <RO>
    node* node_RO = create("<RO>");

    // check for the possible relational operators
    if (_token.instance == "<<" || _token.instance == ">>" ||
        _token.instance == "<" || _token.instance == ">" ||
        _token.instance == "=" || _token.instance == "%") {
        std::cout << "consumed: " << _token.instance << " token\n";
        new_token(node_RO, _token.instance);  // store the relational operator in the node
        retrieve();  // consume the relational operator
    } 
    else
        error("relational operator", _token.instance);
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

    // skip comment tokens
    if (_token.instance[0] == '$') {
        _tokens results = scanner.scanner();
        _token = std::get<1>(results);
    }
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
    std::cerr << "Error, expected [ " << expected << " ] but received token: " << value << std::endl;
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



