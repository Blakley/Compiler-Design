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
Generator::Generator(Node* root) : root(root) {
    // constructor function
    generate(root);
}


/**
 * ------------------------------------------
 *              Destructor
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
    if (node == nullptr)
        return;

    // ============================
    //  handle <input> nonterminal
    // ============================

    // extract defined identifiers (local variable)
    if (node->label == "<in>") {
        for (const auto& token : node->tokens) {
            // search for an identifer in the token string
            std::string identifier;
            size_t position = token.find("identifier: \"");

            // extract variable name   
            if (position != std::string::npos) {
                size_t s = position + 13;
                size_t e = token.find("\"", s);

                // add local variable
                if (e != std::string::npos) {
                    identifier = token.substr(s, e - s);
                    
                    // convert to uppercase
                    std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::toupper);
                    locals.insert(identifier);
                }
            }           
        }
    }

    // ==============================
    //  handle <varList> nonterminal
    // ===============================

    // extract defined identifiers (local variable)
    if (node->label == "<varList>") {
        // extract defined identifiers
        for (const auto& token : node->tokens) {
            // search for an identifer in the token string
            std::string identifier;
            size_t position = token.find("identifier: \"");

            // extract variable name   
            if (position != std::string::npos) {
                size_t s = position + 13;
                size_t e = token.find("\"", s);

                // add local variable
                if (e != std::string::npos) {
                    identifier = token.substr(s, e - s);

                    // convert to uppercase
                    std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::toupper);
                    locals.insert(identifier);
                }
            }    
        }
    }

    // traverse the children
    for (auto child : node->children)
        generate(child);


    /*
        Notes:
    
        create local variable list structure:
        variables that will be defined in assembly code

        create stack variable list structure:
        variables that will be added into the stack in the 
        assembly code we'll use this structure whenever possible   
    */

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

    // output each line of generated code to the file
    for (const std::string& line : assembly)
        outputFile << line << std::endl;


    // =====================
    //  add local variables
    // =====================
    for (const std::string& var : locals)
        outputFile << var << std::endl;


    // close the file
    outputFile.close();
}
