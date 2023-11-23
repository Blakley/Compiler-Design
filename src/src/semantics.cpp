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

    // Perform logic based on the definitions flag
    if (definitions) {
        // Check for variable definitions and insert them into the symbol table
        // todo:
    } 
    else {
        // Check for variable uses and verify them against the symbol table
        // todo:
    }

    // Recursively traverse the children
    for (auto child : node->children)
        traverse(child);
}



    /*
            ===================
            Project 3 --> TODO:
            ===================

            Goal, implemenet static semantics: proper definition and use of variables
            
            * A variable has to be defined anywhere in the program to be used (must satisfy syntax), before or after use

                1. variable definition: any identifier listed in any <varList> is definition

                2. variable use: identifier showing up in any statement
                
                * rules:
                    no multiple variables with same name and a variable used must be defined

                    There can be two kinds of errors: 
                        1. multiple variables with the same name in the same scope 
                        2. undefined variable
        

            * Variable name can only be defined once in a scope but can be reused in another scope
    
            * There is only one scope, the global scope, regardless of where a variable is defined
    
            * In main.cpp, call tree.semantics(tree.root_node)
                It just does a tree traversal (like the traverse function) (might need to perform 2 traversals)
                

    */