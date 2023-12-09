/*
    Name: Anthony Blakley
    Date: 12/09/2023
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
        int temp_counter;                   // counter used to generate unique variables
        int label_counter;                  // counter used to generate unique labels
        
        std::vector<std::string> _operators;   // program operators
        std::vector<std::string> _relationals; // program relational operators
        std::vector<std::string> _values;      // list of node values

        std::set<std::string> locals;       // local variables
        std::vector<std::string> assembly;  // store generated assembly code

    public:
        Generator(Node* root);  // Constructor 
        ~Generator();           // Destructor 

        void configure();          // setup appropriate program operators
        void get_values(Node* n);  // gets all values in expression

        std::string get_temp();                    // returns a unique temporary variable name
        std::string identify(Node* n, int o);      // returns an identifier or value
        
        void generate(Node* n);             // starts code generation    
        void generate_xin(Node* n);         // handles reading in a value
        void generate_xout(Node* n);        // handles outputting a value
        void generate_varList(Node* n);     // handles variable assignment code
        int  generate_exp(Node* n);         // handle getting the output of an expression
        void generate_vars(Node* n);        // handles creating the local variables
        void generate_xclose();             // handles the closing assembly code

        void output(const std::string& fileName);  // outputs generated code to a file
};

#endif  // GENERATOR_H  