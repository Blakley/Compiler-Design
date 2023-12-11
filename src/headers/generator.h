/*
    Name: Anthony Blakley
    Date: 12/11/2023
    Description: 
        Header file for code generator class
*/

# ifndef GENERATOR_H
# define GENERATOR_H

# include "tree.h"
# include <fstream>
# include <vector>
# include <tuple>
# include <set>
# include <map>

class Generator {
    private:
        Node* root;                         // root of AST
        int temp_counter;                   // counter used to generate unique variables
        int label_counter;                  // counter used to generate unique labels

        std::map<std::string, int> if_labels;  // stores the end labels of if statements
        std::map<std::pair<std::string, std::string>, int> loop_labels; // stores the end labels of loop statements
        
        std::set<std::string> locals;       // local variables
        std::vector<std::string> assembly;  // store generated assembly code

    public:
        Generator(Node* root);  // Constructor 
        ~Generator();           // Destructor 

        std::string get_temp();               // returns a unique temporary variable name
        std::string get_label(std::string s); // returns a unique label name
        std::string identify(Node* n, int o); // returns an identifier or value

        void generate(Node* n);             // starts code generation    
        void generate_xin(Node* n);         // handles reading in a value
        void generate_xout(Node* n);        // handles outputting an expression
        void generate_assign(Node* n);      // handles expression assignment
        void generate_varList(Node* n);     // handles variable assignment code
        void generate_if(Node* n);          // handles expression cases
        void generate_loop(Node*);          // handles expression iterations
        std::string generate_exp(Node* n);  // handle getting the output of an expression
        void generate_vars(Node* n);        // handles creating the local variables
        void generate_xclose();             // handles the closing assembly code

        void optimize(std::vector<std::string>& t); // optimize expression operators
        void output(const std::string& fileName);   // outputs generated code to a file
};

#endif  // GENERATOR_H  