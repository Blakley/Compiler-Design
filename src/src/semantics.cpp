/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
        Semantic function declarations
*/

# include "../headers/semantics.h"
# include <iostream>
# include <regex>


/**
 * ------------------------------------------
 *              Constructor
 * 
 * @param r : root node of AST
 * ------------------------------------------
*/
Semantics::Semantics(Node* r) : root(r) {
    // constructor function

    // set tree definitions check to true
    definitions = true;
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Semantics::~Semantics() {
    // destructor function
}


/**
 * ------------------------------------------
 *     Insert Variable into Symbol Table
 * 
 * @param identifier : variable to insert
 * ------------------------------------------
*/
void Semantics::insert(const std::string& identifier) {
    // insert the variable into the symbol table
    variables.insert(identifier);
}


/**
 * ------------------------------------------
 *       Verify Variable in Symbol Table
 * 
 * @param identifier : variable to verify
 * @return bool      : true if found 
 * ------------------------------------------
*/
bool Semantics::verify(const std::string& identifier) const {
    // verify if the variable is in the symbol table
    return variables.find(identifier) != variables.end();
}


/**
 * ------------------------------------------
 *      Perform Static Semantics Check
 * ------------------------------------------
*/
void Semantics::static_semantics() {
    // Traverse the tree for variable definitions
    traverse(root);

    // Output the defined variables
    std::cout << "Defined Variables: ";
    for (const auto& variable : variables) {
        std::cout << variable << " ";
    }
    std::cout << "\n\n";

    // we've checked tree definitions
    definitions = false;

    // Traverse the tree for variable usage
    traverse(root);
}


/**
 * ------------------------------------------
 *  Traverse the Tree for Definitions or Uses
 * 
 * @param node : the current node 
 * ------------------------------------------
*/
void Semantics::traverse(Node* node) {
    if (node == nullptr)
        return;

    // check for variable definitions and insert them into the symbol table
    if (definitions) {
        // check for identifiers defined in the <varList> nonterminal
        if (node->label == "<varList>") {
            // extract defined identifiers
            for (const auto& token : node->tokens) {
                // search for an identifer in the token string
                std::smatch matches;
                std::regex pattern("identifier: \"([^\"]+)\"");
                               
                if (std::regex_search(token, matches, pattern)) {
                    // insert identifier into symbol table
                    std::string identifier = matches[1];
                    insert(identifier);
                } 
            }
        }
    } 
    else {
        // check for variable uses and verify them against the symbol table
        
        // variable usage happens in the following nonterminals
        if (node->label == "<in>" || node->label == "<R>") {
            
            // extract used identifier
            for (const auto& token : node->tokens) {
                // search for an identifer in the token string
                std::smatch matches;
                std::regex pattern("identifier: \"([^\"]+)\" line: (\\d+)");

                // verify if the variable is in the symbol table           
                if (std::regex_search(token, matches, pattern)) {
                    std::string identifier = matches[1];
                    std::string line = matches[2];

                    // report error for undefined variable (those not in symbol table)
                    if (!verify(identifier))
                        error("Undefined variable: \"" + identifier + "\" referenced at line: " + line);
                } 
            }
        }
    }

    // traverse the children
    for (auto child : node->children)
        traverse(child);
}


/**
 * ------------------------------------------
 *         Handle error semantics
 * 
 * @param message  : error message
 * ------------------------------------------
*/
void Semantics::error(std::string message) {
    std::cerr << "[Error], SEMANTICS ERROR: ( " << message << " )" << std::endl;
    exit(EXIT_FAILURE);
}
