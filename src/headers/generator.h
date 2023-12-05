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
        std::string expression;             // current generated expression
        std::set<std::string> locals;       // local variables
        std::vector<std::string> assembly;  // store generated assembly code
    
    public:
        Generator(Node* root);  // Constructor 
        ~Generator();           // Destructor 

        std::string identify(Node* n);             // returns an identifier

        void generate(Node* n);                    // starts code generation
        void output(const std::string& fileName);  // outputs generated code to a file

        void generate_xin(Node* n);         // 
        void generate_xout(Node* n);        //
        



        void generate_variables(Node* n);   // 
        void generate_xcond(Node* n);       // 
        void generate_expression(Node* n);  //
        void generate_addition(Node* n);    //
        void generate_subtraction(Node* n); //
        void generate_term(Node* n);       //

        void generate_xclose();             // 

};

#endif  // GENERATOR_H  