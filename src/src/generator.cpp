/*
    Name: Anthony Blakley
    Date: 12/09/2023
    Description: 
        Generator function declarations
*/

#include "../headers/generator.h"
#include <algorithm>


/**
 * ------------------------------------------
 *              Constructor
 * 
 * @param r : root node of AST
 * ------------------------------------------
*/
Generator::Generator(Node* root) : root(root), temp_counter(0), label_counter(0) {
    // add appropriate operators
    configure();
    
    // start generating assembly
    generate(root);

    // generate ending assembly
    generate_xclose();
}


/**
 * ------------------------------------------
 *               Destructor
 * ------------------------------------------
*/
Generator::~Generator() {
    // destructor function
}


/**
 * ------------------------------------------
 *   Setup appropriate program operators
 * ------------------------------------------
*/
void Generator::configure() {
    // relational operator flag
    bool relational = false;

    for (auto _operator : root->tokens) {
        if (_operator == "?")
            relational = true;
        
        if (relational)
            _relationals.push_back(_operator);
        else
            _operators.push_back(_operator);
    }
}


/**
 * ------------------------------------------
 *  Traverse the Tree to generate the .asm
 * 
 * @param node : the current node 
 * ------------------------------------------
*/
void Generator::generate(Node* node) {
    // ensure the node is not null
    if (node == nullptr)
        return;

    /**
     * ===============================
     *  handle <varList> nonterminal:
     *  <varList> -> identifier : integer ; | identifier : integer <varList>
     * ===============================
    */
    if (node->label == "<varList>")
        generate_varList(node);

    /**
     * ===============================
     *  handle <in> nonterminal:
     *  <in> -> xin >> identifier ; 
     * ===============================
    */
    if (node->label == "<in>")
        generate_xin(node);

    /**
     * ===============================
     *  handle <out> nonterminal:
     *  <out> -> xout << <exp> ;
     * ===============================
    */
    if (node->label == "<out>")
        generate_xout(node);     


    /**
     * ===============================
     *  handle <exp> nonterminal:
     *  <exp> -> <M> / <exp> | <M> * <exp> | <M> 
     * ===============================
    */
    if (node->label == "<exp>")
        generate_exp(node);


    // traverse the children
    for (auto child : node->children)
        generate(child);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<varList>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_varList(Node* node) {
    // get identifier name
    std::string identifier = identify(node, 0);
    
    // get identifier value
    std::string value = identify(node, 1);

    // generate assembly
    assembly.push_back("LOAD " + value);
    assembly.push_back("STORE " + identifier);

    // traverse child nodes
    for (auto child : node->children) {
        // skip redundant processing
        if (child->label != "<varList>")
            generate_varList(child);
    }
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *         input statement (<in>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xin(Node* node) {
    // get value to READ in
    std::string value = identify(node, 0);

    // append the READ instruction for input
    if (value != "")                     
        assembly.push_back("READ " + value);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<out>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xout(Node* node) {
    // collect variables and integers
    get_values(node);
    
    // ================================
    //        create assembly
    // ================================

    if (_operators.size() == 0) {
        // case 1: output single variable or integer
        assembly.push_back("WRITE " + _values[0]);
        _values.clear();
    }
    else {
        // case 2: todo
    }
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<exp>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_exp(Node* node) {
    // todo:
    /*
        1)
        2)
        3)
        4)
        5)
    
    */
}


/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *   (<xclose>) section of the program
 * ------------------------------------------
*/
void Generator::generate_xclose() {   
    // append STOP instruction
    assembly.push_back("STOP");

    // generate local variables
    generate_vars(root);

    // add local variables after STOP and initialize to 0
    for (const std::string& var : locals)
        assembly.push_back(var + " 0");
}


/**
 * ------------------------------------------
 *      Generates assembly code for the 
 *    local variables in the input program
 * 
 *  @param node : current node
 * ------------------------------------------
*/
void Generator::generate_vars(Node* node) {
    // ensure the node is not null
    if (node == nullptr)
        return;

    // find identifiers
    for (const auto& token : node->tokens) {
        // search for an identifier in the token string
        std::string identifier;
        size_t position = token.find("identifier: \"");

        // extract variable name   
        if (position != std::string::npos) {
            size_t s = position + 13;
            size_t e = token.find("\"", s);

            // add local variable
            if (e != std::string::npos) {
                identifier = token.substr(s, e - s);
                locals.insert(identifier);
            }
        }           
    }

    // traverse the children
    for (auto child : node->children)
        generate_vars(child);
}


/**
 * ------------------------------------------
 *  Extracts first variable/value from node  
 * 
 *  @param node   : the current node
 *  @param option : what we want to extract
 *                  0 for name, 1 for value
 * 
 *  @return       : a variable name or value
 * ------------------------------------------
*/
std::string Generator::identify(Node* node, int option) {
    // output string
    std::string output = "";

    for (const auto& token : node->tokens) {
        if (option == 0) {
            // extract identifier name
            size_t position = token.find("identifier: \"");
            
            if (position != std::string::npos) {
                size_t start = position + 13;
                size_t end = token.find("\"", start);
                
                if (end != std::string::npos) {
                    output = token.substr(start, end - start);
                    break; 
                }
            }
        } 
        else {
            // extract identifier value
            size_t position = token.find("integer:");

            if (position != std::string::npos) {
                size_t spacePos = token.find(" ", position + 8);
                
                if (spacePos != std::string::npos) {
                    output = token.substr(spacePos + 1);
                    size_t spaceAfterInteger = output.find(" ");

                    if (spaceAfterInteger != std::string::npos) 
                        output = output.substr(0, spaceAfterInteger);
                    
                    break; 
                }
            }
        }
    }

    return output;
}


/**
 * ------------------------------------------
 *    Returns a unique temporary variable   
 * 
 *  @return : unique name
 * ------------------------------------------
*/
std::string Generator::get_temp() {
    // generate name based on counter value
    std::string output = "";
    output = "temp" + std::to_string(temp_counter);
    temp_counter ++;
    return output;
}


/**
 * ------------------------------------------
 *       Determines all values of 
 *       the node and its children
 * 
 *  @param : the current node
 * ------------------------------------------
*/
void Generator::get_values(Node* node) {
    // locate <R> nonterminal
    if (node->label == "<R>" && node->tokens.size() > 0) {

        // check if the token contains "integer" or "identifier"
        size_t pos_a = node->tokens[0].find("integer");
        size_t pos_b = node->tokens[0].find("identifier");
            
        // extract integer value
        if (pos_a != std::string::npos) {
            size_t p = node->tokens[0].find(":", pos_a);

            if (p != std::string::npos) {
                std::string s = node->tokens[0].substr(p + 1);
                int integer = std::stoi(s);
                
                // store value
                _values.push_back(std::to_string(integer));
            }
        } 
        else {
            // extract variable value from token
            size_t p = node->tokens[0].find("\"", pos_b);
            
            if (p != std::string::npos) {
                size_t e = node->tokens[0].find("\"", p + 1);

                if (e != std::string::npos) {
                    std::string identifier = node->tokens[0].substr(p + 1, e - p - 1);
                    
                    // store value
                    _values.push_back(identifier);
                }
            }
        }
    }
        
    // traverse the children
    for (auto child : node->children)
        get_values(child);
}


/**
 * ------------------------------------------
 *      Outputs the generated assembly
 * 
 *  @param prefix : output file name
 * ------------------------------------------
*/
void Generator::output(const std::string& prefix) {
    // get the base filename
    size_t pos = prefix.find_last_of("/\\");
    std::string file = (pos != std::string::npos) ? prefix.substr(pos + 1) : prefix;

    // remove existing extension
    size_t dotPos = file.find_last_of(".");
    if (dotPos != std::string::npos)
        file = file.substr(0, dotPos);
    
    // create output file from the prefix
    file += ".asm";
    std::ofstream outputFile(file);

    // check output file
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << file << " for writing." << std::endl;
        return;
    }

    // output each line of generated code to the file (outputs other assembly)
    for (const std::string& line : assembly)
        outputFile << line << std::endl;

    // close the file
    outputFile.close();
}
