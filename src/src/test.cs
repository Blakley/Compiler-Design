
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
    }
    else if (value == ")") {
        // end of an inner expression            

        // store result of inner expression it its corresponding temporary variable
        assembly.push_back("STORE " + _stack[_stack.size() - 1].temporary);

        // load original temporary variable back into accumulator
        if (_stack.size() > 1) {
            // load previous outter expression's temporary variable into accumulator
            assembly.push_back("LOAD " + _stack[_stack.size() - 2].temporary);

            // perform operation with previous expression and evaluated expression
            assembly.push_back(_stack[_stack.size() - 2].unconsumed_operator + " " + _stack[_stack.size() - 1].temporary);
        }
        else {
            // load original temporary variable back into accumulator
            assembly.push_back("LOAD " + temporary);

            // perform operation with previous expression (in accumulator) and evaluated expression
            assembly.push_back(unconsumed_operator + " " + _stack[_stack.size() - 1].temporary); 
        }
        
        // remove expression
        _stack.pop_back();
    }
    else {
        // handle identifier and integer values
        if (!_stack.empty()) {
            // evalute inner expression

            // load first value into accumulator
            if (_stack[_stack.size() - 1].previous_value.empty()) {
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

// evaluate expression & generate required assembly
for (auto value : node->tokens) {
    if (value == "(") {
        // handle start of new inner-expression
        if (!_stack.empty()) {
            // store the expression in outter, previous, stack variable
            std::string outter = std::to_string(_stack[_stack.size() - 1].index);
            assembly.push_back("STACKW " + outter);
        }
        else
            // store the current expression in the _stack temp variable
            assembly.push_back("STORE _stack");
    
        // create a new stack variable for the inner expression
        struct stack_data data;
        data.index = stack_counter;
        
        assembly.push_back("PUSH");
        stack_counter ++;

        // add data to stack
        _stack.push_back(data);
    }
    else if (value == ")") {
        // handle end of inner-expression
        
        // a) store result of inner expression in its corresponding stack variable
        assembly.push_back("STACKW " + std::to_string(_stack[_stack.size() - 1].index));

        // b) evaluate expression ( <exp> ) with previous expression
        if (_stack.size() > 1) {
            // load previous expression into accumulator then store it in the temp variable
            assembly.push_back("STACKR " + std::to_string(_stack[_stack.size() - 2].index + 1));
            assembly.push_back("STORE _stack");

            // load evaluated expression into accumulator
            assembly.push_back("STACKR " + std::to_string(_stack[_stack.size() - 1].index));
            
            // perform operation with acc and updated _stack temp
            assembly.push_back(_stack[_stack.size() - 2].unconsumed + " _stack");
            
            // store output into stack variable 
            assembly.push_back("STACKW " + std::to_string(_stack[_stack.size() - 1].index));
        }
        else {
            // store current _stack variable onto stack
            assembly.push_back("PUSH");
            assembly.push_back("LOAD _stack");
            assembly.push_back("STACKW " + std::to_string(stack_counter - 1));

            // load previous expression into accumulator then store it in the temp variable
            assembly.push_back("STACKR " + std::to_string(_stack[_stack.size() - 1].index + 1));
            assembly.push_back("STORE _stack");

            // load original _stack temp back into accumulator
            assembly.push_back("STACKR " + std::to_string(stack_counter));

            // perform operation with acc and updated _stack temp
            assembly.push_back(outter_unconsumed + " _stack");    

            // store output into temp (_stack)
            assembly.push_back("STORE _stack");
            
            // remove the _stack temp value from the stack
            assembly.push_back("POP");
        }
        
        // remove expression
        _stack.pop_back();
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
    else if (value == "~") {
        // handle negation operator, negate accumulator
        assembly.push_back("MULT -1"); 
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