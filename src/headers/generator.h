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

class Generator {
    private:
        Node* root;                         // root of AST
        std::vector<std::string> assembly;  // store generated assembly code

    public:
        Generator(Node* root);  // Constructor 
        ~Generator();           // Destructor 

        void generate();                           // starts code generation
        void output(const std::string& fileName);  // outputs generated code to a file
};

#endif  // GENERATOR_H