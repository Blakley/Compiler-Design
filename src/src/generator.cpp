/*
    Name: Anthony Blakley
    Date: 12/10/2023
    Description: 
        Generator function declarations
*/

# include "../headers/generator.h"
# include <algorithm>
# include <map>

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
 *               Destructor
 * ------------------------------------------
*/
Generator::~Generator() {
    // destructor function
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
     *  handle <varList> nonterminal:
     *  <varList> -> identifier : integer ; | identifier : integer <varList>
     * ===============================
    */
    if (node->label == "<varList>")
        generate_varList(node);

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
     *  handle <assign> nonterminal:
     *  <assign> -> xlet identifier <exp> ;
     * ===============================
    */
    if (node->label == "<assign>")
        generate_assign(node);     

    /**
     * ===============================
     *  handle <if> nonterminal:
     *  <if> -> xcond [ <exp> <RO> <exp> ] <stat> 
     * ===============================
    */
    if (node->label == "<if>")
        generate_if(node);

    /**
     * ===============================
     *  handle <loop> nonterminal:
     *  <loop> -> xloop [ <exp> <RO> <exp> ] <stat> 
     * ===============================
    */
    if (node->label == "<loop>")
        generate_loop(node);


    // traverse the children
    for (auto child : node->children)
        generate(child);
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
    // get evaluated expression
    std::string output = generate_exp(node->children[0]);

    // generate assembly
    assembly.push_back("WRITE " + output);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *       output statement (<assign>)
 * 
 * @param node: current node
 * ------------------------------------------
*/
void Generator::generate_assign(Node* node) {
    // get result identifier
    std::string identifier = identify(node, 0);

    // get evaluated expression
    std::string output = generate_exp(node->children[0]);

    // generate assembly, copy the value of output into identifier
    assembly.push_back("COPY " + identifier + " " + output);
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *          current <exp> node
 * 
 * @param node : current node
 * @return     : temporary variable
 * ------------------------------------------
*/
std::string Generator::generate_exp(Node* node) {
    // case 1: single value
    if (node->tokens.size() == 1)
        return node->tokens[0];
    
    // case 2: multi-level expression

    // ========================
    //     setup variables
    //=========================

    // create new temporary variable
    std::string temporary = get_temp();

    // stores the unconsumed operator
    std::string unconsumed_operator = "";

    // stores the previous value
    std::string previous_value = "";

    /*
        create structure to hold stack elements
        useful for nested expressions ( <exp> )
    */
    struct stack_data {
        // stores the inner expression's unconsumed operator
        std::string unconsumed_operator = "";

        // stores the previous inner value
        std::string previous_value = "";

        // stores a temporary variable for the inner expressions
        std::string temporary = ""; 
    };

    // stack to handle nested expressions    
    std::vector<stack_data> _stack;

    // ========================
    //   evaluate expression
    //=========================

    // optimize expression
    optimize(node->tokens);

    // evaluate expression & generate required assembly
    for (auto value : node->tokens) {

        // determine expression value
        if (value == "~") {
            // negate accumulator
            assembly.push_back("MULT -1"); 
        }
        else if (value == "+" || value == "-" || value == "*" || value == "/") {
            
            if (!_stack.empty()) {
                // operator is an inner expression operator
                _stack[_stack.size() - 1].unconsumed_operator = 
                    (value == "+") ? "ADD" :
                    (value == "-") ? "SUB" :
                    (value == "*") ? "MULT" : "DIV";
            }
            else {
                // operator is for outter expression
                unconsumed_operator =  
                    (value == "+") ? "ADD" :
                    (value == "-") ? "SUB" :
                    (value == "*") ? "MULT" : "DIV";
            }
        }
        else if (value == "(") {
            // start of new inner expression
            std::cout << "About to create a new inner expression\n"; 
            
            // store the current expression
            if (!_stack.empty())  
                // store in outter temporary variable
                assembly.push_back("STORE " + _stack[_stack.size() - 1].temporary);
            else
                // store expression in outter-most temporary variable
                assembly.push_back("STORE " + temporary);
                
            // create new temporary variable for inner expression
            struct stack_data data;
            data.temporary = get_temp();
            _stack.push_back(data);

            std::cout << "Created temporary variable: " << data.temporary << ", for an inner expression\n";
        }
        else if (value == ")") {
            // end of an inner expression            
            std::cout << "An inner expression was evaluated\n"; 

            // store result of inner expression it its corresponding temporary variable
            assembly.push_back("STORE " + _stack[_stack.size() - 1].temporary);

            // load original temporary variable back into accumulator
            if (_stack.size() > 1) {
                // load previous outter expression's temporary variable into accumulator
                assembly.push_back("LOAD " + _stack[_stack.size() - 2].temporary);

                // perform operation with previous expression and evaluated expression
                assembly.push_back(_stack[_stack.size() - 2].unconsumed_operator + " " + _stack[_stack.size() - 1].temporary);

                std::cout << "Performing operation with previous expression and an inner expression ";
                std::cout << "using outter operation: " << _stack[_stack.size() - 2].unconsumed_operator << ", and inner expression value: " << _stack[_stack.size() - 1].temporary << "\n"; 
            }
            else {
                // load original temporary variable back into accumulator
                assembly.push_back("LOAD " + temporary);

                // perform operation with previous expression (in accumulator) and evaluated expression
                assembly.push_back(unconsumed_operator + " " + _stack[_stack.size() - 1].temporary);

                std::cout << "Performing operation with previous expression and an inner expression ";
                std::cout << "using outter operation: " << unconsumed_operator << ", and inner expression value: " << _stack[_stack.size() - 1].temporary << "\n"; 
            }
            
            // remove expression
            _stack.pop_back();
        }
        else {
            // handle identifier and integer values
            if (!_stack.empty()) {
                // evalute inner expression
                std::cout << "evaluating an inner expression\n"; 

                // load first value into accumulator
                if (_stack[_stack.size() - 1].previous_value.empty()) {
                    std::cout << "loading first value of an inner expression into the accumulator\n";

                    // load first inner expression value into the accumulator
                    assembly.push_back("LOAD " + value);
                }
                else {
                    // apply the inner expression's unconsumed operator to the current value
                    assembly.push_back(_stack[_stack.size() - 1].unconsumed_operator + " " + value);

                    // reset inner_unconsumed operator
                    _stack[_stack.size() - 1].unconsumed_operator = "";
                }

                // update previous_inner
                _stack[_stack.size() - 1].previous_value = value;
            }
            else {
                // evaluate outter-most expression
                std::cout << "evaluating outter-most expression\n"; 

                // load first value into the accumulator
                if (previous_value.empty())
                    assembly.push_back("LOAD " + value); 
                else {
                    // apply the unconsumed operator to the current value
                    assembly.push_back(unconsumed_operator + " " + value);

                    // reset the unconsumed operator
                    unconsumed_operator = "";
                }

                // update previous_value
                previous_value = value;  
            }
        }
    }

    // store complete evaluated expression, from accumulator, in temporary variable 
    assembly.push_back("STORE " + temporary);
    return temporary;
}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *          current <if> node
 * 
 * @param node : current node
 * ------------------------------------------
*/
void Generator::generate_if(Node* node) {
    // <if> -> xcond <exp> <RO> <exp> <stat>

    // get first evaluated expression
    std::string a = generate_exp(node->children[0]);

    // load first part of expression into the accumulator
    assembly.push_back("LOAD " + a);

    // get relational operator
    std::string r = node->children[1]->tokens[0];

    // get second evaluated expression
    std::string b = generate_exp(node->children[2]);

    std::cout << "first expression: " << a << ", relational operator: " << r << ", second expression: " << b << "\n";

    // generate labels for branching
    std::string label_a = get_label("");
    std::string label_b = get_label("end");

    // 


    /*

        once we encounter if, we need to generate label for if the statement is both true and false
        before we evalute if the condition is true, we need to generate the code for both and only 
        then jump to the appropriate branch

        if condition is true:
            
            navigate to the <stat> child of the <if> node
            perform/(generate assembly) the necessary operations on the <stat> children,
            create a list of these nodes so we can skip traversing them in the generate function

        if condition is false:

            jump to label_false which should continue the execution of the program as normal


        alternative method for handling <if> :
            generate all code as normal, then iterate tree again, this time
            finding all conditional nodes, <if> and inserting conditional assembly & labels
            whereever necessary
    */

}


/**
 * ------------------------------------------
 *     Generates assembly code for the 
 *          current <loop> node
 * 
 * @param node : current node
 * ------------------------------------------
*/
void Generator::generate_loop(Node* node) {
    // todo:
}


/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *   (<xclose>) section of the program
 * ------------------------------------------
*/
void Generator::generate_xclose() {   
    // append STOP instruction
    assembly.push_back("STOP");

    // generate local variables
    generate_vars(root);

    // add local variables after STOP and initialize to 0
    for (const std::string& var : locals)
        assembly.push_back(var + " 0");
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
    // generate name based on temp counter
    std::string output = "";
    output = "temp" + std::to_string(temp_counter);
    temp_counter ++;

    // add temporary value to locals
    locals.insert(output);

    return output;
}


/**
 * ------------------------------------------
 *       Returns a unique label name  
 * 
 *  @param  : label type identifier 
 *  @return : unique name
 * ------------------------------------------
*/
std::string Generator::get_label(std::string s) {
    // generate name based on label counter
    std::string output = "";

    if (s == "")
        // normal label
        output = "label" + std::to_string(label_counter);
    else
        // ending label
        output = "label_end" + std::to_string(label_counter);

    label_counter ++;
    return output;
}


/**
 * ------------------------------------------
 *    Removes redundancy in an expression   
 * ------------------------------------------
 */
void Generator::optimize(std::vector<std::string>& tokens) {
    // vector to store the optimized expression
    std::vector<std::string> optimized_exp;
    
    // flag to track consecutive dashes
    bool is_consecutive = false;

    // counter for consecutive dashes
    int consecutive = 0;

    // iterate through each token in the input expression
    for (const std::string& token : tokens) {
        // check if the current token is a dash
        if (token == "-") {
            // Increment the consecutive dash counter
            consecutive++;
            is_consecutive = true;
        } 
        else {
            // check if the previous tokens formed a consecutive dash sequence
            if (is_consecutive) {
                // if the number of consecutive dashes is even, replace with '+', else with '-'
                if (consecutive % 2 == 0)
                    optimized_exp.push_back("+");
                else
                    optimized_exp.push_back("-");
                
                // reset consecutive dash counter and flag
                consecutive = 0;
                is_consecutive = false;
            }

            // add the current token to the optimized expression
            optimized_exp.push_back(token);
        }
    }

    // handle the case where consecutive dashes end at the end of the vector
    if (is_consecutive) {
        if (consecutive % 2 == 0)
            optimized_exp.push_back("+");
        else 
            optimized_exp.push_back("-");
    }

    // update the input vector with the optimized expression
    tokens = optimized_exp;
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
