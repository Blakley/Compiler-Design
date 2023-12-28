/*
    Name: Anthony Blakley
    Date: 12/20/2023
    Description: 
        Generator function declarations
*/

# include "../headers/generator.h"
# include <algorithm>

/**
 * ------------------------------------------
 *              Constructor
 * 
 * @param r : root node of AST
 * ------------------------------------------
*/
Generator::Generator(Node* root) : root(root), label_counter(0), stack_counter(0) {    
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
     * -------------------------------
     *   handle generating end label
     *       for if branching
     * -------------------------------
    */
    if (!if_labels.empty()) {
        // check if there are end labels to process
        auto it = if_labels.begin();
        while (it != if_labels.end()) {
            if (node->indentation <= it->second) {
                // append label to assembly
                assembly.push_back(it->first + ": NOOP");

                // remove end label from map
                it = if_labels.erase(it);
            }
            else    
                ++ it;
        }
    }    

    /**
     * -------------------------------
     *   handle generating end label
     *      for loop branching
     * -------------------------------
    */
    if (!loop_labels.empty()) {
        // check if there are end labels to process
        auto it = loop_labels.begin();
        while (it != loop_labels.end()) {
            if (node->indentation <= it->second) {
                // append iteration continue assembly
                assembly.push_back("BR " + it->first.first);

                // add label to assembly
                assembly.push_back(it->first.second + ": NOOP");

                // remove from the map
                it = loop_labels.erase(it);
            } 
            else
                ++ it;
        }
    }

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
 *  uses the stack for temporary storage
 * 
 * @param node : current node
 * @return     : updated _stack storage value
 *               or a single value 
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

    // create initial temporary variable
    if (locals.empty())
        locals.insert("stack_T");

    // stack to handle nested expressions    
    std::vector<stack_data> _stack;

    // variables for outter-most expression, outside parenthesis
    std::string outter_previous = "";
    std::string outter_unconsumed = "";

    // ========================
    //   evaluate expression
    //=========================

    // expected outputs for below: 30
    // xout << 2 * ( 5 + 10 ) ;
    // xout << 2 * ( 5 + ( 2 * 5 ) ) ;
    // xout << 2 * ( 4 + ( 2 * 5 ) ) + 2 ;
    // xout << 2 * ( 5 + 10 ) / ( 1 + 0 ) ;
    // xout << 2 * ( 4 + 10 ) / ( 1 + 1 ) + 16 ;
    // xout << 2 * ( 5 + ( 2 * 5 ) ) / ( 1 + 1 ) + 15 ;
    
    // optimize expression
    optimize(node->tokens);

    // evaluate expression & generate required assembly
    for (auto value : node->tokens) {
        if (value == "(") {
            // store current expression onto stack
            shift(_stack, 0);
            assembly.push_back("PUSH");
            assembly.push_back("STACKW 0");
            
            // create new stack data structure
            stack_data data;
            data.index = stack_counter;
            stack_counter ++;

            // add data to stack
            _stack.push_back(data);
        }
        else if (value == ")") {
            // 1) store evaluated expression in stack temporary variable
            assembly.push_back("STORE stack_T");
            
            if (_stack.size() > 1) {
                // 2a) move previous expression into accumulator
                assembly.push_back("STACKR " + std::to_string(_stack[_stack.size() - 2].index - 1));

                // 3a) perform operation with accumulator and stack temporary variable
                assembly.push_back(_stack[_stack.size() - 2].unconsumed + " stack_T");

                // 4a) store expression in appropriate stack variable
                assembly.push_back("STACKW " + std::to_string(_stack[_stack.size() - 2].index - 1));
            }
            else {
                // 2b) move original value, TOS, into accumulator
                assembly.push_back("STACKR 0");
            
                // 3b) perform operation with accumulator and stack temporary variable
                assembly.push_back(outter_unconsumed + " stack_T");

                // 4b) store evaluted output, accumulator, in stack temporary variable
                assembly.push_back("STORE stack_T");
            }
            
            // remove evaluated stack_data structure
            shift(_stack, 1);
            _stack.pop_back();
            assembly.push_back("POP");
        }
        else if (value == "~") {
            // handle negation operator, negate accumulator
            assembly.push_back("MULT -1"); 
        }
        else if (value == "+" || value == "-" || value == "*" || value == "/") {
            // handle operators
            if (!_stack.empty()) {
                // operator is an inner expression operator
                _stack[_stack.size() - 1].unconsumed = 
                    (value == "+") ? "ADD" :
                    (value == "-") ? "SUB" :
                    (value == "*") ? "MULT" : "DIV";
            }
            else {
                // operator is for outter expression
                outter_unconsumed =  
                    (value == "+") ? "ADD" :
                    (value == "-") ? "SUB" :
                    (value == "*") ? "MULT" : "DIV";
            }
        } 
        else {
            // handle identifier and integer values
            if (!_stack.empty()) {
                // evaluate inner expression

                // load first value into the accumulator
                if (_stack[_stack.size() - 1].previus_value.empty()) {
                    // load first inner expression value into the accumulator
                    assembly.push_back("LOAD " + value);
                }
                else {
                    // apply the inner expression's unconsumed operator to the current value
                    assembly.push_back(_stack[_stack.size() - 1].unconsumed + " " + value);

                    // reset inner expressoin's unconsumed operator
                    _stack[_stack.size() - 1].unconsumed = "";
                }

                // update previous inner value
                _stack[_stack.size() - 1].previus_value = value;
            }
            else {
                // evaluate outter-most expression

                // load first value into the accumulator
                if (outter_previous.empty())
                    assembly.push_back("LOAD " + value); 
                else {
                    // apply the unconsumed operator to the current value
                    assembly.push_back(outter_unconsumed + " " + value);

                    // reset the unconsumed operator
                    outter_unconsumed = "";
                }

                // update previous_value
                outter_previous = value;  
            }
        }
    }

    // update final stack temporary variable if necessary
    if (assembly[assembly.size() - 1] != "STORE stack_T")
        assembly.push_back("STORE stack_T");

    // return evaluted expression
    return "stack_T";
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
    // ==========================
    //   get expression values
    // ==========================

    // get first evaluated expression
    std::string a = generate_exp(node->children[0]);

    // get relational operator
    std::string r = node->children[1]->tokens[0];

    // get second evaluated expression
    std::string b = generate_exp(node->children[2]);

    // ==========================
    //  create labels & mapping
    // ==========================

    // generate labels for branching
    std::string label_start = get_label("");
    std::string label_end = get_label("end");

    // create relational branch mapping
    std::map<std::string, std::string> operations = {
        {"<",  "BRNEG"},
        {">",  "BRPOS"},
        {">>", "BRZPOS"},
        {"<<", "BRZNEG"},
        {"%",  "BRZERO"},
        {"=",  "BRZERO"}
    };

    // ==========================
    //     generate assembly
    // ==========================

    // load first part of expression into the accumulator
    assembly.push_back("LOAD " + a);
    
    // perform evaluation
    assembly.push_back("SUB " + b);

    // create label navigation assembly
    if (r == "%")
        assembly.push_back(operations[r] + " " + label_end);   // not equal condition
    else {
        // create label navigation assembly
        assembly.push_back(operations[r] + " " + label_start); // condition
        assembly.push_back("BR " + label_end);                 // jump to label if false
        assembly.push_back(label_start + ": NOOP");            // jump to label if true
    }

    // store reference to end label and indentation
    if_labels.insert({label_end, node->indentation});    
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
    // ==========================
    //   get expression values
    // ==========================

    // get first evaluated expression
    std::string a = generate_exp(node->children[0]);

    // get relational operator
    std::string r = node->children[1]->tokens[0];

    // get second evaluated expression
    std::string b = generate_exp(node->children[2]);

    // generate labels for branching
    std::string label_start = get_label("");
    std::string label_end = get_label("end");

    // create relational branch mapping
    std::map<std::string, std::string> operations = {
        {"<",  "BRNEG"},
        {">",  "BRPOS"},
        {">>", "BRZPOS"},
        {"<<", "BRZNEG"},
        {"%",  "BRZERO"},
        {"=",  "BRZERO"}
    };    

    // relational branch negations
    std::map<std::string, std::string> negations = {
        {"%", "="},
        {"=", "%"},
        {"<", ">>"},
        {">", "<<"},
        {">>", "<"},
        {"<<", ">"}
    };

    // determine negated condition
    std::string negated = operations[negations[r]];

    // ==========================
    //     generate assembly
    // ==========================
    
    // create loop starting label
    assembly.push_back(label_start + ": NOOP");

    // load first part of expression into the accumulator
    assembly.push_back("LOAD " + a);
    
    // perform evaluation
    assembly.push_back("SUB " + b);

    // handle not equal condition
    if (r == "=") {
        // create additional loop exiting
        std::string _label = get_label("");
        assembly.push_back(operations[r] + " " + _label); 
        assembly.push_back("BR " + label_end);
        assembly.push_back(_label + ": NOOP");
    }
    else {
        // break out of loop if negation of condition is true
        assembly.push_back(negated + " " + label_end); 
    }

    // store reference to the loop labels and indentation
    loop_labels.insert({{label_start, label_end}, node->indentation});
}


