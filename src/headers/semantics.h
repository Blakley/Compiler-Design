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
        
        void static_semantics(); // Performs static semantics check
        void traverse(Node* n);  // Traverses the AST 
};

# endif  // SEMANTICS_H