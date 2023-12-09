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

    // ================================
    //        create assembly
    // ================================

    // get evaluated expression
    std::string output = generate_exp(node->children[0]);

    // generate assembly
    assembly.push_back("WRITE " + output);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *          current <exp> node
 * 
 * @param node : current node
 * @return     : temporary variable
 * ------------------------------------------
*/
std::string Generator::generate_exp(Node* node) {
    // case 1: single value
    if (node->tokens.size() == 1)
        return node->tokens[0];
    
    // case 2: multi-level expression

    // ========================
    //     setup variables
    //=========================

    // create new temporary variable
    std::string temporary = get_temp();

    // stores a temporary variable for the inner expressions
    std::string inner_temporary = "";

    // stores the unconsumed operator
    std::string unconsumed_operator = "";

    // stores the previous value
    std::string previous_value = "";

    // stores the number of consecutive "-" signs
    // todo:
    // int consecutive = 0;
    
    // expression index & expression size
    // int index = 0;
    // int size = node->tokens.size();

    // ========================
    //   evaluate expression
    //=========================

    // evaluate expression & generate required assembly
    for (auto value : node->tokens) {
        // determine expression value
        if (value == "~") {
            // negate accumulator
            assembly.push_back("MULT -1"); 
        }
        else if (value == "+" || value == "-" || value == "*" || value == "/") {
            // ========================
            //   handle operators
            //=========================
            
            // value is an operator
            unconsumed_operator =  
                (value == "+") ? "ADD" :
                (value == "-") ? "SUB" :
                (value == "*") ? "MULT" : "DIV";
        }        
        else if (value == "(") {
            // todo : (later)
            // create temporary variable for inner expression
        }
        else if (value == ")") {
            // todo: (later)
            // stop evaluating inner expresion
        }
        else {
            // ===============================
            //   handle identifers & integers
            //================================

            if (previous_value.empty()) {
                // load first value into the accumulator
                assembly.push_back("LOAD " + value); 
            }
            else {
                // apply the unconsumed operator to the previous value
                assembly.push_back(unconsumed_operator + " " + previous_value);

                // reset the unconsumed operator
                unconsumed_operator = "";
            }

            // update previous_value
            previous_value = value;
        }
    }

    // store evaluated expression, from accumulator, in temporary variable 
    assembly.push_back("STORE " + temporary);
    return temporary;
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

    // add temporary value to locals
    locals.insert(output);

    return output;
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
