/*

    // list of identifiers and keywords
    std::vector<std::string> k = {
        "xopen", "xclose", "xloop", "xdata", "xexit", 
        "xin", "xout", "xcond", "xthen", "xlet", "xfunc"
    };

*/

Maybe have a first() and follow() functions
that determine the first and follow sets of a nonterminal


/*


    * the BNF gets hard-coded into functions. Each nonterminal, 'A', 'B', 'C', etc..
    gets implemented as a function (look at P2 Suggesions filesuggesions file). Name each function the same as the
    nonterminal name: A(), B(), C(), etc ..), each one will generate a node with a label string based on the nonterminal name

    * We'll need to compute First sets whenever there are predictions needed
    (Look at P2 BNF variables File)

    * Building and outputting the tree (look at P2 Tree File):
        To define the node: note that it needs a label, could be string, and should be the same as the function name generating it
        should have any tokens processed in the function (only ids, integers, and relational operators and those from expressions need to be stored),
        and finally will have children for each nonterminal in a production 
        - I believe you hav eat most 4 nonterminals per production

    * We'll have a global variable "current_token" that will be used in each Nonterminal function

    * remove print outputs in scanner other than error outputs

*/

Call next nonterminal function
 // // determine which nonterminal the token belongs to
    // if (identify(_token)) {
    //     // get corresponding function
    //     auto it = nonterminals.find(_token.instance);
        
    //     // call the function
    //     (this->*(it->second))();
    // }
    // else {
    //     // todo: handle non-terminal case
    //     std::cout << "token isn't a nonterminal\n";
    // }


Figure out main parser function 
to see when to call/retrieve the next tokens