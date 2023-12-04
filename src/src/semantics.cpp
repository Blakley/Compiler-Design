/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
        Semantic function declarations
*/

# include "../headers/semantics.h"
# include <iostream>


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

    // we've now checked tree definitions
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
        // check for identifiers defined in the <varList> nonterminal (under <vars>)
        if (node->label == "<varList>") {
            // extract defined identifiers
            for (const auto& token : node->tokens) {
                // search for an identifer in the token string
                std::string identifier;
                std::string line;

                size_t identifierPos = token.find("identifier: \"");
                size_t linePos = token.find(" line: ");

                if (identifierPos != std::string::npos && linePos != std::string::npos) {
                    identifier = token.substr(identifierPos + 13, linePos - (identifierPos + 13));
                    line = token.substr(linePos + 7);
                    
                    // multiple definitions (already present in symbol table)
                    if (verify(identifier))
                        error("Variable: \"" + identifier + " at line " + line + ", previously defined");

                    insert(identifier);
                } 
            }
        }
    } 
    else {
        // check for variable uses and verify them against the symbol table
        
        // variable usage happens in the following nonterminals (under <stat>)
        if (node->label == "<in>" || node->label == "<R>") {
            
            // extract used identifier
            for (const auto& token : node->tokens) {
                // search for an identifer in the token string
                std::string identifier;
                std::string line;

                size_t identifierPos = token.find("identifier: \"");
                size_t linePos = token.find(" line: ");

                // verify if the variable is in the symbol table           
                if (identifierPos != std::string::npos && linePos != std::string::npos) { 
                    identifier = token.substr(identifierPos + 13, linePos - (identifierPos + 13));
                    line = token.substr(linePos + 7);

                    // undefined variable (those not in symbol table)
                    if (!verify(identifier))
                        error("Undefined variable: \"" + identifier + ", referenced at line: " + line);
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
