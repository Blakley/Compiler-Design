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
Generator::Generator(Node* root) : root(root), label_counter(0) {
    // constructor function

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
    // destructor function

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

    // std::cout << "node label: " << node->label << std::endl;

    // ===============================
    //  handle <input>   nonterminal
    //  handle <varList> nonterminal
    // ===============================

    if (node->label == "<in>" || node->label == "<varList>")
        generate_variables(node);

    // ===============================
    //    handle <if> nonterminal
    // ===============================    

    if (node->label == "<if>") 
        generate_if(node);

    // traverse the children
    for (auto child : node->children)
        generate(child);
}


/**
 * ------------------------------------------
 * Generates assembly code for the 
 * local variables in the input program
 * 
 *  @param node : the current node
 * ------------------------------------------
*/
void Generator::generate_variables(Node* node) {
    // find identifers
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


/**
 * ------------------------------------------
 * Generates assembly code for the 
 * conditional (if) statement in the AST
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_if(Node* node) {
    // Generate code for the condition
    generate(node->children[0]); // generates code for the left side of the condition
    generate(node->children[2]); // generates code for the right side of the condition

    // subtract the right side from the left side
    assembly.push_back("SUB");

    // use BRNEG to branch if the result is negative
    std::string branchLabel = "Label" + std::to_string(label_counter++);
    assembly.push_back("BRNEG " + branchLabel);

    // generate code for the true branch (if condition is true)
    generate(node->children[1]);

    // insert the branch label
    assembly.push_back(branchLabel + ":");
}


/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *       section of the program
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

    // add local variables after STOP and initialize to 0
    for (const std::string& var : locals)
        assembly.push_back(var + " 0");
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
