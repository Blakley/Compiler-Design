/*
    Name: Anthony Blakley
    Date: 12/11/2023
    Description: 
        Parser function declarations
*/

# include "../headers/parser.h"
# include <iostream>


/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Parser::Parser(Scanner& s, Tree& t) : scanner(s), tree(t) {
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

    // store node identation values
    indentation(tree.root_node(), 0);
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
    // create first node (root)
    tree.new_node("<program>");

    // call function to parse <vars>
    // add child <vars> to parent <program>
    retrieve();
    parse_vars();
    tree.new_child(tree.root_node(), tree.reference);

    // verify the opening parenthesis
    if (_token.instance != "xopen")
        error("xopen", _token.instance);

    // call function to parse <stats>
    // add child <stats> to parent <program>
    retrieve(); 
    parse_stats();
    tree.new_child(tree.root_node(), tree.reference);

    // verify the closing parenthesis
    if (_token.instance != "xclose")
        error("xclose", _token.instance);

    retrieve(); 
    if (_token.id != eof_tk)
        error("EOF", _token.instance);
}


/**
 * ------------------------------------------
 *     Implementation for parsing <vars>
 *  
 *  BNF production rule: 
 *      <vars> -> empty | xdata [or identifier] <varList>
 * ------------------------------------------
*/
void Parser::parse_vars() {
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<vars>");

    // get a reference to the created node 
    Node* vars_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // verify token value
    if (_token.instance == "xdata") {        
        // add "xdata" token to <vars> node
        tree.new_token(vars_node, &_token);

        // the examples show an identifier first
        // but handle the case where its xdata then the identifier
        retrieve();

        if (_token.id == identifier_tk) {
            // add child <varList> to parent <vars>
            parse_varList();
            tree.new_child(vars_node, tree.reference);
        }
        else
            error("identifier", _token.instance);
    }
    else if (_token.id == identifier_tk) {
        // add child <varList> to parent <vars>
        parse_varList();
        tree.new_child(vars_node, tree.reference);
    }
    // else, <vars> should be empty

    // store reference to <vars> to be added as a child
    tree.reference = vars_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<varList>");

    // get a reference to the created node 
    Node* varList_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // check for the identifier
    if (_token.id == identifier_tk) {
        // add "identifier" token to <varList> node
        tree.new_token(varList_node, &_token);

        retrieve(); // retrieve next token

        // verify the colon
        if (_token.instance == ":")
            retrieve(); // retrieve next token
        else
            error(":", _token.instance);

        // verify the integer
        if (_token.id == integer_tk) {
            // add "integer" token to <varList> node
            // retrieve next token
            tree.new_token(varList_node, &_token);
            retrieve(); 
        }
        else
            error("integer", _token.instance);

        // check for the semicolon or the next <varList>
        if (_token.instance == ";") {
            retrieve(); // finished, retrieve next token
        }
        // no semicolon; optional <varList> follows
        else if (_token.id == identifier_tk) {
            // add child <varList> to parent <varList>
            parse_varList();
            tree.new_child(varList_node, tree.reference);
        }
        // no semicolon or <varList>; 
        else
            error("';' or identifier", _token.instance);
    } 
    else
        error("identifier", _token.instance);

    // store reference to <varList> to be added as a child
    tree.reference = varList_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<stat>");

    // get a reference to the created node 
    Node* stat_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

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

    // add child <from above> to parent <stat>
    tree.new_child(stat_node, tree.reference);

    // store reference to <stat> to be added as a child
    tree.reference = stat_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<stats>");

    // get a reference to the created node 
    Node* stats_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // parse a single statement
    // add child <stat> to parent <stats>
    parse_stat();
    tree.new_child(stats_node, tree.reference);

    // parse additional statements (if any)
    // add child <mStat> to parent <stats>
    retrieve();
    parse_mStat();
    tree.new_child(stats_node, tree.reference);

    // store reference to <stats> to be added as a child
    tree.reference = stats_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<mStat>");

    // get a reference to the created node 
    Node* mStat_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // check if there are more statements
    if (_token.instance == "xin"   || _token.instance == "xout" || _token.instance == "{" 
    ||  _token.instance == "xcond" || _token.instance == "xloop" 
    ||  _token.instance == "xlet") {

        // parse another statement
        // add child <stat> to parent <mStat>
        parse_stat();
        tree.new_child(mStat_node, tree.reference);

        // parse additional statements (if any)
        // add child <mStat> to parent <mStat>
        retrieve();
        parse_mStat();
        tree.new_child(mStat_node, tree.reference);
    } 
    // else, <mStat> should be empty

    // store reference to <mStat> to be added as a child
    tree.reference = mStat_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<exp>");

    // get a reference to the created node 
    Node* exp_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // handle first part of rule
    // add child <M> to parent <exp>
    parse_M();
    tree.new_child(exp_node, tree.reference);

    // check for division, multiplication, addition, subtraction, and ~
    while (_token.instance == "/" || _token.instance == "*"
        || _token.instance == "+" || _token.instance == "-" || _token.instance == "~") {

        _operations.push_back(_token.instance); // store operator

        retrieve();  // retrieve next token

        // add child <M> to parent <exp>
        parse_M();

        // add operator identifier
        tree.new_child(exp_node, tree.reference);
    }

    // store reference to <exp> to be added as a child
    tree.reference = exp_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<M>");

    // get a reference to the created node 
    Node* m_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // handle first part of rule
    // add child <N> to parent <M>
    parse_N();
    tree.new_child(m_node, tree.reference);

    // check for addition
    while (_token.instance == "+") {        
        _operations.push_back(_token.instance); // store operator
        
        retrieve(); // retrieve next token

        // parse the next <M>
        // add child <M> to parent <M>
        parse_M();  
        tree.new_child(m_node, tree.reference);
    }

    // store reference to <M> to be added as a child
    tree.reference = m_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<N>");

    // get a reference to the created node 
    Node* n_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // verify first token
    if (_token.instance == "~") {
        _operations.push_back(_token.instance); // store operator

        retrieve(); // retrieve next token

        // parse the next <N>
        // add child <N> to parent <N>
        parse_N();  
        tree.new_child(n_node, tree.reference);
    } 
    else {
        // parse <R>
        // add child <R> to parent <N>
        parse_R();  
        tree.new_child(n_node, tree.reference);

        // check for subtraction
        bool unary = false;
        while (_token.instance == "-") {
            _operations.push_back(_token.instance); // store operator

            unary = true;
            retrieve(); // retrieve next token
        }

        // handle: <R> - <N> case
        if (unary) {
            // add child <N> to parent <N>
            parse_N();
            tree.new_child(n_node, tree.reference);
        }
    }

    // store reference to <N> to be added as a child
    tree.reference = n_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<R>");

    // get a reference to the created node 
    Node* r_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // verify first token
    if (_token.instance == "(") {
        _operations.push_back(_token.instance); // store value

        retrieve();  // retrieve next token

        // parse the enclosed <exp>
        // add child <exp> to parent <R>
        parse_exp(); 
        tree.new_child(r_node, tree.reference);

        // verify and consume the closing parenthesis
        if (_token.instance == ")") {
            _operations.push_back(_token.instance); // store value

            retrieve(); // retrieve next token
        }
        else
            error(")", _token.instance);
    } 
    else if (_token.id == identifier_tk || _token.id == integer_tk) {
        // add "identifier" or "integer" token to <R> node
        // retrieve next token

        _operations.push_back(_token.instance); // store value

        tree.new_token(r_node, &_token);
        retrieve(); 
    }
    else 
        error("identifier, integer, or (", _token.instance);


    // store reference to <R> to be added as a child
    tree.reference = r_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<block>");

    // get a reference to the created node 
    Node* block_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // verify the opening curly brace
    if (_token.instance == "{")
        retrieve(); // retrieve next token
    else
        error("{", _token.instance);

    // call function to parse <vars>
    // add child <vars> to parent <block>
    parse_vars();
    tree.new_child(block_node, tree.reference);

    // call function to parse <stats>
    // add child <stats> to parent <block>
    parse_stats();
    tree.new_child(block_node, tree.reference);

    // verify the closing curly brace
    if (_token.instance != "}")
        error("}", _token.instance);

    // store reference to <block> to be added as a child
    tree.reference = block_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<in>");

    // get a reference to the created node 
    Node* in_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

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
    if (_token.id == identifier_tk) {
        // add "identifier" token to <in> node
         // retrieve next token
        tree.new_token(in_node, &_token);
        retrieve(); 
    }
    else
        error("identifier", _token.instance);
    
    // verify next expected token
    if (_token.instance != ";")
        error(";", _token.instance);

    // store reference to <in> to be added as a child
    tree.reference = in_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<out>");

    // get a reference to the created node 
    Node* out_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

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

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <out>
    tree.new_child(out_node, tree.reference);


    // verify next expected token
    if (_token.instance != ";") 
        error(";", _token.instance);

    // store reference to <out> to be added as a child
    tree.reference = out_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<if>");

    // get a reference to the created node 
    Node* if_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

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

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <if>
    tree.new_child(if_node, tree.reference);

    // call function to parse <RO>
    parse_RO();

    // update relation operator for <if>
    tree.reference->tokens.push_back(_relationals[0]);
    _relationals.clear();

    // add child <RO> to parent <if>
    tree.new_child(if_node, tree.reference);

    // call function to parse <exp>
    parse_exp();

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <if>
    tree.new_child(if_node, tree.reference);

    // verify next expected token
    if (_token.instance == "]")
        retrieve(); // retrieve next token
    else
        error("]", _token.instance);

    // call function to parse <stat>
    parse_stat();
    
    // add child <stat> to parent <if>
    tree.new_child(if_node, tree.reference);

    // store reference to <if> to be added as a child
    tree.reference = if_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<loop>");

    // get a reference to the created node 
    Node* loop_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

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

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <loop>
    tree.new_child(loop_node, tree.reference);

    // call function to parse <RO>
    // add child <RO> to parent <loop>
    parse_RO();

    // update relation operator for <loop>
    tree.reference->tokens.push_back(_relationals[0]);
    _relationals.clear();

    tree.new_child(loop_node, tree.reference);

    // call function to parse <exp>
    parse_exp();

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <loop>
    tree.new_child(loop_node, tree.reference);

    // verify next expected token
    if (_token.instance == "]")
        retrieve();
    else
        error("]", _token.instance);

    // call function to parse <stat>
    parse_stat();

    // add child <stat> to parent <loop>
    tree.new_child(loop_node, tree.reference);

    // store reference to <loop> to be added as a child
    tree.reference = loop_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<assign>");

    // get a reference to the created node 
    Node* assign_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // verify first token
    if (_token.instance == "xlet")
        retrieve();
    else
        error("xlet", _token.instance);

    // verify next expected token
    if (_token.id == identifier_tk) {
        // add "identifier" token to <assign> node
        tree.new_token(assign_node, &_token);
        retrieve();
    }
    else
        error("identifier", _token.instance);

    // call function to parse <exp>
    parse_exp();

    // update operations for <exp>
    for (auto o : _operations)
        tree.reference->tokens.push_back(o);
    _operations.clear();

    // add child <exp> to parent <assign>
    tree.new_child(assign_node, tree.reference);

    // verify next expected token
    if (_token.instance != ";")
        error(";", _token.instance); 

    // store reference to <assign> to be added as a child
    tree.reference = assign_node;
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
    /**
     * ======================
     *  handle node storing
     * ======================
    */

    // create new node
    tree.new_node("<RO>");

    // get a reference to the created node 
    Node* RO_node = tree.previous_node();

    /**
     * ======================
     *    handle parsing
     * ======================
    */

    // check for the possible relational operators
    if (_token.instance == "<<" || _token.instance == ">>" ||
        _token.instance == "<"  || _token.instance == ">" ||
        _token.instance == "="  || _token.instance == "%") {   

        _relationals.push_back(_token.instance); // store operator

        // consume the relational operator
        retrieve();  
    }
    else
        error("relational operator", _token.instance);

    // store reference to <RO> to be added as a child
    tree.reference = RO_node;
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
 *    Updates the indentation for the node
 * 
 * @param node        : current node
 * @param indentation : indentation amount
 * ------------------------------------------
*/
void Parser::indentation(Node* node, int amount) {
    if (node == nullptr)
        return;

    // store indentation amount
    node->indentation = amount;

    // traverse children in preorder with increased indentation
    for (auto child : node->children)
        indentation(child, amount + 1);
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
    std::cerr << "[Error], PARSER ERROR, expected [ " << expected << " ] but received token: " << value << std::endl;
    exit(EXIT_FAILURE);
}


