/*
    Name: Anthony Blakley
    Date: 12/03/2023
    Description: 
        Header file for code generator class
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "tree.h"
#include <fstream>
#include <vector>
#include <set>

class Generator {
    private:
        Node* root;                         // root of AST
        int label_counter;                  // counter used to generate unique labels
        
        std::string operation;              // current operation
        std::string expression;             // current generated expression
        
        std::set<std::string> locals;       // local variables
        std::vector<std::string> assembly;  // store generated assembly code
    
    public:
        Generator(Node* root);  // Constructor 
        ~Generator();           // Destructor 

        std::string identify(Node* n);             // returns an identifier
        void generate(Node* n);                    // starts code generation    
        void output(const std::string& fileName);  // outputs generated code to a file
        

        void generate_xin(Node* n);         // handles reading in a value
        void generate_xout(Node* n);        // handles outputting a value
        void generate_vars(Node* n);        // handles creating the local variables
        void generate_xclose();             // handles the closing assembly code

        void generate_xcond(Node* n);       // 
        void generate_xloop(Node* n);       //
                

};

#endif  // GENERATOR_H  