/**
 * ------------------------------------------
 *   Generates assembly code for closing 
 *   (<xclose>) section of the program
 * ------------------------------------------
*/
void Generator::generate_xclose() {   
    // push instruction counter
    int _push = 0;
    int _pop = 0;

    // count the number of PUSH instructions
    for (const std::string& line : assembly) {
        if (line.find("PUSH") != std::string::npos)
            _push ++;

        if (line.find("POP") != std::string::npos)
            _pop ++;
    }
    
    // update PUSH instructions that need a corresponding POP
    _push -= _pop;
    
    // add POP instructions based on PUSH count before STOP
    for (int i = 0; i < _push; ++i)
        assembly.push_back("POP");

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
        // if label
        output = "label" + std::to_string(label_counter);
    else
        // ending label
        output = "label" + std::to_string(label_counter - 1) + "_end";

    label_counter ++;
    return output;
}


/**
 * ------------------------------------------
 *          Shifts stack indices
 * 
 *  @param  _stack : current stack structure
 *  @param  i      : instruction, push or pop
 * ------------------------------------------
 */
void Generator::shift(std::vector<stack_data>& _stack, int i) {
    for (auto& element : _stack) {
        if (i == 0)
            element.index ++; // update PUSH
        else
            element.index --; // update POP  
    }
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

    // output success message
    std::cout << "\n";
    std::cout << std::string(35, '=');
    std::cout << "\n";
    std::cout << " [Status]: assembly file created\n";
    std::cout << std::string(35, '=');
    std::cout << "\n\n";

    // close the file
    outputFile.close();
}
