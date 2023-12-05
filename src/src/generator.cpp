/*
    Name: Anthony Blakley
    Date: 12/03/2023
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
Generator::Generator(Node* root) : root(root), label_counter(0), expression("") {
    // start generating assembly
    generate(root);

    // generate ending assembly
    generate_xclose();
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Generator::~Generator() {
    // clear the locals set
    locals.clear();
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

    // test output
    std::cout << "node: " << node->label << "\n";

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
     *  // handle <out> nonterminal:
     *  <out> -> xout << <exp> ;
     * ===============================
    */
    if (node->label == "<out>") {
        std::cout << "\nnodes of <out>\n";
        generate_xout(node);      
        std::cout << "\n";
    }

    // loop
    // if


    // traverse the children
    for (auto child : node->children)
        generate(child);
}


/**
 * ------------------------------------------
 * Generates assembly code for the 
 * input statement (<in>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xin(Node* node) {
    // get value to READ in
    std::string value = identify(node);

    // append the READ instruction for input
    if (value != "")                     
        assembly.push_back("READ " + value);
}


/**
 * ------------------------------------------
 * Generates assembly code for the 
 * output statement (<out>)
 * 
 * @ TODO
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xout(Node* node) {
    // list of operators used in <out>
    std::vector<std::string> operators;

    // list of variables used in <out>
    std::vector<std::string> values;

    // traverse the children of <out> to collect variables and integers
    


    // traverse the children of <out> to collect operators








    // traverse the children of <out>
    for (auto child : node->children) {
        
        // current node
        Node* current = child;


        /*
        
            1) Extract all the identifiers and or integers that is used in the output (integers)
            2) Extract all the operators used in the output (variables)
            
            3)

            start by storing the first value in integers into the accumulator:
                STORE integers[i]

            next determine if the first operator:
                variables[operator_index]
                convert the operator to the corresponding assembly:
                    if operator == "+", operator = ADD
                    if operator == "-", operator = SUB
                    ...

            next, move to the next value in the integers and before putting it into the accumulator,
            perform the last operation stored in variables

            example ADD: variables[operator_index] integers[value_index]

            keep doing this process until the entire <out> node (all <out> children) have been evaluted and 
            the final value is stored in the accumulator.

            finally, store the value in the accumulator in the variable "temp", which is a variable in the assembly
            code that we've added elsewhere:
                "STORE temp"

            then, output the value:
                "WRITE temp"
        */



    }
    


    return;

    // test output
    std::cout << "<out> child: " << node->label << "\n";

    // determine if value is integer or identifier
    if (node->label == "<R>") {
        // print token
        std::cout << node->tokens[0] << "\n";

        // check if the token contains "integer" or "identifier"
        size_t pos_a = node->tokens[0].find("integer");
        size_t pos_b = node->tokens[0].find("identifier");
        
        // extract int value
        if (pos_a != std::string::npos) {
            
            size_t p = node->tokens[0].find(":", pos_a);
            
            if (p != std::string::npos) {
                std::string s = node->tokens[0].substr(p + 1);
                int integer = std::stoi(s);
                std::cout << "Integer value: " << integer << "\n";

                // add int to Accumulator, first value in <out> expression
                if (operation == "")
                    assembly.push_back("LOAD " + std::to_string(integer));
            }
        } 
        else {
            // extract variable value
            size_t p = node->tokens[0].find("\"", pos_b);
            
            if (p != std::string::npos) {
                
                size_t e = node->tokens[0].find("\"", p + 1);

                if (e != std::string::npos) {
                    std::string varible = node->tokens[0].substr(p + 1, e - p - 1);

                    // add variable to Accumulator, first value in <out> expression
                    if (operation == "")
                        assembly.push_back("LOAD " + varible);

                    std::cout << "Identifier value: " << varible << "\n";
                }
            }
        }
    }

    // traverse the children of <out>
    for (auto child : node->children)
        generate_xout(child);
}






















/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *   (<xclose>) section of the program
 * ------------------------------------------
*/
void Generator::generate_xclose() {
    // push instruction counter
    int push_counter = 0;

    // count the number of PUSH instructions
    for (const std::string& line : assembly) {
        if (line.find("PUSH") != std::string::npos)
            push_counter ++;
    }

    // add POP instructions based on PUSH count before STOP
    for (int i = 0; i < push_counter; ++i)
        assembly.push_back("POP");
    
    // append STOP instruction
    assembly.push_back("STOP");

    // generate local variables
    generate_vars(root);

    // add local variables after STOP and initialize to 0
    for (const std::string& var : locals)
        assembly.push_back(var + " 0");

    // append temp variable
    assembly.push_back("temp 0 ");
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
 *    Extracts first identifier from node  
 * 
 *  @param node : the current node
 *  @return     : an identifier value
 * ------------------------------------------
*/
std::string Generator::identify(Node* node) {
    std::string value = "";

    // find identifiers in the tokens
    for (const auto& token : node->tokens) {
        std::string identifier;
        size_t position = token.find("identifier: \"");

        if (position != std::string::npos) {
            size_t s = position + 13;
            size_t e = token.find("\"", s);

            if (e != std::string::npos) {
                identifier = token.substr(s, e - s);
                return identifier;
            }
        }
    }

    return value;
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
