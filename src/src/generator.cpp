/*
    Name: Anthony Blakley
    Date: 12/03/2023
    Description: 
        Generator function declarations
*/

#include "../headers/generator.h"
#include <algorithm>


/**
 * ------------------------------------------
 *              Constructor
 * 
 * @param r : root node of AST
 * ------------------------------------------
*/
Generator::Generator(Node* root) : root(root), temp_counter(0), label_counter(0) {
    // start generating assembly
    generate(root);

    // generate ending assembly
    generate_xclose();
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Generator::~Generator() {
    // clear the locals set
    locals.clear();
}


/**
 * ------------------------------------------
 *  Traverse the Tree to generate the .asm
 * 
 * @param node : the current node 
 * ------------------------------------------
*/
void Generator::generate(Node* node) {
    // ensure the node is not null
    if (node == nullptr)
        return;

    /**
     * ===============================
     *  handle <in> nonterminal:
     *  <in> -> xin >> identifier ; 
     * ===============================
    */
    if (node->label == "<in>")
        generate_xin(node);

    /**
     * ===============================
     *  handle <out> nonterminal:
     *  <out> -> xout << <exp> ;
     * ===============================
    */
    if (node->label == "<out>")
        generate_xout(node);     

    /**
     * ===============================
     *  handle <varList> nonterminal:
     *  <varList> -> identifier : integer ; | identifier : integer <varList>
     * ===============================
    */
    if (node->label == "<varList>")
        generate_varList(node);

    /**
     * ===============================
     *  handle <exp> nonterminal:
     *  <exp> -> <M> / <exp> | <M> * <exp> | <M> 
     * ===============================
    */
    if (node->label == "<exp>")
        generate_exp(node);


    // if (node->label == "<R>")
    //     generate_R(node);

    if (node->label == "<N>")
        generate_N(node);


    // traverse the children
    for (auto child : node->children)
        generate(child);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *         input statement (<in>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xin(Node* node) {
    // get value to READ in
    std::string value = identify(node, 0);

    // append the READ instruction for input
    if (value != "")                     
        assembly.push_back("READ " + value);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<out>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_xout(Node* node) {
    // collect variables and integers
    // get_values(node);
    
    // // collect operators
    // get_operators(node);

    // ================================
    //        create assembly
    // ================================

    // if (node_operators.size() == 0)
    //     // case 1: output single value
    //     assembly.push_back("WRITE " + node_values[0]);
    // else {
    //     // case 2: output multiple values
    //     // todo:
    // }

    // // // clear values and operators for next potential <out> node
    // node_values.clear();
    // node_operators.clear();
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<varList>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_varList(Node* node) {
    // get identifier name
    std::string identifier = identify(node, 0);
    
    // get identifier value
    std::string value = identify(node, 1);

    // generate assembly
    assembly.push_back("LOAD " + value);
    assembly.push_back("STORE " + identifier);

    // traverse child nodes
    for (auto child : node->children) {
        // skip redundant processing
        if (child->label != "<varList>")
            generate_varList(child);
    }
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<exp>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_exp(Node* node) {
    // determine expression path    
    // if (node->label == "<exp>") {
        
    //     if (node->children.size() == 2) {
    //         // <M> / <exp> or <M> * <exp> production
    //         // <exp> will show up as <M>

    //          std::cout << "from <exp> --> <M> / <exp> or <M> * <exp> production\n";

    //         // generate code for left operand <M>
    //         generate_M(node->children[0]);

    //         std::string temp_var = get_temp();
    //         assembly.push_back("STORE " + temp_var);

    //         // generate code for right operand <M>
    //         generate_M(node->children[1]);

    //         // determine operation based on the operator
    //         std::string op = (node->tokens[0] == "/") ? "DIV" : "MULT";

    //         // generate code to perform the operation using the temporary variable
    //         assembly.push_back(op + " " + temp_var);
    //     }

    //      // <M> production
    //     if (node->children.size() == 1 && node->children[0]->label == "<M>") {
    //         std::cout << "from <exp> --> <M> production\n";
    //         generate_M(node->children[0]);
    //     }
    // }

    // // traverse child nodes
    // for (auto child : node->children)
    //     generate_exp(child);
}


/**
 * ------------------------------------------
 * Generates assembly code for the multiplication
 * 
 * <M> -> <N> + <M> | <N> 
 * ------------------------------------------
 */
void Generator::generate_M(Node* node) {
    // TODO: Implement logic for <M>
}


/**
 * ------------------------------------------
 * Generates assembly code for the subtraction
 * 
 * <N> -> <R> - <N> | ~ <N> | <R> 
 * ------------------------------------------
 */
void Generator::generate_N(Node* node) {
    // determine node type
    std::cout << "\n<N> node: " << node->label << "\n";
    std::cout << "<N> node children: " << node->children.size() << "\n";
    for(auto t: node->tokens)
        std::cout << "<N> node token: " << node->tokens[0] << "\n";
    std::cout << "\n";

    if (node->children.size() == 2) {
        // <R> - <N> production
        // count number of - tokens to determine operation
        int negatives = node->tokens.size();

        std::cout << "generate_N:\t<R> - <N> production\n";
        std::cout << "generate_N:\t<R> - <N> " << negatives << " negative values operations\n";

        if (negatives % 2 == 0) {
            // even amount, make addition instead
            std::cout << "generate_N:\t<R> - <N>, will become addition instead\n";
        }
    }
    else {
        if (!node->tokens.empty()) {
            //  ~ <N> production
            std::cout << "generate_N:\t~ <N> production\n";
        }
        else {
            // <R> production
            std::cout << "generate_N:\tchild node: " << node->children[0]->label << "\n";
            generate_R(node->children[0]);
        }
    }
}


/**
 * ------------------------------------------
 * Generates assembly code for the expression
 * 
 * <R> -> ( <exp> ) | identifier | integer 
 * ------------------------------------------
 */
void Generator::generate_R(Node* node) {
    // determine node type
    std::string value = identify(node, 0);

    if (value == "") {
        // determine if node is integer type
        value = identify(node, 1);

        if (value == "") {
            // <R> is (<exp>)
            std::cout << "generate_R:\tchild node is <exp> type: " << node->children[0]->label << "\n";
            generate_exp(node->children[0]);
        }
        else {
            // <R> is integer
            std::cout << "generate_R:\t <R> Node is integer type: " << node->label << "\n";
        }
    }
    else {
        // <R> is identifier
        std::cout << "generate_R :\t <R> Node is identifier type: " << node->label << "\n";
    }
}


/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *   (<xclose>) section of the program
 * ------------------------------------------
*/
void Generator::generate_xclose() {
    // push instruction counter
    int push_counter = 0;

    // count the number of PUSH instructions
    for (const std::string& line : assembly) {
        if (line.find("PUSH") != std::string::npos)
            push_counter ++;
    }

    // add POP instructions based on PUSH count before STOP
    for (int i = 0; i < push_counter; ++i)
        assembly.push_back("POP");
    
    // append STOP instruction
    assembly.push_back("STOP");

    // generate local variables
    generate_vars(root);

    // add local variables after STOP and initialize to 0
    for (const std::string& var : locals)
        assembly.push_back(var + " 0");

    // append temp variable
    // assembly.push_back("temp 0 ");
}


/**
 * ------------------------------------------
 *      Generates assembly code for the 
 *    local variables in the input program
 * 
 *  @param node : current node
 * ------------------------------------------
*/
void Generator::generate_vars(Node* node) {
    // ensure the node is not null
    if (node == nullptr)
        return;

    // find identifiers
    for (const auto& token : node->tokens) {
        // search for an identifier in the token string
        std::string identifier;
        size_t position = token.find("identifier: \"");

        // extract variable name   
        if (position != std::string::npos) {
            size_t s = position + 13;
            size_t e = token.find("\"", s);

            // add local variable
            if (e != std::string::npos) {
                identifier = token.substr(s, e - s);
                locals.insert(identifier);
            }
        }           
    }

    // traverse the children
    for (auto child : node->children)
        generate_vars(child);
}


/**
 * ------------------------------------------
 *  Extracts first variable/value from node  
 * 
 *  @param node   : the current node
 *  @param option : what we want to extract
 *                  0 for name, 1 for value
 * 
 *  @return       : a variable name or value
 * ------------------------------------------
*/
std::string Generator::identify(Node* node, int option) {
    // output string
    std::string output = "";

    for (const auto& token : node->tokens) {
        if (option == 0) {
            // extract identifier name
            size_t position = token.find("identifier: \"");
            
            if (position != std::string::npos) {
                size_t start = position + 13;
                size_t end = token.find("\"", start);
                
                if (end != std::string::npos) {
                    output = token.substr(start, end - start);
                    break; 
                }
            }
        } 
        else {
            // extract identifier value
            size_t position = token.find("integer:");

            if (position != std::string::npos) {
                size_t spacePos = token.find(" ", position + 8);
                
                if (spacePos != std::string::npos) {
                    output = token.substr(spacePos + 1);
                    size_t spaceAfterInteger = output.find(" ");

                    if (spaceAfterInteger != std::string::npos) 
                        output = output.substr(0, spaceAfterInteger);
                    
                    break; 
                }
            }
        }
    }

    return output;
}


/**
 * ------------------------------------------
 *    Returns a unique temporary variable   
 * 
 *  @return : unique name
 * ------------------------------------------
*/
std::string Generator::get_temp() {
    // generate name based on counter value
    std::string output = "";
    output = "temp" + std::to_string(temp_counter);
    temp_counter ++;
    return output;
}


/**
 * ------------------------------------------
 *       Determines all values of 
 *       the node and its children
 * 
 *  @param : the current node
 * ------------------------------------------
*/
void Generator::get_values(Node* node) {
    // locate <R> nonterminal
    if (node->label == "<R>") {

        // check if the token contains "integer" or "identifier"
        size_t pos_a = node->tokens[0].find("integer");
        size_t pos_b = node->tokens[0].find("identifier");
        
        // extract integer value
        if (pos_a != std::string::npos) {
            size_t p = node->tokens[0].find(":", pos_a);

            if (p != std::string::npos) {
                std::string s = node->tokens[0].substr(p + 1);
                int integer = std::stoi(s);
                
                // store value
                node_values.push_back(std::to_string(integer));
            }
        } 
        else {
            // extract variable value from token
            size_t p = node->tokens[0].find("\"", pos_b);
            
            if (p != std::string::npos) {
                size_t e = node->tokens[0].find("\"", p + 1);

                if (e != std::string::npos) {
                    std::string identifier = node->tokens[0].substr(p + 1, e - p - 1);
                    
                    // store value
                    node_values.push_back(identifier);
                }
            }
        }
    }
        
    // traverse the children
    for (auto child : node->children)
        get_values(child);
}


/**
 * ------------------------------------------
 *       Determines all operators of 
 *        the node and its children
 * 
 *  @param : the current node
 * ------------------------------------------
*/
void Generator::get_operators(Node* node) {
    // locate nodes containing operators
    if (node->label == "<exp>" 
    ||  node->label == "<M>"     
    ||  node->label == "<N>") {
        for (auto t : node->tokens)
            node_operators.push_back(t);
    }

    // traverse the children
    for (auto child : node->children)
        get_operators(child);
}


/**
 * ------------------------------------------
 *      Outputs the generated assembly
 * 
 *  @param prefix : output file name
 * ------------------------------------------
*/
void Generator::output(const std::string& prefix) {
    // get the base filename
    size_t pos = prefix.find_last_of("/\\");
    std::string file = (pos != std::string::npos) ? prefix.substr(pos + 1) : prefix;

    // remove existing extension
    size_t dotPos = file.find_last_of(".");
    if (dotPos != std::string::npos)
        file = file.substr(0, dotPos);
    
    // create output file from the prefix
    file += ".asm";
    std::ofstream outputFile(file);

    // check output file
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << file << " for writing." << std::endl;
        return;
    }

    // output each line of generated code to the file (outputs other assembly)
    for (const std::string& line : assembly)
        outputFile << line << std::endl;

    // close the file
    outputFile.close();
}
