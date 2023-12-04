/*
    Name: Anthony Blakley
    Date: 12/03/2023
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
        bool definitions;   // checking tree for variable definitions or variable usage
        std::unordered_set<std::string> variables; // variable symbol table

    public:
        Semantics(Node* r);  // Constructor
        ~Semantics();        // Destructor

        void insert(const std::string& identifier);         // insert variable into symbol table
        bool verify(const std::string& identifier) const;   // check if a variable is in the symbol table
        
        void static_semantics();    // performs static semantics check
        void traverse(Node* n);     // traverses the AST 
        void error(std::string m);  // semantics error handler 
};

# endif  // SEMANTICS_H