/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
        Header file for static semantics class
*/

# ifndef SEMANTICS_H
# define SEMANTICS_H

# include "tree.h"
# include <string>
# include <iostream>
# include <unordered_set>

class Semantics {
    private:
        Node* root;         // root node of tree
        bool definitions;   // flag for if we are checking tree for variable definitions
        std::unordered_set<std::string> variables; // variable symbol table

    public:
        Semantics(Node* r);  // Constructor
        ~Semantics();        // Destructor

        void insert(const std::string& identifier);         // insert variable into symbol table
        bool verify(const std::string& identifier) const;   // check if a variable is in the symbol table
        
        void static_semantics();    // Performs static semantics check
        void traverse(Node* n);     // Traverses the AST 
        void error(std::string m);  // Semantics error handler 
};

# endif  // SEMANTICS_H


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

    

    */